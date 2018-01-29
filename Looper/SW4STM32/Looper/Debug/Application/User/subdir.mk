################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/SRAMDriver.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/adc.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/ads1256_test.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/audio.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/dma.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/drums.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/fmc.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/gpio.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/main.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/menu.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/midi.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/spi.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/stm32f429i_discovery.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/stm32f429i_discovery_sdram.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/stm32f4xx_hal_msp.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/stm32f4xx_it.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/tim.c \
C:/Users/Tomek/workspace/tom32f4/Looper/Src/usart.c 

OBJS += \
./Application/User/SRAMDriver.o \
./Application/User/adc.o \
./Application/User/ads1256_test.o \
./Application/User/audio.o \
./Application/User/dma.o \
./Application/User/drums.o \
./Application/User/fmc.o \
./Application/User/gpio.o \
./Application/User/main.o \
./Application/User/menu.o \
./Application/User/midi.o \
./Application/User/spi.o \
./Application/User/stm32f429i_discovery.o \
./Application/User/stm32f429i_discovery_sdram.o \
./Application/User/stm32f4xx_hal_msp.o \
./Application/User/stm32f4xx_it.o \
./Application/User/tim.o \
./Application/User/usart.o 

C_DEPS += \
./Application/User/SRAMDriver.d \
./Application/User/adc.d \
./Application/User/ads1256_test.d \
./Application/User/audio.d \
./Application/User/dma.d \
./Application/User/drums.d \
./Application/User/fmc.d \
./Application/User/gpio.d \
./Application/User/main.d \
./Application/User/menu.d \
./Application/User/midi.d \
./Application/User/spi.d \
./Application/User/stm32f429i_discovery.d \
./Application/User/stm32f429i_discovery_sdram.d \
./Application/User/stm32f4xx_hal_msp.d \
./Application/User/stm32f4xx_it.d \
./Application/User/tim.d \
./Application/User/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/SRAMDriver.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/SRAMDriver.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/adc.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/ads1256_test.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/ads1256_test.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/audio.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/audio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/dma.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/dma.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/drums.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/drums.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/fmc.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/fmc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/gpio.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/main.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/menu.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/menu.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/midi.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/midi.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/spi.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f429i_discovery.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/stm32f429i_discovery.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f429i_discovery_sdram.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/stm32f429i_discovery_sdram.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f4xx_hal_msp.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/stm32f4xx_hal_msp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f4xx_it.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/stm32f4xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/tim.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/tim.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/usart.o: C:/Users/Tomek/workspace/tom32f4/Looper/Src/usart.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' -DNO_TEST '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F429xx '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Tomek/workspace/tom32f4/Looper/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


