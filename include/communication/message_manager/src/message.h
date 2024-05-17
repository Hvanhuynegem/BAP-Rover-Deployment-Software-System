#pragma once

#include <communication/message_manager/src/parameter.h>

#define MAX_DATA_SIZE MAX_MESSAGE_SIZE - HEADER_MESSAGE_SIZE

typedef enum : uint8_t
{
    UNDF = 0U,
    TRON = 1U,
    ROUTER = 2U,
    SHRIMP = 3U,
    OPAL = 4U,
    PAYLOAD = 5U,
    LOCO = 6U,
    COMMS = 7U,
    LAND = 8U,
    UMBI = 9U,
    DX = 10U,
    BUS_MANAGER = 11U,
} App_id_t;

typedef enum : uint8_t
{
    ALIVE,
    START,
    STOP,
    SLEEP,
    WALK,
    CONFIRM,
    FAIL,
    LEG_ROTATE,
    LEG_REQUEST,
    LEG_RESPONSE,
    STATUS_REQUEST,
    STATUS_RESPONSE,
    LOG_REQUEST,
    LOG_RESPONSE,
    TRANSMISSION,
    RECEPTION,
    // TODO: Remove test procedures
    PING,
    PONG,
    HELLO_WORLD,
} Procedure_t;

class Message_t
{
  public:
    enum class Message_error_t : uint8_t
    {
        SUCCESS = 0U,
        INVALID_PARAMETER = 1U,
        BUFFER_OVERFLOW = 2U,
        // Add more error types as needed
    };

    static constexpr uint8_t max_message_size = 255;
    static constexpr uint8_t header_size = sizeof(App_id_t) + sizeof(App_id_t) + sizeof(Procedure_t) + 2 * sizeof(uint8_t);
    static constexpr uint8_t max_data_size = max_message_size - header_size;

    Message_t();
    Message_t(App_id_t src, App_id_t dest, Procedure_t proc);

    Message_error_t from_bytes(void *buffer, bool copy_data = true);                         // TODO: implement message error return type
    Message_error_t to_bytes(void *buffer, uint16_t &buffer_size, bool copy_data = true);    // TODO: implement message error return type

    bool add_parameter(Parameter_id_t param_id, void *data);

    constexpr uint8_t get_header_size()
    {
        return header_size;
    }

    uint8_t get_data_size();
    uint8_t *get_data();
    uint8_t get_size();
    App_id_t get_source();
    App_id_t get_destination();
    Procedure_t get_procedure();
    uint8_t get_parameter_count();
    uint8_t get_parameter(uint8_t index);    // TODO: Implement this

  private:
    App_id_t source;
    App_id_t destination;
    Procedure_t procedure;
    uint8_t parameter_count;
    uint8_t data_size;
    uint8_t data[max_data_size];
};
