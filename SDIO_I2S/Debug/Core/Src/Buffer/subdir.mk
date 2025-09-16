################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Buffer/Audio_Process.c 

OBJS += \
./Core/Src/Buffer/Audio_Process.o 

C_DEPS += \
./Core/Src/Buffer/Audio_Process.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Buffer/%.o Core/Src/Buffer/%.su: ../Core/Src/Buffer/%.c Core/Src/Buffer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/Admin/Desktop/STM32_Driver/SDIO_I2S/FATFS" -I"C:/Users/Admin/Desktop/STM32_Driver/SDIO_I2S/Core/Src/Buffer" -I"C:/Users/Admin/Desktop/STM32_Driver/SDIO_I2S/Core/Src/Console" -I"C:/Users/Admin/Desktop/STM32_Driver/SDIO_I2S/Core/Src/Driver_Library" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Buffer

clean-Core-2f-Src-2f-Buffer:
	-$(RM) ./Core/Src/Buffer/Audio_Process.d ./Core/Src/Buffer/Audio_Process.o ./Core/Src/Buffer/Audio_Process.su

.PHONY: clean-Core-2f-Src-2f-Buffer

