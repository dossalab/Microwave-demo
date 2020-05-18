CC := arm-none-eabi-gcc
AS := arm-none-eabi-gcc
LD := arm-none-eabi-gcc
GDB := arm-none-eabi-gdb
SIZE := arm-none-eabi-size

# Project settings
exec := zygote.elf
arch := -mcpu=cortex-m3 -mthumb
spec := -specs=nosys.specs -specs=nano.specs -nostartfiles
opt := -ggdb3 -Os
misc := -Wall -Wno-main -ffunction-sections -fdata-sections
libs := -lc
link := -Tmcu/stm32f103.ld -Wl,--gc-sections

# Source files
objects += $(addprefix core/, main.o tasks.o ui.o ui_ll.o \
	   	board.o beep.o microwave.o)
objects += $(addprefix mcu/, cortex.o rcc.o gpio.o spi.o timer.o)
objects += $(addprefix drivers/, st7789.o)
objects += $(addprefix misc/, delay.o)
objects += $(addprefix res/, roboto_22.o roboto_50_digits.o)

include Makefile.lvgl

# Header directories
inc += -I. -Ivendor

include Makefile.arm-generic

