################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Buffer/CAN_Process.c 

OBJS += \
./Core/Src/Buffer/CAN_Process.o 

C_DEPS += \
./Core/Src/Buffer/CAN_Process.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Buffer/%.o Core/Src/Buffer/%.su: ../Core/Src/Buffer/%.c Core/Src/Buffer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F105xC -c -I../Core/Inc -I"C:/Users/Admin/Desktop/STM32_Driver/CAN_F1/Core/Src/Buffer" -I"C:/Users/Admin/Desktop/STM32_Driver/CAN_F1/Core/Src/Console" -I"C:/Users/Admin/Desktop/STM32_Driver/CAN_F1/Core/Src/Driver_Library" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Buffer

clean-Core-2f-Src-2f-Buffer:
	-$(RM) ./Core/Src/Buffer/CAN_Process.d ./Core/Src/Buffer/CAN_Process.o ./Core/Src/Buffer/CAN_Process.su

.PHONY: clean-Core-2f-Src-2f-Buffer

