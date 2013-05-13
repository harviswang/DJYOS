################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := cs-rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/user/src/subdir.mk
-include src/user/critical/subdir.mk
-include src/user/config/subdir.mk
-include src/loader/si/subdir.mk
-include src/djyos/wdt/subdir.mk
-include src/djyos/stdin/touch/subdir.mk
-include src/djyos/stdin/keypad/subdir.mk
-include src/djyos/stdin/char_term/subdir.mk
-include src/djyos/stdin/subdir.mk
-include src/djyos/shell/subdir.mk
-include src/djyos/rsc/subdir.mk
-include src/djyos/pipe/subdir.mk
-include src/djyos/msg/subdir.mk
-include src/djyos/memory/subdir.mk
-include src/djyos/lock/subdir.mk
-include src/djyos/int/subdir.mk
-include src/djyos/gui/window/subdir.mk
-include src/djyos/gui/lib/subdir.mk
-include src/djyos/gui/gk_draw/subdir.mk
-include src/djyos/gui/font/subdir.mk
-include src/djyos/gui/encoding/subdir.mk
-include src/djyos/gui/display/subdir.mk
-include src/djyos/driver/subdir.mk
-include src/djyos/djyfs/flashfile/subdir.mk
-include src/djyos/djyfs/subdir.mk
-include src/djyos/core/subdir.mk
-include src/djyos/clock/subdir.mk
-include src/djyos/buffer/subdir.mk
-include src/bsp/vendor_lib/stm32/STM32F10x_StdPeriph_Driver/src/subdir.mk
-include src/bsp/vendor_lib/stm32/subdir.mk
-include src/bsp/vendor_lib/CMSIS/CoreSupport/subdir.mk
-include src/bsp/cpu_peripheral/src/subdir.mk
-include src/bsp/board_peripheral/nandflash/subdir.mk
-include src/bsp/board_peripheral/keyboard/subdir.mk
-include src/bsp/board_peripheral/STMPE811/subdir.mk
-include src/bsp/board_peripheral/LCD_9325/subdir.mk
-include src/bsp/arch/startup/subdir.mk
-include src/bsp/arch/core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
qh_1.hex \

SECONDARY_LIST += \
qh_1.lst \

SECONDARY_SIZE += \
qh_1.siz \


# All Target
all: qh_1.elf secondary-outputs

# Tool invocations
qh_1.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Windows GCC C Linker'
	arm-none-eabi-gcc -T"../config/Debug.lds" -nostartfiles -Xlinker --gc-sections -Wl,-Map,qh_1.map -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o "qh_1.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

qh_1.hex: qh_1.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex qh_1.elf  "qh_1.hex"
	@echo 'Finished building: $@'
	@echo ' '

qh_1.lst: qh_1.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Listing'
	arm-none-eabi-objdump -h -S qh_1.elf > "qh_1.lst"
	@echo 'Finished building: $@'
	@echo ' '

qh_1.siz: qh_1.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Print Size'
	arm-none-eabi-size  --format=berkeley qh_1.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) qh_1.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
