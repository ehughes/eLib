################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: MonkeyBootTestApp\ -\ Valid.axf

# Tool invocations
MonkeyBootTestApp\ -\ Valid.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"Z:\Dropbox\eLib\MonkeyBoot\SRC\lpc_board_keil_mcb_4357\Debug" -L"Z:\Dropbox\eLib\MonkeyBoot\SRC\lpc_chip_43xx\Debug" -Xlinker -Map="MonkeyBootTestApp - Valid.map" -Xlinker --gc-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -T "MonkeyBootTestApp_-_Valid_Debug.ld" -o "MonkeyBootTestApp - Valid.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) "MonkeyBootTestApp - Valid.axf"
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "MonkeyBootTestApp - Valid.axf" ; arm-none-eabi-objcopy -v -O ihex "MonkeyBootTestApp - Valid.axf" "boot.hex"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
