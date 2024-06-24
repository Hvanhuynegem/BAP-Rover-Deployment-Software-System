/*
 * payload_messages.h
 *
 * This header file contains definitions for static payload messages used throughout the project.
 *
 * Author: Henri Vanhuynegem
 * created: 19/06/2024
 * Last edited: 19/06/2024
 *
 */

#ifndef PAYLOAD_MESSAGES_H
#define PAYLOAD_MESSAGES_H

#include <stdint.h>

// Example payload messages

// initialisation and acknowledgements
static const uint8_t PAYLOAD_INIT[] = "INIT";
static const uint8_t PAYLOAD_ACK[] = "ACK";
static const uint8_t PAYLOAD_NACK[] = "NACK";
// transit modes
static const uint8_t PAYLOAD_GENERAL_STARTUP[] = "GENERAL_STARTUP";
static const uint8_t PAYLOAD_LAUNCH_INTEGRATION[] = "LAUNCH_INTEGRATION";
static const uint8_t PAYLOAD_TRANSIT[] = "TRANSIT";
static const uint8_t PAYLOAD_PRE_DEPLOYMENT[] = "PRE_DEPLOYMENT";
static const uint8_t PAYLOAD_DEPLOYMENT[] = "DEPLOYMENT";

// transit mode request
static const uint8_t PAYLOAD_TRANSIT_MODE[] = "TM";

// electronic components checkup messages
static const uint8_t PAYLOAD_UMBILICAL_CONNECTED[] = "umbilical cord connected";
static const uint8_t PAYLOAD_UMBILICAL_NOT_CONNECTED[] = "umbilical cord not connected";
static const uint8_t PAYLOAD_BUS_SENSE_BROKEN[] = "The bus voltage cannot be measured";
static const uint8_t PAYLOAD_TEMP_SENSOR_1_BROKEN[] = "Temperature sensor 1 is broken";
static const uint8_t PAYLOAD_TEMP_SENSOR_2_BROKEN[] = "Temperature sensor 2 is broken";
static const uint8_t PAYLOAD_SUPERCAP_VOLTAGE_ERROR[] = "The supercap voltage cannot be read";
static const uint8_t PAYLOAD_SUPERCAP_VOLTAGE_ZERO[] = "The supercap voltage is 0V";
static const uint8_t PAYLOAD_ALL_NEA_READY[] = "All the NEAs are ready";
static const uint8_t PAYLOAD_NEA1_READY[] = "NEA 1 is ready";
static const uint8_t PAYLOAD_NEA1_NOT_READY[] = "NEA 1 is not ready";
static const uint8_t PAYLOAD_NEA2_READY[] = "NEA 2 is ready";
static const uint8_t PAYLOAD_NEA2_NOT_READY[] = "NEA 2 is not ready";
static const uint8_t PAYLOAD_NEA3_READY[] = "NEA 3 is ready";
static const uint8_t PAYLOAD_NEA3_NOT_READY[] = "NEA 3 is not ready";
static const uint8_t PAYLOAD_NEA4_READY[] = "NEA 4 is ready";
static const uint8_t PAYLOAD_NEA4_NOT_READY[] = "NEA 4 is not ready";
static const uint8_t PAYLOAD_SUPERCAP1_READY[] = "supercapacitor 1 is ready";
static const uint8_t PAYLOAD_SUPERCAP1_NOT_READY[] = "supercapacitor 1 is not ready";
static const uint8_t PAYLOAD_SUPERCAP2_READY[] = "supercapacitor 2 is ready";
static const uint8_t PAYLOAD_SUPERCAP2_NOT_READY[] = "supercapacitor 2 is not ready";
static const uint8_t PAYLOAD_SUPERCAP3_READY[] = "supercapacitor 3 is ready";
static const uint8_t PAYLOAD_SUPERCAP3_NOT_READY[] = "supercapacitor 3 is not ready";
static const uint8_t PAYLOAD_POWER_ROVER_OFF[] = "Power to the rover is switched off";
static const uint8_t PAYLOAD_DEPLOYMENT_COMPLETE[] = "Deployment is complete";

// error messages
static const uint8_t PAYLOAD_ERROR[] = "ERROR_MESSAGE";
static const uint8_t PAYLOAD_TOO_LARGE[] = "MESSAGE_TOO_LARGE";
static const uint8_t PAYLOAD_INVALID_MESSAGE[] = "INVALID_MESSAGE";
static const uint8_t PAYLOAD_INVALID_CHECKSUM[] = "INVALID_CHECKSUM";
static const uint8_t PAYLOAD_EMPTY[] = "";


#endif // PAYLOAD_MESSAGES_H
