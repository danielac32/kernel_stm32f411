
FLASH       ?= st-flash
TOOLSET     ?= arm-none-eabi-
CC           = $(TOOLSET)gcc
LD           = $(TOOLSET)gcc
AR           = $(TOOLSET)gcc-ar
OBJCOPY      = $(TOOLSET)objcopy
OPTFLAGS    ?= -Og
STRIP=arm-none-eabi-strip


RM = rm -f
fixpath = $(strip $1)

 
CFLAGS      ?=-O1 -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffreestanding -nostartfiles  -fno-builtin
LDFLAGS      = -ffreestanding -nostartfiles  -fno-builtin -I include  
INCLUDES     =   -I include -I stm32lib  -I sd-spi/Inc -I gpio/Inc -I fat32/Inc -I spi/Inc -I w25q/Inc -I usb/Inc -I usb/class -I tinyscript
CFLAGS2     ?= $(CFLAGS) -mthumb $(OPTFLAGS)
LDSCRIPT     =  ld.script

 
OBJDIR       = obj
SOURCES      =   $(wildcard system/*.c) $(wildcard system/*.S)
OBJECTS      = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SOURCES)))))
SRCLIB         = $(wildcard lib/*.c)
LIBOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCLIB)))))
SRCSHELL         = $(wildcard shell/*.c)
SHELLOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCSHELL)))))
SRCDEVICE         = $(wildcard device/nam/*.c) $(wildcard device/tty/*.c) $(wildcard device/led/*.c) $(wildcard device/flash/*.c)
DEVICEOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCDEVICE)))))

SRCGPIO         = $(wildcard gpio/Src/*.c)
GPIOOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCGPIO)))))

SRCUSB         = $(wildcard usb/Src/*.c) $(wildcard usb/msc/*.c) $(wildcard usb/cdc/*.c)
USBOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCUSB)))))

SRCSPI         = $(wildcard spi/Src/*.c) $(wildcard sd-spi/Src/*.c)
SPIOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCSPI)))))


SRCW25Q         = $(wildcard w25q/Src/*.c) 
W25QOBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCW25Q)))))

SRCFAT32         = $(wildcard fat32/Src/*.c)  $(wildcard tinyscript/*.c) $(wildcard cc/*.c) #$(wildcard basic/*.c) 
FAT32OBJ         = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRCFAT32)))))



DOUT         = kernel


SRCPATH = $(sort $(dir $(SOURCES) $(SRCLIB) $(SRCSHELL) $(SRCDEVICE) $(SRCSPI) $(SRCFAT32) $(SRCW25Q) $(SRCGPIO) $(SRCUSB) ))
vpath %.c $(SRCPATH)
vpath %.S $(SRCPATH)


 
$(OBJDIR):
	@mkdir $@

flash:
	#st-flash --reset --format ihex write kernel.hex
	st-flash write $(DOUT).bin 0x08010000
	st-flash reset
reset:
	st-flash reset


demo: $(DOUT).bin
		arm-none-eabi-objdump -d kernel.elf > kernel.list


	
$(DOUT).bin : $(DOUT).elf
	@echo building $@
	@$(OBJCOPY) -O binary $< $@



$(DOUT).elf : $(OBJDIR) $(LIBOBJ) $(SHELLOBJ) $(OBJECTS) $(DEVICEOBJ) $(GPIOOBJ) $(USBOBJ) $(SPIOBJ) $(FAT32OBJ) $(W25QOBJ)
	@echo building $@
	@$(LD) $(CFLAGS2) $(LDFLAGS) -Wl,--script='$(LDSCRIPT)' $(LIBOBJ) $(SHELLOBJ) $(DEVICEOBJ) $(OBJECTS) $(GPIOOBJ) $(USBOBJ) $(SPIOBJ) $(FAT32OBJ) $(W25QOBJ)  -o $@

clean: $(OBJDIR)
	$(MAKE) --version
	@$(RM) $(DOUT).*
	@$(RM) $(call fixpath, $(OBJDIR)/*.*)


$(OBJDIR)/%.o: %.S
	@echo assembling $<
	@$(CC) $(CFLAGS2)  $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@echo compiling $<
	@$(CC) $(CFLAGS2)  $(INCLUDES) -c $< -o $@



blink:
	clear
	arm-none-eabi-gcc -O0 -c -o app/blink/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fPIE -pie -static
	arm-none-eabi-gcc -O0 -c -o app/blink/main.o   $(CFLAGS) -mthumb $(INCLUDES)  app/blink/main.c -fPIE -pie -static
	arm-none-eabi-gcc -O0 -c -o app/blink/syscall.o   $(CFLAGS) -mthumb $(INCLUDES)  app/lib/syscall.c -fPIE -pie -static
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb $(INCLUDES)  app/lib/string.c -fPIE -pie -static
	arm-none-eabi-gcc -O0 -c -o app/blink/doprnt.o   $(CFLAGS) -mthumb $(INCLUDES)  app/lib/doprnt.c -fPIE -pie -static
	arm-none-eabi-gcc -O0 -c -o app/blink/printf.o   $(CFLAGS) -mthumb $(INCLUDES)  app/lib/printf.c -fpie
	arm-none-eabi-ld app/blink/entry.o app/blink/main.o app/blink/syscall.o app/blink/doprnt.o app/blink/printf.o $(INCLUDES)  -nostartfiles -nostdlib -T app/app_base.ld  -o app/blink/blink.elf 


echo:
	clear
	arm-none-eabi-gcc -O0 -c -o app/echo/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/echo/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/echo/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/echo/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/echo/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/echo/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/echo/entry.o app/echo/main.o app/blink/doprnt.o app/blink/printf.o app/echo/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/echo/echo.elf 
	arm-none-eabi-objdump -d app/echo/echo.elf


cat:
	clear
	arm-none-eabi-gcc -O0 -c -o app/cat/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/cat/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/cat/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/cat/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/cat/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/cat/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/cat/entry.o app/cat/main.o app/blink/string.o app/cat/doprnt.o app/cat/printf.o app/cat/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/cat/cat.elf 
	arm-none-eabi-objdump -d app/cat/cat.elf

cd:
	clear
	arm-none-eabi-gcc -O0 -c -o app/cd/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/cd/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/cd/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/cd/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/cd/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/cd/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/cd/entry.o app/cd/main.o app/blink/string.o app/cd/doprnt.o app/cd/printf.o app/cd/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/cd/cd.elf 
	arm-none-eabi-objdump -d app/cd/cd.elf


dump:
	clear
	arm-none-eabi-gcc -O0 -c -o app/dump/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/dump/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/dump/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/dump/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/dump/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/dump/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/dump/entry.o app/dump/main.o app/blink/string.o app/dump/doprnt.o app/dump/printf.o app/dump/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/dump/dump.elf 
	arm-none-eabi-objdump -d app/dump/dump.elf

help:
	clear
	arm-none-eabi-gcc -O0 -c -o app/help/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/help/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/help/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/help/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/help/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/help/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/help/entry.o app/help/main.o app/blink/string.o app/help/doprnt.o app/help/printf.o app/help/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/help/help.elf 
	arm-none-eabi-objdump -d app/help/help.elf


kill:
	clear
	arm-none-eabi-gcc -O0 -c -o app/kill/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/kill/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/kill/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/kill/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/kill/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/kill/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/kill/entry.o app/kill/main.o app/blink/string.o app/kill/doprnt.o app/kill/printf.o app/kill/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/kill/kill.elf 
	arm-none-eabi-objdump -d app/kill/kill.elf


loader:
	clear
	arm-none-eabi-gcc -O0 -c -o app/loader/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/loader/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/loader/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/loader/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/loader/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/loader/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/loader/entry.o app/loader/main.o app/blink/string.o app/loader/doprnt.o app/loader/printf.o app/loader/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/loader/loader.elf 
	arm-none-eabi-objdump -d app/loader/loader.elf


ls:
	clear
	arm-none-eabi-gcc -O0 -c -o app/ls/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/ls/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/ls/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/ls/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/ls/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/ls/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/ls/entry.o app/ls/main.o app/blink/string.o app/ls/doprnt.o app/ls/printf.o app/ls/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/ls/ls.elf 
	arm-none-eabi-objdump -d app/ls/ls.elf

mem:
	clear
	arm-none-eabi-gcc -O0 -c -o app/mem/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/mem/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/mem/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/mem/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/mem/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/mem/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/mem/entry.o app/mem/main.o app/blink/string.o app/mem/doprnt.o app/mem/printf.o app/mem/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/mem/mem.elf 
	arm-none-eabi-objdump -d app/mem/mem.elf

mkdir:
	clear
	arm-none-eabi-gcc -O0 -c -o app/mkdir/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/mkdir/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/mkdir/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/mkdir/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/mkdir/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/mkdir/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/mkdir/entry.o app/mkdir/main.o app/blink/string.o app/mkdir/doprnt.o app/mkdir/printf.o app/mkdir/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/mkdir/mkdir.elf 
	arm-none-eabi-objdump -d app/mkdir/mkdir.elf


ps:
	clear
	arm-none-eabi-gcc -O0 -c -o app/ps/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/ps/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/ps/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/ps/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/ps/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/ps/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/ps/entry.o app/ps/main.o app/blink/string.o app/ps/doprnt.o app/ps/printf.o app/ps/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/ps/ps.elf 
	arm-none-eabi-objdump -d app/ps/ps.elf

pwd:
	clear
	arm-none-eabi-gcc -O0 -c -o app/pwd/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/pwd/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/pwd/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/pwd/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/pwd/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/pwd/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/pwd/entry.o app/pwd/main.o app/blink/string.o app/pwd/doprnt.o app/pwd/printf.o app/pwd/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/pwd/pwd.elf 
	arm-none-eabi-objdump -d app/pwd/pwd.elf


rm:
	clear
	arm-none-eabi-gcc -O0 -c -o app/rm/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/rm/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/rm/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/blink/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/rm/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/rm/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/rm/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/rm/entry.o app/rm/main.o app/blink/string.o app/rm/doprnt.o app/rm/printf.o app/rm/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/rm/rm.elf 
	arm-none-eabi-objdump -d app/rm/rm.elf


touch:
	clear
	arm-none-eabi-gcc -O0 -c -o app/touch/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/touch/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/touch/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/touch/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/touch/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/touch/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/touch/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/touch/entry.o app/touch/main.o app/touch/string.o app/touch/doprnt.o app/touch/printf.o app/touch/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/touch/touch.elf 
	arm-none-eabi-objdump -d app/touch/touch.elf


shell:
	clear
	arm-none-eabi-gcc -O0 -c -o app/shell/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/shell/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/shell/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/shell/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/shell/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/shell/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/shell/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/shell/entry.o app/shell/main.o app/shell/string.o app/shell/doprnt.o app/shell/printf.o app/shell/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/shell/shell.elf 
	arm-none-eabi-objdump -d app/shell/shell.elf


rvvm:
	clear
	arm-none-eabi-gcc -O0 -c -o app/rvvm/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/rvvm/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/rvvm/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/rvvm/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/rvvm/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/rvvm/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/rvvm/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/rvvm/entry.o app/rvvm/main.o app/rvvm/string.o app/rvvm/doprnt.o app/rvvm/printf.o app/rvvm/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/rvvm/rvvm.elf 
	arm-none-eabi-objdump -d app/rvvm/rvvm.elf

mark1:
	clear
	arm-none-eabi-gcc -O0 -c -o app/mark1/entry.o  $(CFLAGS) -mthumb  app/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o app/mark1/main.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/mark1/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/mark1/string.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/string.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/mark1/syscall.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/syscall.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/mark1/doprnt.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/doprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o app/mark1/printf.o   $(CFLAGS) -mthumb -I include  -I stm32lib  app/lib/printf.c -fpie
	arm-none-eabi-ld app/mark1/entry.o app/mark1/main.o app/mark1/string.o app/mark1/doprnt.o app/mark1/printf.o app/mark1/syscall.o -I include  -I stm32lib  -nostartfiles -nostdlib -T app/app_base.ld  -o app/mark1/mark1.elf 
	arm-none-eabi-objdump -d app/mark1/mark1.elf


apps:
	make cd
	make rm
	make cat
	make dump
	make echo
	make help
	make kill
	make ls
	make mkdir
	make ps
	make shell
	make touch
	make loader
	make mem

	