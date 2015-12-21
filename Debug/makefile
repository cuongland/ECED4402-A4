################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/arm_5.1.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./uart.obj" \
"./train.obj" \
"./testcase.obj" \
"./systick.obj" \
"./sysctl.obj" \
"./switch.obj" \
"./spi.obj" \
"./semaphore.obj" \
"./railsystem.obj" \
"./process.obj" \
"./messageuart.obj" \
"./messageatmel.obj" \
"./memory.obj" \
"./main.obj" \
"./lm3s9d92_startup_ccs.obj" \
"./gpiof.obj" \
"./gpioe.obj" \
"./cursor.obj" \
"./SVCCall.obj" \
"./Queue.obj" \
"./PriorityQueue.obj" \
"./PendSVHandler.obj" \
"./PcbQueue.obj" \
"./Kernel.obj" \
"./HallSensor.obj" \
"./ControlSoftware.obj" \
"../lm3s9d92.cmd" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: a4.out

# Tool invocations
a4.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -g --diag_warning=225 --display_error_number --diag_wrap=off -z -m"a4.map" --heap_size=0x400 --stack_size=0x400 -i"C:/ti/ccsv5/tools/compiler/arm_5.1.1/lib" -i"C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="a4_linkInfo.xml" --rom_model -o "a4.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(TMS470_EXECUTABLE_OUTPUTS__QUOTED) "a4.out"
	-$(RM) "ControlSoftware.pp" "HallSensor.pp" "Kernel.pp" "PcbQueue.pp" "PendSVHandler.pp" "PriorityQueue.pp" "Queue.pp" "SVCCall.pp" "cursor.pp" "gpioe.pp" "gpiof.pp" "lm3s9d92_startup_ccs.pp" "main.pp" "memory.pp" "messageatmel.pp" "messageuart.pp" "process.pp" "railsystem.pp" "semaphore.pp" "spi.pp" "switch.pp" "sysctl.pp" "systick.pp" "testcase.pp" "train.pp" "uart.pp" 
	-$(RM) "ControlSoftware.obj" "HallSensor.obj" "Kernel.obj" "PcbQueue.obj" "PendSVHandler.obj" "PriorityQueue.obj" "Queue.obj" "SVCCall.obj" "cursor.obj" "gpioe.obj" "gpiof.obj" "lm3s9d92_startup_ccs.obj" "main.obj" "memory.obj" "messageatmel.obj" "messageuart.obj" "process.obj" "railsystem.obj" "semaphore.obj" "spi.obj" "switch.obj" "sysctl.obj" "systick.obj" "testcase.obj" "train.obj" "uart.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

