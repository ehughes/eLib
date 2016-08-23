################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/HAL/LPC18XX/HAL_LPC18xx.c 

OBJS += \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/HAL/LPC18XX/HAL_LPC18xx.o 

C_DEPS += \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/HAL/LPC18XX/HAL_LPC18xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/HAL/LPC18XX/%.o: ../src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/HAL/LPC18XX/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -D__MULTICORE_NONE -DDEBUG -D__CODE_RED -DCORE_M4 -D__USE_LPCOPEN -D__LPC43XX__ -I"Z:\Dropbox\eLib\MonkeyBoot\SRC\MonkeyBoot_M4\src\FILE_SYSTEM\fatfs" -I"Z:\Dropbox\eLib\MonkeyBoot\SRC\lpc_chip_43xx\inc" -I"Z:\Dropbox\eLib\MonkeyBoot\SRC\MonkeyBoot_M4\src" -I"Z:\Dropbox\eLib\MonkeyBoot\SRC\lpc_board_keil_mcb_4357\inc" -I"Z:\Dropbox\eLib\MonkeyBoot\SRC\MonkeyBoot_M4\src\FILE_SYSTEM" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


