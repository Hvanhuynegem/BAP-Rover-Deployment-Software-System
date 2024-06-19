################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./src/lander_communication/lander_communication.obj" \
"./src/lander_communication/lander_communication_protocol.obj" \
"./src/lander_communication/uart_communication.obj" \
"./src/lander_communication/learning to use UART/PinHigh.obj" \
"./src/lander_communication/learning to use UART/SLIP encoding.obj" \
"./src/lander_communication/learning to use UART/SLIP retransmission.obj" \
"./src/lander_communication/learning to use UART/UART transmission using buffer.obj" \
"./src/lander_communication/learning to use UART/UART transmit pin test.obj" \
"./src/main.obj" \
"./src/system_health/ECCS.obj" \
"./src/system_health/NEA_readout.obj" \
"./src/system_health/bus_current_readout.obj" \
"./src/system_health/heat_resistor_control.obj" \
"./src/system_health/main_system_init.obj" \
"./src/system_health/supercap_readout.obj" \
"./src/system_health/temp_sensors.obj" \
"./src/system_health/umbilical_cord.obj" \
"./src/transit_modes/deployment.obj" \
"./src/transit_modes/general_startup.obj" \
"../lnk_msp430fr5969.cmd" \
$(GEN_CMDS__FLAG) \
-llibmpu_init.a \
-llibmath.a \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/lander_communication/subdir_vars.mk
-include src/lander_communication/learning\ to\ use\ UART/subdir_vars.mk
-include src/subdir_vars.mk
-include src/system_health/subdir_vars.mk
-include src/transit_modes/subdir_vars.mk
-include subdir_rules.mk
-include src/lander_communication/subdir_rules.mk
-include src/lander_communication/learning\ to\ use\ UART/subdir_rules.mk
-include src/subdir_rules.mk
-include src/system_health/subdir_rules.mk
-include src/transit_modes/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
rover\ deployment\ software\ system.out 

EXE_OUTPUTS__QUOTED += \
"rover deployment software system.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(A_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "rover deployment software system.out"

# Tool invocations
rover\ deployment\ software\ system.out: $(OBJS) $(CMD_SRCS) $(A_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --advice:power=all --advice:hw_config=all --define=__MSP430FR5969__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 -z -m"rover deployment software system.map" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on -i"C:/ti/ccs1271/ccs/ccs_base/msp430/include" -i"C:/ti/ccs1271/ccs/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"C:/ti/ccs1271/ccs/ccs_base/msp430/lib/FR59xx" -i"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/lib" -i"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --priority --reread_libs --define=_MPU_ENABLE --diag_wrap=off --display_error_number --warn_sections --xml_link_info="rover deployment software system_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "rover deployment software system.out" $(ORDERED_OBJS) $(A_SRCS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "src\lander_communication\lander_communication.obj" "src\lander_communication\lander_communication_protocol.obj" "src\lander_communication\uart_communication.obj" "src\lander_communication\learning to use UART\PinHigh.obj" "src\lander_communication\learning to use UART\SLIP encoding.obj" "src\lander_communication\learning to use UART\SLIP retransmission.obj" "src\lander_communication\learning to use UART\UART transmission using buffer.obj" "src\lander_communication\learning to use UART\UART transmit pin test.obj" "src\main.obj" "src\system_health\ECCS.obj" "src\system_health\NEA_readout.obj" "src\system_health\bus_current_readout.obj" "src\system_health\heat_resistor_control.obj" "src\system_health\main_system_init.obj" "src\system_health\supercap_readout.obj" "src\system_health\temp_sensors.obj" "src\system_health\umbilical_cord.obj" "src\transit_modes\deployment.obj" "src\transit_modes\general_startup.obj" 
	-$(RM) "src\lander_communication\lander_communication.d" "src\lander_communication\lander_communication_protocol.d" "src\lander_communication\uart_communication.d" "src\lander_communication\learning to use UART\PinHigh.d" "src\lander_communication\learning to use UART\SLIP encoding.d" "src\lander_communication\learning to use UART\SLIP retransmission.d" "src\lander_communication\learning to use UART\UART transmission using buffer.d" "src\lander_communication\learning to use UART\UART transmit pin test.d" "src\main.d" "src\system_health\ECCS.d" "src\system_health\NEA_readout.d" "src\system_health\bus_current_readout.d" "src\system_health\heat_resistor_control.d" "src\system_health\main_system_init.d" "src\system_health\supercap_readout.d" "src\system_health\temp_sensors.d" "src\system_health\umbilical_cord.d" "src\transit_modes\deployment.d" "src\transit_modes\general_startup.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

