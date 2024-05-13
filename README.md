# Rover Deployment Software System (RDSS)

## Overview
The Rover Deployment Software System (RDSS) is designed to manage and coordinate the operations between a lander and a rover during lunar exploration missions. The system handles critical functionalities including communication, power management, deployment sequencing, and system health monitoring.

## Key Features
- **Communication**: Manages data transmission between the lander and rover using RS-485 and Bluetooth Low Energy (BLE) protocols.
- **Power Management**: Controls power delivery from the lander to the rover, ensuring optimal power levels throughout the mission.
- **Deployment**: Executes a sequence of operations to deploy the rover upon the lander reaching the lunar surface.
- **System Health Monitoring**: Monitors the health of the system using temperature sensors and implements necessary actions based on the sensor data.

## Components
- **Internal Communication**: Facilitates robust communication between two MSP430FR5969 MCUs configured as Master and Slave.
- **RDS-Rover Communication**: Utilizes BLE for wireless data transfer and RS-485 for wired communication through an umbilical cord.
- **RDS-Lander Communication**: Employs Serial Line Internet Protocol (SLIP) for reliable data handling.
- **Power Management System**: Integrates sensors for real-time voltage and current monitoring, controlling the energy supply to the rover's systems.
- **Deployment Control**: Manages the activation of non-explosive actuators (NEAs) to ensure proper deployment of the rover.
- **Exception Handling**: Robust exception handling framework to manage edge cases and ensure system resilience.

