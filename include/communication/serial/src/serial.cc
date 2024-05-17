#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <cstring>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include "serial.h"

/* Old termios struct so we can restore the old configuration if necessary, */
struct termios old_struct;

uint32_t serial_close(int serial_fd)
{
    /* Disable RS485. */
    struct serial_rs485 rs485conf;
    rs485conf.flags = 0;
    ioctl(serial_fd, TIOCSRS485, &rs485conf);

    /* Restore old settings. */
    // TODO: do we want to do this?
    // tcsetattr(serial_fd, TCSANOW, &old_struct);

    return close(serial_fd);
}

uint32_t serial_configure(const char *serial_port, Baudrate_t baudrate, Serial_mode_t mode, int *serial_fd)
{
    int flags;
    struct termios serial;

    fprintf(stdout, "[SERIAL] Opening file descriptor %s\n", serial_port);

    /* Open the device file */
    *serial_fd = open(serial_port, O_RDWR | O_NOCTTY | O_NONBLOCK | O_SYNC);

    if(*serial_fd == -1)
    {
        fprintf(stderr, "[SERIAL] Error creating file descriptor with code %d\n", errno);
        return -1;
    }

    /* Get current serial atributes. */
    if(tcgetattr(*serial_fd, &old_struct) < 0)
    {
        fprintf(stderr, "[SERIAL] Error getting termios attributes with code %d\n", errno);
        return -2;
    }

    /* Copy from old_struct to serial so we can still restore the old settings if necessary. */
    memcpy(&serial, &old_struct, sizeof(struct termios));

    /* Explicitely bit flip the NONBLOCKING flag, if needed */
    if(mode == SERIAL_BLOCKING)
    {
        flags = fcntl(*serial_fd, F_GETFL, 0); /* Get the option flags for *serial_fd */
        flags &= ~O_NONBLOCK;                  /* Clear the O_NONBLOCK option */
        fcntl(*serial_fd, F_SETFL, flags);     /* Try to make it a blocking-mode file descriptor */
    }

    /* https://www.mkssoftware.com/docs/man5/struct_termios.5.asp
     * See the meaning of c_xflag here and the definition of struct termios in general.
     */
    serial.c_iflag = 0;
    serial.c_oflag = 0;
    serial.c_lflag = 0;
    serial.c_cflag = CS8 | CREAD;

    serial.c_cc[VMIN] = 0;
    serial.c_cc[VTIME] = 255;

    switch(baudrate)
    {
        case BAUD9600:
            serial.c_cflag |= B9600;
            break;
        case BAUD115200:
            serial.c_cflag |= B115200;
            break;
        default:
            fprintf(stderr, "[SERIAL] Invalid baud rate %d\n", baudrate);
            return -3;
    }

    /* Apply the configuration above */
    if(tcsetattr(*serial_fd, TCSANOW, &serial) < 0)
    {
        fprintf(stderr, "[SERIAL] Error setting termios attributes with code %d\n", errno);
        return -4;
    }

    /* Initialise RS485. */
    struct serial_rs485 rs485conf;
    int extra_space = 0;

    /* Used to calculate a dynamic extra range when the DE pin would
     * be toggled before- and after the data. Unused as for now */
    // int extra_space = floor(((1.0 / ((float) baud / 10.0)) * 1000.0));

    /* Enable RS485 mode: */
    rs485conf.flags |= SER_RS485_ENABLED;

    /* Set logical level for RTS pin equal to 1 when sending: */
    rs485conf.flags |= SER_RS485_RTS_ON_SEND;

    /* or, set logical level for RTS pin equal to 0 after sending: */
    rs485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);

    /* Set rts delay before send, if needed: */
    rs485conf.delay_rts_before_send = extra_space;

    /* Set rts delay after send, if needed: */
    rs485conf.delay_rts_after_send = extra_space;

    /* Set this flag if you want to receive data even while sending data */
    rs485conf.flags &= ~(SER_RS485_RX_DURING_TX);

    if(ioctl(*serial_fd, TIOCSRS485, &rs485conf) < 0)
    {
        fprintf(stderr, "[SERIAL] Failed to initialize the RS-485 driver with error %d\n", errno);
        return -5;
    }

    return 0;
}

uint32_t serial_write(int serial_fd, char *data, size_t length)
{
    ssize_t count;
    uint32_t error;

    count = write(serial_fd, data, length);

    /* Handle any potential error regarding writing to serial file descriptor */
    if(count == -1)
    {
        fprintf(stderr, "[SERIAL] Error writing to bus with code %d\n", errno);
        error = 1u;
    }
    else if((size_t)count != length)
    {
        fprintf(stderr, "[SERIAL] Count %ld not equal to length %ld\n", count, length);
        error = 2u;
    }
    else
    {
        error = 0u;
    }

    return error;
}

uint32_t serial_read(int serial_fd, char *data, size_t *length, uint32_t timeout_milliseconds)
{
    int count;
    int fd;
    fd_set read_fds, write_fds, except_fds;
    struct timeval timeout;
    int bytes_available;          /* ioctl() will set this variable which tells how many bytes have been received already. */
    size_t total_bytes_read = 0u; /* Variable holding the amount of bytes read to far. */
    bool timeout_expired = false;

    /* Define file descriptors. */
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(serial_fd, &read_fds);

    /* Set timeout in milliseconds. */
    timeout.tv_sec = 0;
    timeout.tv_usec = timeout_milliseconds * 1000u;

    /* Set length to 0 to avoid arbitrary number on error. */
    *length = 0;

    /* Flush buffer before reading. */
    tcflush(serial_fd, TCIFLUSH);

    while(!timeout_expired)
    {
        if(select(serial_fd + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1)
        {
            /* Read a set number of bytes. */
            ioctl(serial_fd, FIONREAD, &bytes_available);
            count = read(serial_fd, data + total_bytes_read, bytes_available);

            /* Handle read error. */
            if(count == -1)
            {
                fprintf(stderr, "[SERIAL] Failed to read from bus with code\n", errno);
                return 1u;
            }

            /* Add to total bytes read. */
            total_bytes_read = total_bytes_read + static_cast<size_t>(count);

            /* Check if the first data byte is equal to the preamble. */
            if(data[0] != 0x3Au)
            {
                fprintf(stderr, "[SERIAL] Invalid preamble %02x\n", data[0]);
                return 3u;
            }

            // TODO: return early with code 6 if destination is not correct

            /* Store the length if at least 5 bytes have been read (length is 5th byte). */
            if(total_bytes_read >= 5u)
            {
                *length = 5u + data[4] + 3u;
            }

            /* Stop if we have read enough bytes. Stop condition can only be true when length is defined. */
            if((total_bytes_read > 5u) && (total_bytes_read >= *length))
            {
                break;
            }
        }
        else
        {
            /* Timeout has been expired */
            fprintf(stderr, "[SERIAL] Timeout expired.\n");
            timeout_expired = true;
            return 2u;
        }
    }

    return 0u;
}
