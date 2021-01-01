/*AVR blink led*/
#define F_CPU 8000000UL			//cpu speed for compiler (UL == unsigned long)
#include <avr/io.h>					//define pins, ports, etc
#include <util/delay.h>				//import delay functions

int main(void)
{
	/* ---------- initialize ---------- */
	DDRB |= (1<<PB0);				//enable data direction register B on

	/* ---------- loop ---------- */
	while(1) {
		PORTB |= (1<<PB0);			//turn on PB0 on PORTB
		_delay_ms(500);			//wait
		
		PORTB &= ~(1<<PB0);			//turn off PB0 on PORTB
		_delay_ms(500);			//wait
	}
	return 0;
}
