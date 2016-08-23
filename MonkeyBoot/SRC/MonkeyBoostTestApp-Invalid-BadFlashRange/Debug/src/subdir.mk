################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MonkeyBootTestApp-InvalidMemoryRange.c \
../src/cr_startup_lpc43xx.c \
../src/crp.c 

OBJS += \
./src/MonkeyBootTestApp-InvalidMemoryRange.o \
./src/cr_startup_lpc43xx.o \
./src/crp.o 

C_DEPS += \
./src/MonkeyBootTestApp-InvalidMemoryRange.d \
./src/cr_startup_lpc43xx.d \
./src/crp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__MULTICORE_NONE -DDEBUG -D__CODE_RED -DCORE_M4 -D__LPC43XX__ -D__REDLIB__ -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


