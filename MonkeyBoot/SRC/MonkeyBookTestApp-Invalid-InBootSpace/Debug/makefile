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
all: MonkeyBookTestApp-Invalid-InBootSpace.axf

# Tool invocations
MonkeyBookTestApp-Invalid-InBootSpace.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="MonkeyBookTestApp-Invalid-InBootSpace.map" -Xlinker --gc-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -T "MonkeyBookTestApp-Invalid-InBootSpace_Debug.ld" -o "MonkeyBookTestApp-Invalid-InBootSpace.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) MonkeyBookTestApp-Invalid-InBootSpace.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "MonkeyBookTestApp-Invalid-InBootSpace.axf" ; arm-none-eabi-objcopy -v -O ihex "MonkeyBookTestApp-Invalid-InBootSpace.axf" "boot.hex" ; # arm-none-eabi-objcopy -v -O binary "MonkeyBookTestApp-Invalid-InBootSpace.axf" "MonkeyBookTestApp-Invalid-InBootSpace.bin" ; # checksum -p LPC4357 -d "MonkeyBookTestApp-Invalid-InBootSpace.bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
