MCU	= atmega32
CC	= avr-gcc
CFLAGS	= -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -funsigned-bitfields -fpack-struct -mcall-prologues -std=gnu99
LDFLAGS	= -Wl,-Map,usart.map
SRC	= bootloader.c firmware.c gateway.c test.c\
	CAN_Node.c\
	CAN_NodeActivity.c\
	bit.c rail.c signal.c switch.c\
	mcp2515.c spi.c eeprom.c uart.c serial.c timer.c sleep.c can_ring.c cmd_queue.c result_queue.c config.c\
	can_processing_new.c can_processing_old.c

OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
AVRDUDE=avrdude

all:	firmware.hex bootloader.hex gateway.hex test.hex firmware.lst bootloader.lst gateway.lst

old:	firmware_old.hex firmware_old.lst

revision.h:	.
	./revision.sh

#######################
# Firmware for CAN node
#######################

firmware_old.out:	firmware.o bit.o mcp2515.o rail.o signal.o spi.o switch.o eeprom.o serial.o timer.o sleep.o can_ring.o cmd_queue.o result_queue.o config.o can_processing_old.o
	$(LINK.c) -Wl,-Map,firmware_old.map -o $@ $^

firmware_old.hex:	firmware_old.out
	$(OBJCOPY) -R .eeprom -O ihex $^ $@

firmware_old.lst:	firmware_old.out
	$(OBJDUMP) -h -S -j .text -j .data $^ >$@

###########################
# New firmware for CAN node
###########################

firmware.out:	firmware.o bit.o mcp2515.o rail.o signal.o spi.o switch.o eeprom.o serial.o timer.o sleep.o can_ring.o cmd_queue.o result_queue.o config.o can_processing_new.o CAN_Node.o CAN_NodeActivity.o
	$(LINK.c) -Wl,-Map,firmware.map -o $@ $^

firmware.hex:	firmware.out
	$(OBJCOPY) -R .eeprom -O ihex $^ $@

firmware.lst:	firmware.out
	$(OBJDUMP) -h -S -j .text -j .data $^ >$@

#####################################
# Firmware for CAN/RS232 gateway node
#####################################

gateway.out:	gateway.o mcp2515.o spi.o uart.o sleep.o can_ring.o
	$(LINK.c) -Wl,-Map,gateway.map -o $@ $^

gateway.hex:	gateway.out
	$(OBJCOPY) -R .eeprom -O ihex $^ $@

gateway.lst:	gateway.out
	$(OBJDUMP) -h -S -j .text -j .data $^ >$@

###############
# Test firmware
###############

test.out:	test.o mcp2515.o spi.o bit.o timer.o sleep.o eeprom.o
	$(LINK.c) -Wl,-Map,test.map -o $@ $^

test.hex:	test.out
	$(OBJCOPY) -R .eeprom -O ihex $^ $@

#########################
# Bootloader for CAN node
#########################

bootloader.out:	bootloader.o mcp2515.o spi.o eeprom.o
	$(LINK.c) -Wl,-Map,bootloader.map -Wl,--section-start=.text=0x7000 -o $@ $^

bootloader.lst:	bootloader.out
	$(OBJDUMP) -h -S -j .text -j .data $^ >$@

bootloader.hex:	bootloader.out
	$(OBJCOPY) -R .eeprom -O ihex $^ $@

######################
# AVR promming targets
######################

install_gateway:	gateway.hex
	$(AVRDUDE) -p m32 -B 1 -U flash:w:$^ -U hfuse:w:0x09:m -U lfuse:w:0x1e:m

install_bootloader:	bootloader.hex
	$(AVRDUDE) -p m32 -B 1 -U flash:w:$^ -U hfuse:w:0xc0:m -U lfuse:w:0x20:m -U lock:w:0x2f:m

install_bootloader_jtag:	bootloader.hex
	$(AVRDUDE) -p m32 -B 1 -U flash:w:$^ -U hfuse:w:0x0:m -U lfuse:w:0x20:m -U lock:w:0x2f:m

verify_firmware:	firmware.hex
	$(AVRDUDE) -p m32 -B 1 -U flash:v:$^

read_fuse:
	$(AVRDUDE) -p m32 -B 1 -U hfuse:r:-:h -U lfuse:r:-:h

read_lock:
	$(AVRDUDE) -p m32 -B 1 -U lock:r:-:h

#####################
# some global targets
#####################

depend:	revision.h
	$(CPP) -M $(SRC) $(CFLAGS) >.dep

clean:
	rm -rf *.o *.map *.hex *.out *.lst .dep revision.h

-include .dep
