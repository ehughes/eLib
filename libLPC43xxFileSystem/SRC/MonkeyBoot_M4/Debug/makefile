################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/HCD/EHCI/subdir.mk
-include src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/HCD/subdir.mk
-include src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/HAL/LPC18XX/subdir.mk
-include src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/subdir.mk
-include src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Class/Host/subdir.mk
-include src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Class/Common/subdir.mk
-include src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/subdir.mk
-include src/FILE_SYSTEM/fatfs/DISK_DRIVER/SD/subdir.mk
-include src/FILE_SYSTEM/fatfs/DISK_DRIVER/subdir.mk
-include src/FILE_SYSTEM/fatfs/subdir.mk
-include src/FILE_SYSTEM/USB_MSC_ROM/subdir.mk
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
all: libLPC43xxFileSystem.axf

# Tool invocations
libLPC43xxFileSystem.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\lpc_board_keil_mcb_4357\Debug" -L"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\lpc_chip_43xx\Debug" -Xlinker -Map="libLPC43xxFileSystem.map" -Xlinker --gc-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -T "libLPC43xxFileSystem_Debug.ld" -o "libLPC43xxFileSystem.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) libLPC43xxFileSystem.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "libLPC43xxFileSystem.axf"; # arm-none-eabi-objcopy -v -O binary "libLPC43xxFileSystem.axf" "libLPC43xxFileSystem.bin" ; # checksum -p LPC4357 -d "libLPC43xxFileSystem.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
