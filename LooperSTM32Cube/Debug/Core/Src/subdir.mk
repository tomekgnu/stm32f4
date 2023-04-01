################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ILI9341_GFX.c \
../Core/Src/ILI9341_STM32_Driver.c \
../Core/Src/SRAMDriver.c \
../Core/Src/adc.c \
../Core/Src/ads1256.c \
../Core/Src/audio.c \
../Core/Src/dac.c \
../Core/Src/diskio.c \
../Core/Src/dma.c \
../Core/Src/drums.c \
../Core/Src/ff.c \
../Core/Src/fmc.c \
../Core/Src/fonts.c \
../Core/Src/gpio.c \
../Core/Src/joystick.c \
../Core/Src/main.c \
../Core/Src/memops.c \
../Core/Src/menu.c \
../Core/Src/menu_callback.c \
../Core/Src/menu_strings.c \
../Core/Src/midi.c \
../Core/Src/spi.c \
../Core/Src/spi_flash.c \
../Core/Src/spiffs_cache.c \
../Core/Src/spiffs_check.c \
../Core/Src/spiffs_gc.c \
../Core/Src/spiffs_hydrogen.c \
../Core/Src/spiffs_nucleus.c \
../Core/Src/stm32f429i_discovery.c \
../Core/Src/stm32f429i_discovery_sdram.c \
../Core/Src/stm32f4xx_hal_i2c.c \
../Core/Src/stm32f4xx_hal_i2c_ex.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/tm_stm32_hd44780.c \
../Core/Src/tm_stm32f4_fonts.c \
../Core/Src/tm_stm32f4_ili9341.c \
../Core/Src/tm_stm32f4_keypad.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/ILI9341_GFX.o \
./Core/Src/ILI9341_STM32_Driver.o \
./Core/Src/SRAMDriver.o \
./Core/Src/adc.o \
./Core/Src/ads1256.o \
./Core/Src/audio.o \
./Core/Src/dac.o \
./Core/Src/diskio.o \
./Core/Src/dma.o \
./Core/Src/drums.o \
./Core/Src/ff.o \
./Core/Src/fmc.o \
./Core/Src/fonts.o \
./Core/Src/gpio.o \
./Core/Src/joystick.o \
./Core/Src/main.o \
./Core/Src/memops.o \
./Core/Src/menu.o \
./Core/Src/menu_callback.o \
./Core/Src/menu_strings.o \
./Core/Src/midi.o \
./Core/Src/spi.o \
./Core/Src/spi_flash.o \
./Core/Src/spiffs_cache.o \
./Core/Src/spiffs_check.o \
./Core/Src/spiffs_gc.o \
./Core/Src/spiffs_hydrogen.o \
./Core/Src/spiffs_nucleus.o \
./Core/Src/stm32f429i_discovery.o \
./Core/Src/stm32f429i_discovery_sdram.o \
./Core/Src/stm32f4xx_hal_i2c.o \
./Core/Src/stm32f4xx_hal_i2c_ex.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/tm_stm32_hd44780.o \
./Core/Src/tm_stm32f4_fonts.o \
./Core/Src/tm_stm32f4_ili9341.o \
./Core/Src/tm_stm32f4_keypad.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/ILI9341_GFX.d \
./Core/Src/ILI9341_STM32_Driver.d \
./Core/Src/SRAMDriver.d \
./Core/Src/adc.d \
./Core/Src/ads1256.d \
./Core/Src/audio.d \
./Core/Src/dac.d \
./Core/Src/diskio.d \
./Core/Src/dma.d \
./Core/Src/drums.d \
./Core/Src/ff.d \
./Core/Src/fmc.d \
./Core/Src/fonts.d \
./Core/Src/gpio.d \
./Core/Src/joystick.d \
./Core/Src/main.d \
./Core/Src/memops.d \
./Core/Src/menu.d \
./Core/Src/menu_callback.d \
./Core/Src/menu_strings.d \
./Core/Src/midi.d \
./Core/Src/spi.d \
./Core/Src/spi_flash.d \
./Core/Src/spiffs_cache.d \
./Core/Src/spiffs_check.d \
./Core/Src/spiffs_gc.d \
./Core/Src/spiffs_hydrogen.d \
./Core/Src/spiffs_nucleus.d \
./Core/Src/stm32f429i_discovery.d \
./Core/Src/stm32f429i_discovery_sdram.d \
./Core/Src/stm32f4xx_hal_i2c.d \
./Core/Src/stm32f4xx_hal_i2c_ex.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/tm_stm32_hd44780.d \
./Core/Src/tm_stm32f4_fonts.d \
./Core/Src/tm_stm32f4_ili9341.d \
./Core/Src/tm_stm32f4_keypad.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -DNO_TEST -c -I../Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ILI9341_GFX.cyclo ./Core/Src/ILI9341_GFX.d ./Core/Src/ILI9341_GFX.o ./Core/Src/ILI9341_GFX.su ./Core/Src/ILI9341_STM32_Driver.cyclo ./Core/Src/ILI9341_STM32_Driver.d ./Core/Src/ILI9341_STM32_Driver.o ./Core/Src/ILI9341_STM32_Driver.su ./Core/Src/SRAMDriver.cyclo ./Core/Src/SRAMDriver.d ./Core/Src/SRAMDriver.o ./Core/Src/SRAMDriver.su ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/ads1256.cyclo ./Core/Src/ads1256.d ./Core/Src/ads1256.o ./Core/Src/ads1256.su ./Core/Src/audio.cyclo ./Core/Src/audio.d ./Core/Src/audio.o ./Core/Src/audio.su ./Core/Src/dac.cyclo ./Core/Src/dac.d ./Core/Src/dac.o ./Core/Src/dac.su ./Core/Src/diskio.cyclo ./Core/Src/diskio.d ./Core/Src/diskio.o ./Core/Src/diskio.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/drums.cyclo ./Core/Src/drums.d ./Core/Src/drums.o ./Core/Src/drums.su ./Core/Src/ff.cyclo ./Core/Src/ff.d ./Core/Src/ff.o ./Core/Src/ff.su ./Core/Src/fmc.cyclo ./Core/Src/fmc.d ./Core/Src/fmc.o ./Core/Src/fmc.su ./Core/Src/fonts.cyclo ./Core/Src/fonts.d ./Core/Src/fonts.o ./Core/Src/fonts.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/joystick.cyclo ./Core/Src/joystick.d ./Core/Src/joystick.o ./Core/Src/joystick.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/memops.cyclo ./Core/Src/memops.d ./Core/Src/memops.o ./Core/Src/memops.su ./Core/Src/menu.cyclo ./Core/Src/menu.d ./Core/Src/menu.o ./Core/Src/menu.su ./Core/Src/menu_callback.cyclo ./Core/Src/menu_callback.d ./Core/Src/menu_callback.o ./Core/Src/menu_callback.su ./Core/Src/menu_strings.cyclo ./Core/Src/menu_strings.d ./Core/Src/menu_strings.o ./Core/Src/menu_strings.su ./Core/Src/midi.cyclo ./Core/Src/midi.d ./Core/Src/midi.o ./Core/Src/midi.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/spi_flash.cyclo ./Core/Src/spi_flash.d ./Core/Src/spi_flash.o ./Core/Src/spi_flash.su ./Core/Src/spiffs_cache.cyclo ./Core/Src/spiffs_cache.d ./Core/Src/spiffs_cache.o ./Core/Src/spiffs_cache.su ./Core/Src/spiffs_check.cyclo ./Core/Src/spiffs_check.d ./Core/Src/spiffs_check.o ./Core/Src/spiffs_check.su ./Core/Src/spiffs_gc.cyclo ./Core/Src/spiffs_gc.d ./Core/Src/spiffs_gc.o ./Core/Src/spiffs_gc.su ./Core/Src/spiffs_hydrogen.cyclo ./Core/Src/spiffs_hydrogen.d ./Core/Src/spiffs_hydrogen.o ./Core/Src/spiffs_hydrogen.su ./Core/Src/spiffs_nucleus.cyclo ./Core/Src/spiffs_nucleus.d ./Core/Src/spiffs_nucleus.o ./Core/Src/spiffs_nucleus.su ./Core/Src/stm32f429i_discovery.cyclo ./Core/Src/stm32f429i_discovery.d ./Core/Src/stm32f429i_discovery.o ./Core/Src/stm32f429i_discovery.su ./Core/Src/stm32f429i_discovery_sdram.cyclo ./Core/Src/stm32f429i_discovery_sdram.d ./Core/Src/stm32f429i_discovery_sdram.o ./Core/Src/stm32f429i_discovery_sdram.su ./Core/Src/stm32f4xx_hal_i2c.cyclo ./Core/Src/stm32f4xx_hal_i2c.d ./Core/Src/stm32f4xx_hal_i2c.o ./Core/Src/stm32f4xx_hal_i2c.su ./Core/Src/stm32f4xx_hal_i2c_ex.cyclo ./Core/Src/stm32f4xx_hal_i2c_ex.d ./Core/Src/stm32f4xx_hal_i2c_ex.o ./Core/Src/stm32f4xx_hal_i2c_ex.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/tm_stm32_hd44780.cyclo ./Core/Src/tm_stm32_hd44780.d ./Core/Src/tm_stm32_hd44780.o ./Core/Src/tm_stm32_hd44780.su ./Core/Src/tm_stm32f4_fonts.cyclo ./Core/Src/tm_stm32f4_fonts.d ./Core/Src/tm_stm32f4_fonts.o ./Core/Src/tm_stm32f4_fonts.su ./Core/Src/tm_stm32f4_ili9341.cyclo ./Core/Src/tm_stm32f4_ili9341.d ./Core/Src/tm_stm32f4_ili9341.o ./Core/Src/tm_stm32f4_ili9341.su ./Core/Src/tm_stm32f4_keypad.cyclo ./Core/Src/tm_stm32f4_keypad.d ./Core/Src/tm_stm32f4_keypad.o ./Core/Src/tm_stm32f4_keypad.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

