## HOW TO## 
# To build and flash any part of the lab:
# 	make part1
# 	make part2
# 	make part3
# 	make part4
# To figure what the USB device is called:
# 	1. Unplug the Bumbleboi
# 	2. make base_usb
# 	3. Plug in the Bumbleboi
# 	4. make find_usb
# 	5. Set the *your_platform*_DEVICE variable in this file
# To handin:
# 	(Do this on the server)
# 	make handin

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
BOARD_LIB = $(SELF_DIR)/library
SOURCE_LIB = $(wildcard $(BOARD_LIB)/*.c)
CLOCK_RATE = 16000000L

CC = avr-gcc
CFLAGS = -Wall -O2 -pipe -std=c99 -pedantic
BFLAGS = -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -mmcu=atmega645a $(CFLAGS)

MAC_DEVICE = /dev/tty.usbmodem14201
LINUX_DEVICE = /dev/ttyACM0
UNAME := $(shell uname -s)
LINUX = Linux

HANDIN_FILES = *.c *.h Makefile

sample: sample.c $(wildcard $(BOARD_LIB)/*.c)
	$(CC) $(BFLAGS) -o main.elf sample.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

test: test.c $(wildcard $(BOARD_LIB)/*.c)
	$(CC) $(BFLAGS) -o main.elf test.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

lib: $(wildcard $(BOARD_LIB)/*.c)
	$(foreach var,$(SOURCE_LIB), \
        $(CC) $(BFLAGS) -c -o $(var:.c=.o) $(var); )

program_windows: 
	avrdude -F -pm645 -Pcom4 -carduino -u -U flash:w:main.hex

#edit the device to match the device on your computer
# TODO: @Jared Add you Mac device here
program_mac: main.elf
	avrdude -pm645 -P $(MAC_DEVICE) -c arduino -F -u -U flash:w:main.hex
program_linux: main.elf
	avrdude -pm645 -P $(LINUX_DEVICE) -c arduino -F -u -U flash:w:main.hex

clean:
	rm -fr *.elf *.hex *.o
	rm -f dev.txt net_test net_test.txt

# New stuff

part1: lab3_part1.c $(wildcard $(BOARD_LIB)/*.c)
	$(CC) $(BFLAGS) -o main.elf lab3_part1.c $(wildcard $(BOARD_LIB)/*.c)
	make prep

part2: lab3_part2.c $(wildcard $(BOARD_LIB)/*.c)
	$(CC) $(BFLAGS) -o main.elf lab3_part2.c $(wildcard $(BOARD_LIB)/*.c)
	make prep


part3: lab3_part3.c $(wildcard $(BOARD_LIB)/*.c)
	$(CC) $(BFLAGS) -o main.elf lab3_part3.c $(wildcard $(BOARD_LIB)/*.c)
	make prep

part4: lab3_part4.c $(wildcard $(BOARD_LIB)/*.c)
	$(CC) $(BFLAGS) -o main.elf lab3_part4.c $(wildcard $(BOARD_LIB)/*.c)
	make prep

test_hardware: test_hardware.c $(wildcard $(BOARD_LIB)/*.c)
	$(CC) $(BFLAGS) -O2 -o main.elf test_hardware.c $(wildcard $(BOARD_LIB)/*.c)
	make prep

net: clean
	gcc $(CFLAGS) -g -o net_test net_test.c -lm

run_net: net
	time ./net_test

pipe_net: net
	time ./net_test > net_test.txt

prep:
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf
	make program

# Select platform for programming
ifeq ($(UNAME),Linux)
program: program_linux
else
program: program_mac
endif

# Detect USB device
# Pipe all devices to file
base_usb:
	ls /dev > dev.txt
	
# Compare devices to file
find_usb:
	ls /dev | diff - dev.txt

valgrind: net
	valgrind --leak-check=full ./net_test -v

# Handin for use on the Unix servers
handin: $(HANDIN_FILES)
	handin jseng CPE416_lab3 $(HANDIN_FILES)
	handin jseng CPE416_lab3
