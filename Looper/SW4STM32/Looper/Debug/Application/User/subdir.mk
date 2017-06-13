################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/adc.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/audio.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/bsp_driver_sd.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/dac.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/dma.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/drums.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/fatfs.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/fmc.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/gpio.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/keyboard.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/main.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/sdio.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/spi.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/stm32f429i_discovery.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/stm32f429i_discovery_sdram.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/stm32f4xx_hal_msp.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/stm32f4xx_it.c \
C:/Users/Tomasz/workspace/tom32f4/Looper/Src/tim.c 

OBJS += \
./Application/User/adc.o \
./Application/User/audio.o \
./Application/User/bsp_driver_sd.o \
./Application/User/dac.o \
./Application/User/dma.o \
./Application/User/drums.o \
./Application/User/fatfs.o \
./Application/User/fmc.o \
./Application/User/gpio.o \
./Application/User/keyboard.o \
./Application/User/main.o \
./Application/User/sdio.o \
./Application/User/spi.o \
./Application/User/stm32f429i_discovery.o \
./Application/User/stm32f429i_discovery_sdram.o \
./Application/User/stm32f4xx_hal_msp.o \
./Application/User/stm32f4xx_it.o \
./Application/User/tim.o 

C_DEPS += \
./Application/User/adc.d \
./Application/User/audio.d \
./Application/User/bsp_driver_sd.d \
./Application/User/dac.d \
./Application/User/dma.d \
./Application/User/drums.d \
./Application/User/fatfs.d \
./Application/User/fmc.d \
./Application/User/gpio.d \
./Application/User/keyboard.d \
./Application/User/main.d \
./Application/User/sdio.d \
./Application/User/spi.d \
./Application/User/stm32f429i_discovery.d \
./Application/User/stm32f429i_discovery_sdram.d \
./Application/User/stm32f4xx_hal_msp.d \
./Application/User/stm32f4xx_it.d \
./Application/User/tim.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/adc.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/audio.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/audio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/bsp_driver_sd.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/bsp_driver_sd.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/dac.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/dac.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/dma.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/dma.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/drums.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/drums.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/fatfs.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/fatfs.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/fmc.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/fmc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/gpio.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/keyboard.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/keyboard.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/main.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/sdio.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/sdio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/spi.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f429i_discovery.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/stm32f429i_discovery.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f429i_discovery_sdram.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/stm32f429i_discovery_sdram.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f4xx_hal_msp.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/stm32f4xx_hal_msp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f4xx_it.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/stm32f4xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/tim.o: C:/Users/Tomasz/workspace/tom32f4/Looper/Src/tim.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/Tomasz/workspace/tom32f4/Looper/Middlewares/Third_Party/FatFs/src"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


