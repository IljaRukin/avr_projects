
#####alternative1

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

//----------OR----------

#####alternative2

//note
// avr/io.h includes contains the positions of ports relative to a Data Direction Register
//define PA0 0
//define PA1 1
//...
//define PB0 0
//define PB1 1

#define LED			PB0
#define LED_PORT	PORTB
#define LED_DDR		DDRB

#include <avr/io.h>				%define pins, ports, etc
#include <util/delay.h>			%import delay functions

int main(void)
{
	/* ---------- initialize ---------- */
	LED_DDR |= (1 << PB1)			%enable data direction register B on

	/* ---------- loop ---------- */
	while(1) {
		LED_PORT |= (1 << LED);		%turn on PB0 on PORTB
		_delay_ms(1000);			%wait
		
		LED_PORT |= ~(1 << LED);	%turn off PB0 on PORTB
		_delay_ms(1000);			%wait
	}
	return 0;
}


//----------OR----------

#####alternative3

#define LED			PB0
#define LED_PORT	PORTB
#define LED_DDR		DDRB

//B,x = position of bit on port
//P = port name
#define pos(x)		(1 << x)
#define setBit(P,B)		|= (1 << pos(x));
#define flipBit(P,B)	P ^= (1 << pos(x));
#define clearBit(P,B)	P &= ~(1 << pos(x));

#include <avr/io.h>				%define pins, ports, etc
#include <util/delay.h>			%import delay functions

int main(void)
{
	/* ---------- initialize ---------- */
	setBit(LED_DDR, LED);			%enable data direction register B on

	/* ---------- loop ---------- */
	while(1) {
		flipBit(LED_PORT, LED);		%turn on PB0 on PORTB
		_delay_ms(1000);			%wait
		
		flipBit(LED_PORT, LED);		%turn off PB0 on PORTB
		_delay_ms(1000);			%wait
	}
	return 0;
}
