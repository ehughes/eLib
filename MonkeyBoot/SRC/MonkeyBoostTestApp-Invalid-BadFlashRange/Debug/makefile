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
all: MonkeyBootTestApp-InvalidMemoryRange.axf

# Tool invocations
MonkeyBootTestApp-InvalidMemoryRange.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="MonkeyBootTestApp-InvalidMemoryRange.map" -Xlinker --gc-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -T "MonkeyBootTestApp-InvalidMemoryRange_Debug.ld" -o "MonkeyBootTestApp-InvalidMemoryRange.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) MonkeyBootTestApp-InvalidMemoryRange.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "MonkeyBootTestApp-InvalidMemoryRange.axf" ; arm-none-eabi-objcopy -v -O ihex "MonkeyBootTestApp-InvalidMemoryRange.axf" "boot.hex" ; # arm-none-eabi-objcopy -v -O binary "MonkeyBootTestApp-InvalidMemoryRange.axf" "MonkeyBootTestApp-InvalidMemoryRange.bin" ; # checksum -p LPC4357 -d "MonkeyBootTestApp-InvalidMemoryRange.bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
