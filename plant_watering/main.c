#define F_CPU 1000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>

//moisture sensing PC0-PC4
//output PD7,PB0-PB5
//amplified with npn darlington transistor array ULN2003
//(ULN2003: Inputs: 2-7,ground,common connected to ground, Outputs: 15-10)
//output to 5 motor pump for watering plants
//positive voltage required to activate transistor !!


#define pump_time 3000
uint16_t threshold1 = 480; //sensor 0,1,4
uint16_t threshold2 = 480; //sensor 2,3

uint16_t adc_read(uint8_t selected_pin)
{
	selected_pin &= 0b00000111;		//keep only last 3 bits
	ADMUX = (ADMUX & 0xF8)|selected_pin;	//add this three bits to admux => selects pin
	ADCSRA |= (1<<ADSC);			//start adc
	while(ADCSRA & (1<<ADSC));		//wait till finished (ADSC bit becomes zero)
	return (ADC);				//return results (ADCH & ADCL registers)
}

//ISR( WDT_vect )		//execute interrupt on watchdog runout
ISR( TIMER1_OVF_vect )	//execute interrupt on timer1 runout
{
	cli();
	uint8_t latch = 0;
	PORTD |= (1 << PD6);	//turn LED on
	_delay_ms(200);
	if (adc_read(0b00000000) > threshold1)	//ADC0
	{
		PORTB |= (1 << PB1);
		latch = 1;
	}
	if (adc_read(0b00000001) > threshold1)	//ADC1
	{
		PORTB |= (1 << PB2);
		latch = 1;
	}
	if (adc_read(0b00000010) > threshold2)	//ADC2
	{
		PORTB |= (1 << PB3);
		latch = 1;
	}
	if (adc_read(0b00000011) > threshold2)	//ADC3
	{
		PORTB |= (1 << PB4);
		latch = 1;
	}
	if (adc_read(0b00000100) > threshold1)	//ADC4
	{
		PORTB |= (1 << PB5);
		latch = 1;
	}
	if (latch != 0)
	{
	_delay_ms(pump_time);
	PORTB &= ~0b00111111;
	PORTD &= ~(1 << PD7);
	}
	PORTD &= ~(1 << PD6);	//turn LED off
	sei();
	TCNT1 = 0;			//reset timer1
	//WDTCSR |= (1 << WDIE);	//set watchdog interrupt & reset mode
}

int main(void)
{
	//input/output
	DDRC &= ~0b00111111;	//adc input pins PC0-PC5
	MCUCR |= (1 << PUD);	//disable all pull-ups
	ADMUX |= (1<<REFS1)|(1<<REFS0);	//Vref from pin Aref
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);	//enable adc, prescaler 32
	/*
	0b00000000; //ADC0
	0b00000001; //ADC1
	0b00000010; //ADC2
	0b00000011; //ADC3
	0b00000100; //ADC4
	0b00000101; //ADC5
	*/
	
	DDRB |= 0b00111111;	//outputs PB0-PB5
	PORTB &= ~0b00111111;	//outputs off
	DDRD |= 0b11000000;	//LED + 1output PD7
	PORTD &= ~(1 << PD7);	//output PD7 off
	PORTD &= ~(1 << PD6);	//LED off

	//sleep mode
	SMCR |= (1 << SE);	//enable sleep modes
	MCUCR |= (1 << BODS) | (1 << BODSE);	//disable brown out detection
	//additional power save configuration
	PRR |= (1 << PRTWI) | (1 << PRTIM2) | (1 << PRTIM0) | (1 << PRUSART0);//do not disable adc (1 << PRADC) and Timer1 (1 << PRTIM1)

	MCUSR = 0;
	
	//set Watchdog as interrupt(8bit,prescaler:1024, runout every 2⁸cycles/128kHz = 8s)
/*	WDTCSR = (1<<WDCE)|(1<<WDE);
	WDTCSR = (1<<WDIE)|(1<<WDP3)|(1<<WDP0);*/
	
	//set timer1 as interrupt(16bit,prescaler:1024, runout every 2¹⁶cycles/1mHz = 67,1s)
	TIMSK1 |= (1<<TOIE1);
	TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10); // prescaler 1024 (67,1s)
	//TCCR1B |= (0<<CS12)|(1<<CS11)|(1<<CS10); //precaler 64 (4,25s)
	TCNT1 = 0;

	sei();	//enable interrupts


	while(1) {
		SMCR = ( (1 << SM2) | (1 << SM1) | (1 << SM0) | (1 << SE) );  //power down
  		//SMCR = ( (0 << SM2) | (0 << SM1) | (0 << SM0) | (1 << SE) );  //idle
	}
	return 0;
}
