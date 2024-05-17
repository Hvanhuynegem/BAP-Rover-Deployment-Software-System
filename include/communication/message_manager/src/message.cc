#include <cstdio>
#include <cstring>
#include <communication/message_manager/src/message.h>

Message_t::Message_t()
{
    source = UNDF;
    destination = UNDF;
    procedure = ALIVE;
    parameter_count = 0;
    data_size = 0;
}

Message_t::Message_t(App_id_t src, App_id_t dest, Procedure_t proc) : source(src), destination(dest), procedure(proc), parameter_count(0), data_size(0)
{
}

// NOTE: In C++ it may be better to use std::span and std::array
Message_t::Message_error_t Message_t::from_bytes(void *buffer, bool copy_data)
{
    uint8_t *byte_buffer = static_cast<uint8_t *>(buffer);

    source = (App_id_t)byte_buffer[0];
    destination = (App_id_t)byte_buffer[1];
    procedure = (Procedure_t)byte_buffer[2];
    parameter_count = byte_buffer[3];
    data_size = byte_buffer[4];

    if(copy_data)
    {
        if(data_size > max_data_size)
        {
            return Message_error_t::BUFFER_OVERFLOW;
        }
        memcpy(data, &byte_buffer[5], data_size);
    }

    return Message_error_t::SUCCESS;
}

Message_t::Message_error_t Message_t::to_bytes(void *buffer, uint16_t &buffer_size, bool copy_data)
{

    uint8_t *byte_buffer = static_cast<uint8_t *>(buffer);

    byte_buffer[0] = source;
    byte_buffer[1] = destination;
    byte_buffer[2] = procedure;
    byte_buffer[3] = parameter_count;
    byte_buffer[4] = data_size;

    if(copy_data)
    {
        memcpy(&byte_buffer[5], data, data_size);
    }

    buffer_size = get_size();

    return Message_error_t::SUCCESS;
}

bool Message_t::add_parameter(Parameter_id_t param_id, void *param_data)
{
    // Create a new parameter with the given id and data
    Parameter_t param(param_id, param_data);

    // Calculate the new data size after adding the parameter
    size_t new_data_size = data_size + 1 + param.get_size();

    // Check if the new data size does not exceed the maximum,
    // and if the data pointer is not null
    if(new_data_size <= max_data_size && param_data != 0)
    {
        // Increment the parameter count
        parameter_count++;

        // Add the parameter id to the data array
        this->data[get_data_size()] = param_id;

        // Update the data size
        data_size = new_data_size;

        // Copy the parameter data to the data array
        memcpy(&(this->data[data_size - param.get_size()]), param_data, param.get_size());

        // Parameter added successfully
        return true;
    }

    // Could not add parameter
    return false;
}

uint8_t Message_t::get_data_size()
{
    return data_size;
}

uint8_t *Message_t::get_data()
{
    return data;
}

uint8_t Message_t::get_parameter_count()
{
    return parameter_count;
}

uint8_t Message_t::get_size()
{
    return get_header_size() + data_size;
}

App_id_t Message_t::get_destination()
{
    return destination;
}

App_id_t Message_t::get_source()
{
    return source;
}

Procedure_t Message_t::get_procedure()
{
    return procedure;
}
