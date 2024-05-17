#pragma once
#include <stdint.h>

#define MAX_NAME_LENGTH 50
#define PARAMETER_COUNT 20    // Total number of defined parameter types

enum Parameter_id_t : uint8_t
{
    TEST,
    WALK_DISTANCE,
    RADIO_DATA,
    COUNTER,
    HELLO
    // ...
};

enum Parameter_type_t : uint8_t
{
    VALUE,
    ARRAY
};

struct Parameter_info_t
{
    Parameter_id_t id;
    Parameter_type_t type;
    uint8_t size;
    char name[MAX_NAME_LENGTH];
};

class Parameter_t
{
  public:
    Parameter_t(Parameter_id_t id, void *data);

    Parameter_id_t get_id();
    void *get_data();
    Parameter_type_t get_type();
    uint8_t get_size();

    static Parameter_info_t &get_info(Parameter_id_t id);
    static const char *get_name(Parameter_id_t id);

  private:
    Parameter_info_t &info;
    void *data;

    static Parameter_info_t info_array[PARAMETER_COUNT];
};
