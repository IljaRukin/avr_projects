/*AVR blink led*/
#define F_CPU 1000000UL			//cpu speed for compiler (UL == unsigned long)
#include <avr/io.h>					//define pins, ports, etc
#include <util/delay.h>				//import delay functions

int main(void)
{
	/* ---------- initialize ---------- */
	DDRB = 0b00000001;				//enable data direction register B on

	/* ---------- loop ---------- */
	while(1) {
		PORTB = 0b00000001;			//turn on PB0 on PORTB
		_delay_ms(1000);			//wait
		
		PORTB = 0b00000000;			//turn off PB0 on PORTB
		_delay_ms(1000);			//wait
	}
	return 0;
}
