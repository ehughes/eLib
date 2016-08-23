################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/ConfigDescriptor.c \
../src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/Host.c \
../src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/HostStandardReq.c \
../src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/Pipe.c \
../src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/PipeStream.c \
../src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/USBController.c \
../src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/USBMemory.c \
../src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/USBTask.c 

OBJS += \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/ConfigDescriptor.o \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/Host.o \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/HostStandardReq.o \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/Pipe.o \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/PipeStream.o \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/USBController.o \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/USBMemory.o \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/USBTask.o 

C_DEPS += \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/ConfigDescriptor.d \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/Host.d \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/HostStandardReq.d \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/Pipe.d \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/PipeStream.d \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/USBController.d \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/USBMemory.d \
./src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/USBTask.d 


# Each subdirectory must supply rules for building sources it contributes
src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/%.o: ../src/FILE_SYSTEM/fatfs/DISK_DRIVER/USB_MSC_HOST/lpcusblib/Drivers/USB/Core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -D__MULTICORE_NONE -DDEBUG -D__CODE_RED -DCORE_M4 -D__USE_LPCOPEN -D__LPC43XX__ -I"Z:\Dropbox\eLib\MonkeyBoot\SRC\MonkeyBoot_M4\src\FILE_SYSTEM\fatfs" -I"Z:\Dropbox\eLib\MonkeyBoot\SRC\lpc_chip_43xx\inc" -I"Z:\Dropbox\eLib\MonkeyBoot\SRC\MonkeyBoot_M4\src" -I"Z:\Dropbox\eLib\MonkeyBoot\SRC\lpc_board_keil_mcb_4357\inc" -I"Z:\Dropbox\eLib\MonkeyBoot\SRC\MonkeyBoot_M4\src\FILE_SYSTEM" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


