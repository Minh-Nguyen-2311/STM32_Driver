################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Console/ADC_Console.c \
../Core/Src/Console/OOP.c 

OBJS += \
./Core/Src/Console/ADC_Console.o \
./Core/Src/Console/OOP.o 

C_DEPS += \
./Core/Src/Console/ADC_Console.d \
./Core/Src/Console/OOP.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Console/%.o Core/Src/Console/%.su: ../Core/Src/Console/%.c Core/Src/Console/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/Admin/Desktop/STM32_Driver/ADC/Core/Src/Buffer" -I"C:/Users/Admin/Desktop/STM32_Driver/ADC/Core/Src/Console" -I"C:/Users/Admin/Desktop/STM32_Driver/ADC/Core/Src/Driver_Library" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Console

clean-Core-2f-Src-2f-Console:
	-$(RM) ./Core/Src/Console/ADC_Console.d ./Core/Src/Console/ADC_Console.o ./Core/Src/Console/ADC_Console.su ./Core/Src/Console/OOP.d ./Core/Src/Console/OOP.o ./Core/Src/Console/OOP.su

.PHONY: clean-Core-2f-Src-2f-Console

