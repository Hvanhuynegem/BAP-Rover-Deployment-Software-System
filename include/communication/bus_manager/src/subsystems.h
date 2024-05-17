/**
 * Author(s):
 *    Marco van Eerden
 *
 * Contains the subsystem codes for use in a wide array of applications.
 */

#pragma once

#include <stdint.h>

namespace subsystems
{

/* Subsystem, used for source and destination. */
enum Subsystem_t : uint8_t
{
    NONE = 0u,
    OBC = 1u,
    PPU = 2u,
    SHRIMP1 = 3u,
    SHRIMP2 = 4u,
    LEG1 = 5u,
    LEG2 = 6u,
    LEG3 = 7u,
    LEG4 = 8u,
    LEG5 = 9u,
    LEG6 = 10u,
    SOLAR_PANEL = 11u,
    BMS = 12u,
    PAYLOAD = 13u,
    COMMS = 14u,
    COMMS_APP = 15u,

    __SUBSYSTEM_END__
};

}    // namespace subsystems
