################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/tms470_4.9.5

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./uartstdio.obj" \
"./startup_ccs.obj" \
"./main.obj" \
"./VariableMotor.obj" \
"./UserInterface.obj" \
"./Tester.obj" \
"./PrinterTest.obj" \
"./PIDController.obj" \
"./MotorDrive.obj" \
"./MagAccComms.obj" \
"./MagAcc.obj" \
"./Klist.obj" \
"./HBridgeComms.obj" \
"./Globals.obj" \
"./BlueUI.obj" \
"./BlueUART.obj" \
"./AutoTunePID.obj" \
"./AutoHelm.obj" \
-l"libc.a" \
-l"C:\StellarisWare\usblib\ccs-cm4f\Debug\usblib-cm4f.lib" \
-l"c:/StellarisWare/driverlib/ccs-cm4f/Debug/driverlib-cm4f.lib" \
"../lm4f120h5qr.cmd" \

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
all: MetalPrinterTest.out

# Tool invocations
MetalPrinterTest.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv5/tools/compiler/tms470_4.9.5/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --define=DEBUG --define=TARGET_IS_BLIZZARD_RA1 --define=PART_LM4F121H5QR --diag_warning=225 --display_error_number -z --stack_size=5120 -m"MetalPrinterTest.map" --heap_size=0 -i"C:/ti/ccsv5/tools/compiler/tms470_4.9.5/lib" -i"C:/ti/ccsv5/tools/compiler/tms470_4.9.5/include" --reread_libs --warn_sections --display_error_number --rom_model -o "MetalPrinterTest.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(TMS470_EXECUTABLE_OUTPUTS__QUOTED) "MetalPrinterTest.out"
	-$(RM) "AutoHelm.pp" "AutoTunePID.pp" "BlueUART.pp" "BlueUI.pp" "Globals.pp" "HBridgeComms.pp" "Klist.pp" "MagAcc.pp" "MagAccComms.pp" "MotorDrive.pp" "PIDController.pp" "PrinterTest.pp" "Tester.pp" "UserInterface.pp" "VariableMotor.pp" "main.pp" "startup_ccs.pp" "uartstdio.pp" 
	-$(RM) "AutoHelm.obj" "AutoTunePID.obj" "BlueUART.obj" "BlueUI.obj" "Globals.obj" "HBridgeComms.obj" "Klist.obj" "MagAcc.obj" "MagAccComms.obj" "MotorDrive.obj" "PIDController.obj" "PrinterTest.obj" "Tester.obj" "UserInterface.obj" "VariableMotor.obj" "main.obj" "startup_ccs.obj" "uartstdio.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
