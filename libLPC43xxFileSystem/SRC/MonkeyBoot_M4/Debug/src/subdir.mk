################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/System.c \
../src/cr_startup_lpc43xx.c \
../src/crp.c \
../src/main.c \
../src/sysinit.c 

OBJS += \
./src/System.o \
./src/cr_startup_lpc43xx.o \
./src/crp.o \
./src/main.o \
./src/sysinit.o 

C_DEPS += \
./src/System.d \
./src/cr_startup_lpc43xx.d \
./src/crp.d \
./src/main.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -D__MULTICORE_NONE -DDEBUG -D__CODE_RED -DCORE_M4 -D__USE_LPCOPEN -D__LPC43XX__ -I"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\MonkeyBoot_M4\src\FILE_SYSTEM\fatfs" -I"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\lpc_chip_43xx\inc" -I"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\MonkeyBoot_M4\src" -I"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\lpc_board_keil_mcb_4357\inc" -I"C:\Users\emh203\Dropbox\eLib\libLPC43xxFileSystem\SRC\MonkeyBoot_M4\src\FILE_SYSTEM" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


