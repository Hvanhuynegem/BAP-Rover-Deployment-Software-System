################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./src/communication/lander_communication.obj" \
"./src/communication/lander_communication_protocol.obj" \
"./src/communication/uart_communication.obj" \
"./src/communication/learning to use UART/PinHigh.obj" \
"./src/communication/learning to use UART/SLIP encoding.obj" \
"./src/communication/learning to use UART/SLIP retransmission.obj" \
"./src/communication/learning to use UART/UART transmission using buffer.obj" \
"./src/communication/learning to use UART/UART transmit pin test.obj" \
"./src/deployment/actuatorControl.obj" \
"./src/deployment/deploymentControl.obj" \
"./src/deployment/umbilicalCordManagement.obj" \
"./src/main.obj" \
"./src/powerManagement/batteryManagement.obj" \
"./src/powerManagement/powerControl.obj" \
"./src/powerManagement/sensorData.obj" \
"./src/systemHealth/heatResistorControl.obj" \
"./src/utilities/errorHandler.obj" \
"./src/utilities/logger.obj" \
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
-include src/communication/subdir_vars.mk
-include src/communication/learning\ to\ use\ UART/subdir_vars.mk
-include src/deployment/subdir_vars.mk
-include src/subdir_vars.mk
-include src/powerManagement/subdir_vars.mk
-include src/systemHealth/subdir_vars.mk
-include src/utilities/subdir_vars.mk
-include subdir_rules.mk
-include src/communication/subdir_rules.mk
-include src/communication/learning\ to\ use\ UART/subdir_rules.mk
-include src/deployment/subdir_rules.mk
-include src/subdir_rules.mk
-include src/powerManagement/subdir_rules.mk
-include src/systemHealth/subdir_rules.mk
-include src/utilities/subdir_rules.mk
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
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "rover deployment software system.out"

# Tool invocations
rover\ deployment\ software\ system.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --advice:power=all --advice:hw_config=all --define=__MSP430FR5969__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 -z -m"rover deployment software system.map" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on -i"C:/ti/ccs1271/ccs/ccs_base/msp430/include" -i"C:/ti/ccs1271/ccs/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"C:/ti/ccs1271/ccs/ccs_base/msp430/lib/FR59xx" -i"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/lib" -i"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --priority --reread_libs --define=_MPU_ENABLE --diag_wrap=off --display_error_number --warn_sections --xml_link_info="rover deployment software system_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "rover deployment software system.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "src\communication\lander_communication.obj" "src\communication\lander_communication_protocol.obj" "src\communication\uart_communication.obj" "src\communication\learning to use UART\PinHigh.obj" "src\communication\learning to use UART\SLIP encoding.obj" "src\communication\learning to use UART\SLIP retransmission.obj" "src\communication\learning to use UART\UART transmission using buffer.obj" "src\communication\learning to use UART\UART transmit pin test.obj" "src\deployment\actuatorControl.obj" "src\deployment\deploymentControl.obj" "src\deployment\umbilicalCordManagement.obj" "src\main.obj" "src\powerManagement\batteryManagement.obj" "src\powerManagement\powerControl.obj" "src\powerManagement\sensorData.obj" "src\systemHealth\heatResistorControl.obj" "src\utilities\errorHandler.obj" "src\utilities\logger.obj" 
	-$(RM) "src\communication\lander_communication.d" "src\communication\lander_communication_protocol.d" "src\communication\uart_communication.d" "src\communication\learning to use UART\PinHigh.d" "src\communication\learning to use UART\SLIP encoding.d" "src\communication\learning to use UART\SLIP retransmission.d" "src\communication\learning to use UART\UART transmission using buffer.d" "src\communication\learning to use UART\UART transmit pin test.d" "src\deployment\actuatorControl.d" "src\deployment\deploymentControl.d" "src\deployment\umbilicalCordManagement.d" "src\main.d" "src\powerManagement\batteryManagement.d" "src\powerManagement\powerControl.d" "src\powerManagement\sensorData.d" "src\systemHealth\heatResistorControl.d" "src\utilities\errorHandler.d" "src\utilities\logger.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

