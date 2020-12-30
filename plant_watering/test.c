#define F_CPU 1000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>

uint16_t threshold = 450;

uint16_t adc_read(uint8_t selected_pin)
{
	selected_pin &= 0b00000111;		//keep only last 3 bits
	ADMUX = (ADMUX & 0xF8)|selected_pin;	//add this three bits to admus => selects pin
	ADCSRA |= (1<<ADSC);			//start adc
	while(ADCSRA & (1<<ADSC));		//wait till finished (ADSC bit becomes zero)
	return (ADC);				//return results (ADCH & ADCL registers)
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
	//PORTB |= 0b00111111;	//outputs on
	DDRD |= 0b11000000;	//LED + 1output PD7
	PORTD &= ~(1 << PD7);	//output PD7 off
	//PORTD |= (1 << PD7);	//output PD7 on
	PORTD &= ~(1 << PD6);	//LED off
	//PORTD |= (1 << PD6);	//LED on

	while(1)
	{
		_delay_ms(500);
		PORTD |= (1 << PD6);	//LED on
		PORTB = (1 << PB0);	//PB0
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(500);
		PORTD |= (1 << PD6);	//LED on
		PORTB = (1 << PB1);	//PB1
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(500);
		PORTD |= (1 << PD6);	//LED on
		PORTB = (1 << PB2);	//PB2
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(500);
		PORTD |= (1 << PD6);	//LED on
		PORTB = (1 << PB3);	//PB3
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(500);
		PORTD |= (1 << PD6);	//LED on
		PORTB = (1 << PB4);	//PB4
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(500);
		PORTD |= (1 << PD6);	//LED on
		PORTB = (1 << PB5);	//PB5
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(500);
		/*PORTD |= (1 << PD6);	//LED on
		PORTB = (1 << PD7);	//PD7
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTD = 0x00;
		_delay_ms(500);*/
		
		PORTD |= (1 << PD6);	//LED on
		if (adc_read(0b00000000) > threshold)
		{
			PORTB = (1 << PB0);	//PB0
			_delay_ms(500);
		}
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(2000);
		PORTD |= (1 << PD6);	//LED on
		if (adc_read(0b00000001) > threshold)
		{
			PORTB = (1 << PB1);	//PB1
			_delay_ms(500);
		}
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(2000);
		PORTD |= (1 << PD6);	//LED on
		if (adc_read(0b00000010) > threshold)
		{
			PORTB = (1 << PB2);	//PB2
			_delay_ms(500);
		}
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(2000);
		PORTD |= (1 << PD6);	//LED on
		if (adc_read(0b00000011) > threshold)
		{
			PORTB = (1 << PB3);	//PB3
			_delay_ms(500);
		}
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(2000);
		PORTD |= (1 << PD6);	//LED on
		if (adc_read(0b00000100) > threshold)
		{
			PORTB = (1 << PB4);	//PB4
			_delay_ms(500);
		}
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(2000);
		PORTD |= (1 << PD6);	//LED on
		if (adc_read(0b00000110) > threshold)
		{
			PORTB = (1 << PB5);	//PB5
			_delay_ms(500);
		}
		_delay_ms(500);
		PORTD &= ~(1 << PD6);	//LED off
		PORTB = 0x00;
		_delay_ms(2000);
	}
}
