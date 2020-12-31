# avr_gcc_projects

random projects with atmega328p using avr gcc and make including:
- simple blinking LED on Pin PB0 (blink)
- 8x8x5 LED cube with shift registers (simplecube); 8x8bit shift registers to adress a layer and 5 pins to select each layer. LED values (on/off) are stored in an array and can be edited in the main loop. the cube is displayed by triggering a timer interrupt periodically.
- rotating 40 LED strip to display earth globe (sphere); every rotation a magnet and hall sensor triggers an interrupt, wich is used to calculate the rotation frequency and the time for each slice of the globe to be displayed
each atemga328p pin controlls 2 leds wich are multiplexed using one additional pin to switch two npn- & pnp-transistors to select wich leds are used
- automatic plant watering using 5x humidity sensors and pumps (plant_watering); every ~1min the watchdoch triggers an interrupt, the voltage values from the sensors are read (ADC) and if they lie below a threshold the pumps are turned on to water the individual plants. then the atmega328p is put into deep sleep.


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
