# avr_gcc_projects

random projects with atmega328p using avr gcc and make including:
- simple blinking LED on Pin PB0 (blink)
- 8x8x5 LED cube with shift registers (simplecube); 8x8bit shift registers to adress a layer and 5 pins to select each layer. LED values (on/off) are stored in an array and can be edited in the main loop. the cube is displayed by triggering a timer interrupt periodically.
- rotating 40 LED strip to display earth globe (sphere); every rotation a magnet and hall sensor triggers an interrupt, wich is used to calculate the rotation frequency and the time for each slice of the globe to be displayed
each atemga328p pin controlls 2 leds wich are multiplexed using one additional pin to switch two npn- & pnp-transistors to select wich leds are used
- automatic plant watering using 5x humidity sensors and pumps (plant_watering); every ~1min the watchdoch triggers an interrupt, the voltage values from the sensors are read (ADC) and if they lie below a threshold the pumps are turned on to water the individual plants. then the atmega328p is put into deep sleep.

-----

## compile and flash
the included makefile is as simple as possible. it supports compiling/linking and flashing. to do so type:
make compile
make flash

and to remove the .o .elf files type:
make clean

if flashing should be done slowly (when the clock speed is set to 128kHz or below) type:
make flash_slow

the makefile also has some predefined fuse configurations for the atmega328p. Be careful when changing fuses to not set write protection or disable the isp interface.
here are the predefined fuse settings:
fuses_default
fuses_internal_8mhz
fuses_internal_1mhz
fuses_external_16mhz

for the last two beware: slow sck programmer needed:
fuses_internal_128khz
fuses_internal_16khz

to display all commands type:
make help

-----

## steps to install the toolchain in windows:

### install
https://www.microchip.com/en-us/development-tools-tools-and-software/gcc-compilers-avr-and-arm
https://sourceforge.net/projects/gnuwin32/files/make/
https://sourceforge.net/projects/winavr/

### add to path
C:\Program Files\avr8-gnu-toolchain-win32_x86\bin
C:\Program Files (x86)\GnuWin32\bin
C:\Program Files\avrdude

### test functionality in terminal
make -v
avr-gcc --version
avrdude

### setup USBasp
install libusbK driver using Zadig-2.4.exe

-----

## update USBasp programmer

### download
source: https://www.fischl.de/usbasp/usbasp.2011-05-28.tar.gz

### unzip
tar -xvf usbasp.2011-05-28.tar.gz
cd /usbasp.2011-05-28

### flash with only jumper 2 set
sudo avrdude -p atmega8 -c usbasp -U flash:w:bin/firmware/usbasp.atmega8.2011-05-28.hex:i

### verify with jumper 2 unset and jumper 3 set
avrdude -p attiny13 -c usbasp -B 1024 -F -P usb
