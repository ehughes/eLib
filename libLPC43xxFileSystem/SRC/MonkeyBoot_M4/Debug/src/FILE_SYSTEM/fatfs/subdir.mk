################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FILE_SYSTEM/fatfs/ff.c \
../src/FILE_SYSTEM/fatfs/ff_lfn_support.c \
../src/FILE_SYSTEM/fatfs/ff_string.c 

OBJS += \
./src/FILE_SYSTEM/fatfs/ff.o \
./src/FILE_SYSTEM/fatfs/ff_lfn_support.o \
./src/FILE_SYSTEM/fatfs/ff_string.o 

C_DEPS += \
./src/FILE_SYSTEM/fatfs/ff.d \
./src/FILE_SYSTEM/fatfs/ff_lfn_support.d \
./src/FILE_SYSTEM/fatfs/ff_string.d 


# Each subdirectory must supply rules for building sources it contributes
src/FILE_SYSTEM/fatfs/%.o: ../src/FILE_SYSTEM/fatfs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -D__MULTICORE_NONE -DDEBUG -D__CODE_RED -DCORE_M4 -D__USE_LPCOPEN -D__LPC43XX__ -I"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\MonkeyBoot_M4\src\FILE_SYSTEM\fatfs" -I"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\lpc_chip_43xx\inc" -I"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\MonkeyBoot_M4\src" -I"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\lpc_board_keil_mcb_4357\inc" -I"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\MonkeyBoot_M4\src\FILE_SYSTEM" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


