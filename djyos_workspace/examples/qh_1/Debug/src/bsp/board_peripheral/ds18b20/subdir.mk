################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/djysdk/djysrc/djyos/bsp/board_peripheral/qh_1/ds18b20/ds18b20.c 

OBJS += \
./src/bsp/board_peripheral/ds18b20/ds18b20.o 

C_DEPS += \
./src/bsp/board_peripheral/ds18b20/ds18b20.d 


# Each subdirectory must supply rules for building sources it contributes
src/bsp/board_peripheral/ds18b20/ds18b20.o: D:/djysdk/djysrc/djyos/bsp/board_peripheral/qh_1/ds18b20/ds18b20.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -Ddebug -I"D:/djysdk/djysrc/examples/qh_1/src/user/include" -I"D:/djysdk/djysrc/examples/qh_1/src/user/config" -I"D:\djysdk\\djysrc\djyos\bsp\arch\arm\cortex_m\cortex_m3\include" -I"D:\djysdk\\djysrc\djyos\bsp\cpu_peripheral\stm32\include" -I"D:\djysdk\\djysrc\djyos\bsp\board_peripheral\qh_1\include" -I"D:\djysdk\\djysrc\djyos\djyos\include" -I"D:\djysdk\\djysrc\djyos\djyos\gui\include" -I"D:\djysdk\\djysrc\djyos\djyos\djyfs\include" -I"D:\djysdk\\djysrc\djyos\bsp\cpu_peripheral\std_inc" -I"D:\djysdk\\djysrc\djyos\bsp\vendor_lib/CMSIS/CM3/CoreSupport" -I"D:\djysdk\\djysrc\djyos\bsp\vendor_lib/stm32" -I"D:\djysdk\\djysrc\djyos\bsp\vendor_lib/stm32/STM32F10x_StdPeriph_Driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -std=c99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


