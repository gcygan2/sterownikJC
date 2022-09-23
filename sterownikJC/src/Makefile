CPU = atmega328p
PORT = COM3
SPEED = 115200
TARGET = main
CC = avr-gcc
CFLAGS = -O2 -mmcu=$(CPU)

$(TARGET).hex: $(TARGET).bin
	rm -f $(TARGET).hex
	avr-objcopy -j .text -j .data -O ihex $(TARGET).bin $(TARGET).hex
$(TARGET).bin: $(TARGET).o
	$(CC) $(CFLAGS) -o $@ $^
$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $< -o $@

program:
	avrdude -C../../avr/etc/avrdude.conf -c arduino -p $(CPU) \
	-P $(PORT) -b $(SPEED) -V -U flash:w:$(TARGET).hex
flash: program

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).obj $(TARGET).o \
	$(TARGET).d $(TARGET).eep $(TARGET).lst $(TARGET).lss \
	$(TARGET).sym $(TARGET).map $(TARGET)~ $(TARGET).eeprom
