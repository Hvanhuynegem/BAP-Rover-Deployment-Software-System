#include "parameter.h"

// TODO add test to check if ids and indices match.
Parameter_info_t &Parameter_t::get_info(Parameter_id_t id)
{
    for(Parameter_info_t &info: info_array)
    {
        if(info.id == id)
        {
            return info;
        }
    }

    // Return first param info in array if not found
    // TODO: add error checking on this somehow
    return info_array[0];
}

Parameter_t::Parameter_t(Parameter_id_t id, void *data) : info(get_info(id)), data(data)
{
}

Parameter_id_t Parameter_t::get_id()
{
    return info.id;
}

void *Parameter_t::get_data()
{
    return data;
}

Parameter_type_t Parameter_t::get_type()
{
    return info.type;
}

uint8_t Parameter_t::get_size()
{
    return info.size;
}

const char *Parameter_t::get_name(Parameter_id_t id)
{
    return info_array[id].name;
}

// TODO: add test to check if ids and indices match.
Parameter_info_t Parameter_t::info_array[PARAMETER_COUNT] = {
    {TEST, VALUE, sizeof(uint16_t), "Test value (uint16_t)"}, {WALK_DISTANCE, VALUE, sizeof(uint16_t), "Walk Distance (cm)"}, {RADIO_DATA, ARRAY, 255, "Raw radio data"}, {COUNTER, VALUE, 4, "Counter"}, {HELLO, ARRAY, 6, "Hello"}
    // ...
};
