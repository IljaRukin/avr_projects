###Makefile AVR Atmel Atmega328P###
target = main
mcu = atmega328
avr_mcu = m328p
baud = 19200
F_OSC = 16000000
F_CPU = 16000000

#all comands will be applied only if the corresponding file dependence changes or output files are not present
#following command will be executed always:
.PHONY: help compile flash flash_slow clean fuses_default fuses_internal_8mhz fuses_internal_1mhz fuses_internal_128khz fuses_internal_16khz fuses_external_16mhz

#syntax:
#command: file dependence
#	executed script

help:
	@echo 'to start script type:   sudo make   and add ..... '
	@echo 'included commands:'
	@echo 'help'
	@echo 'compile'
	@echo 'flash'
	@echo 'flash_slow'
	@echo 'clean'
	@echo 'fuses_default'
	@echo 'fuses_internal_8mhz'
	@echo 'fuses_internal_1mhz'
	@echo 'fuses_external_16mhz'
	@echo '!!! beware: slow sck programmer needed !!!'
	@echo '!!! fuses_internal_128khz !!!'
	@echo '!!! fuses_internal_16khz !!!'

###command 'all' will be executed always, all other command should be called seperately

compile: ${target}.c
	@echo '####################compile####################'
#-mmcu=#mcu#			specify mcu
#-Os					optimize size
#-O3					optimize speed
#-o #output_file#		output file name
#-cn					only compile & don't link
#-g						output process status
	avr-gcc -I ./ -Os -g -c -mmcu=${mcu} -o ${target}.o ${target}.c
	avr-gcc -I ./ -mmcu=${mcu} -o ${target}.elf ${target}.o
#-O ihex #filename#		output as intel-hex file
#-j #section#			use only specified sections
	avr-objcopy -j .text -j .data -O ihex ${target}.elf ${target}.hex

flash:
	@echo '####################flash####################'
#-v									output process status
#-e									flash erase
#-p #mcu#							set msu
#-c #programmer#					set programmer
#-b #baud#							set baud
#-U #process#:#operation#:#file#	choose file
#-U lfuse:w:#l_fuse#:m 			set low fuse
#-U hfuse:w:#h_fuse#:m			set high fuse
#-U efuse:w:#e_fuse#:m			set extended fuse
	avrdude -v -p ${avr_mcu} -c usbasp -b ${baud} -B 10 -U flash:w:${target}.hex

flash_slow:
	@echo '####################slow flash 1kHz####################'
	avrdude -v -p ${avr_mcu} -c usbasp -B 512 -U flash:w:${target}.hex -F

clean:
	@echo '####################clean####################'
	rm -f *.o *.elf


fuses_default:
	@echo '####################fuses default (1MHz)####################'
	@echo 'this option is for saving the avr when its running slow'
	@echo 'please set slow sck jumper on the programmer'
	avrdude -v -p ${avr_mcu} -c usbasp -B 512 -U lfuse:w:0x62:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m
#	avrdude -v -p ${avr_mcu} -c usbasp -b ${baud} -U lfuse:w:0x62:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m

#all other options:
#bootsize: 256 words boot sector, preserve eeprom:

fuses_internal_8mhz:
	@echo '####################fuses internal oszillator (8mHz)####################'
	avrdude -v -p ${avr_mcu} -c usbasp -b ${baud} -B 10 -U lfuse:w:0xE2:m -U hfuse:w:0xD7:m -U efuse:w:0xFF:m

fuses_internal_1mhz:
	@echo '####################fuses internal oszillator (1mHz)####################'
	avrdude -v -p ${avr_mcu} -c usbasp -b ${baud} -B 10 -U lfuse:w:0x62:m -U hfuse:w:0xD7:m -U efuse:w:0xFF:m

fuses_external_16mhz:
	@echo '####################fuses external oszillator (16mhz)####################'
	avrdude -v -p ${avr_mcu} -c usbasp -b ${baud} -B 10 -U lfuse:w:0xF7:m -U hfuse:w:0xD7:m -U efuse:w:0xFF:m

#beware this options (they need a slow sck programmer)

fuses_internal_128khz:
	@echo '####################fuses internal oszillator (128kHz)####################'
	@echo 'warning: flashing only with slow sck possible'
	avrdude -v -p ${avr_mcu} -c usbasp -b ${baud} -B 10 -U lfuse:w:0xE3:m -U hfuse:w:0xD7:m -U efuse:w:0xFF:m

fuses_internal_16khz:
	@echo '####################fuses internal oszillator (16kHz)####################'
	@echo 'warning: flashing only with slow sck possible'
	avrdude -v -p ${avr_mcu} -c usbasp -b ${baud} -B 10 -U lfuse:w:0x63:m -U hfuse:w:0xD7:m -U efuse:w:0xFF:m


