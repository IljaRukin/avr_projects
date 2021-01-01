#define F_CPU 16000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>


//uint8_t cube[layer][row];
uint8_t cube[8][8];


void update(void)
{
	for(uint8_t layer = 0; layer < 8; layer++)
	{
		for(uint8_t row = 0; row < 8; row++)
		{
			PORTB &= ~(1 << PB0);			//set SRCLK and RCLK to zero
			PORTB &= ~(1 << PB1);
			
			PORTD = cube[layer][row];		//set values for specified layer
			
			asm volatile("nop");
			asm volatile("nop");
			PORTB |= (1 << PB0);			//SRCLK (shifting bit to shift register on rising edge) (one bit to each shift register at outputs from portD)
			asm volatile("nop");
			asm volatile("nop");
			PORTB &= ~(1 << PB0);
		}
			
			asm volatile("nop");
			asm volatile("nop");
			PORTB |= (1 << PB1);			//RCLK (shifting bit to memory on rising edge)
			asm volatile("nop");
			asm volatile("nop");
			PORTB &= ~(1 << PB1);
			
			PORTB &= ~(1 << PB2);			//OE low (display result)
			if(layer == 0)
			{
				PORTC^= (1 << PC5);
				_delay_ms(1.2);
				PORTC^= (1 << PC5);
			}
			else if(layer == 1)
			{
				PORTC^= (1 << PC4);
				_delay_ms(1.5);
				PORTC^= (1 << PC4);
			}
			else if(layer == 2)
			{
				PORTC^= (1 << PC3);
				_delay_ms(1.5);
				PORTC^= (1 << PC3);
			}
			else if(layer == 3)
			{
				PORTC^= (1 << PC2);
				_delay_ms(0.3);
				PORTC^= (1 << PC2);
			}
			else if(layer == 4)
			{
				PORTC^= (1 << PC1);
				_delay_ms(0.3);
				PORTC^= (1 << PC1);
			}
			else if(layer == 5)
			{
				PORTC^= (1 << PC0);
				_delay_ms(0.3);
				PORTC^= (1 << PC0);
			}
			else if(layer == 6)
			{
				PORTB^= (1 << PB5);
				_delay_ms(0.3);
				PORTB^= (1 << PB5);
			}
			else if(layer == 7)
			{
				PORTB^= (1 << PB4);
				_delay_ms(0.3);
				PORTB^= (1 << PB4);
			}
			PORTB |= (1 << PB2);
	}
}

ISR( TIMER0_OVF_vect )			//execute interrupt on timer overflow
{
	TCNT0 = 0;				//set timer value
	
	asm volatile("nop");
	
	update();
}

void fill_cube(void)
{
	for(uint8_t layer = 0; layer < 8; layer++)
	{
		for(uint8_t row = 0; row < 8; row++)
		{
			cube[layer][row]=0;
		}
	}
}

void clear_cube(void)
{
	for(uint8_t layer = 0; layer < 8; layer++)
	{
		for(uint8_t row = 0; row < 8; row++)
		{
			cube[layer][row]=255;
		}
	}
}

void flip_perimeter1(uint8_t layer)
{
	cube[layer][3] ^=0b00011000;
	cube[layer][4] ^=0b00011000;
}
void flip_perimeter2(uint8_t layer)
{
	cube[layer][2] ^=0b00111100;
	cube[layer][3] ^=0b00100100;
	cube[layer][4] ^=0b00100100;
	cube[layer][5] ^=0b00111100;
}
void flip_perimeter3(uint8_t layer)
{
	cube[layer][1] ^=0b01111110;
	cube[layer][2] ^=0b01000010;
	cube[layer][3] ^=0b01000010;
	cube[layer][4] ^=0b01000010;
	cube[layer][5] ^=0b01000010;
	cube[layer][6] ^=0b01111110;
}
void flip_perimeter4(uint8_t layer)
{
	cube[layer][0] ^=0b11111111;
	cube[layer][1] ^=0b10000001;
	cube[layer][2] ^=0b10000001;
	cube[layer][3] ^=0b10000001;
	cube[layer][4] ^=0b10000001;
	cube[layer][5] ^=0b10000001;
	cube[layer][6] ^=0b10000001;
	cube[layer][7] ^=0b11111111;
}

void wave(void)
{
	fill_cube();
	
	//fade to plane in the middle
	_delay_ms(100);
	for(uint8_t row = 0; row < 8; row++)
	{
		cube[0][row]=255;
		cube[4][row]=255;
	}
	_delay_ms(100);
	for(uint8_t row = 0; row < 8; row++)
	{
		cube[1][row]=255;
		cube[3][row]=255;
	}
	
	//wave rise
	_delay_ms(100);
	flip_perimeter1(2);
	flip_perimeter1(3);
	_delay_ms(100);
	flip_perimeter1(3);
	flip_perimeter1(4);
	flip_perimeter2(2);
	flip_perimeter2(3);
	_delay_ms(100);
	flip_perimeter1(4);
	flip_perimeter1(3);
	flip_perimeter2(3);
	flip_perimeter2(4);
	flip_perimeter3(2);
	flip_perimeter3(3);
	for(uint8_t cycle;cycle<10;cycle++)
	{
		_delay_ms(100);
		flip_perimeter1(3);
		flip_perimeter1(2);
		flip_perimeter2(4);
		flip_perimeter2(3);
		flip_perimeter3(3);
		flip_perimeter3(4);
		flip_perimeter4(2);
		flip_perimeter4(3);
		_delay_ms(100);
		flip_perimeter1(2);
		flip_perimeter1(1);
		flip_perimeter2(3);
		flip_perimeter2(2);
		flip_perimeter3(4);
		flip_perimeter3(3);
		flip_perimeter4(3);
		flip_perimeter4(4);
		_delay_ms(100);
		flip_perimeter1(1);
		flip_perimeter1(0);
		flip_perimeter2(2);
		flip_perimeter2(1);
		flip_perimeter3(3);
		flip_perimeter3(2);
		flip_perimeter4(4);
		flip_perimeter4(3);
		_delay_ms(100);
		flip_perimeter1(0);
		flip_perimeter1(1);
		flip_perimeter2(1);
		flip_perimeter2(0);
		flip_perimeter3(2);
		flip_perimeter3(1);
		flip_perimeter4(3);
		flip_perimeter4(2);
		_delay_ms(100);
		flip_perimeter1(1);
		flip_perimeter1(2);
		flip_perimeter2(0);
		flip_perimeter2(1);
		flip_perimeter3(1);
		flip_perimeter3(0);
		flip_perimeter4(2);
		flip_perimeter4(1);
		_delay_ms(100);
		flip_perimeter1(2);
		flip_perimeter1(3);
		flip_perimeter2(1);
		flip_perimeter2(2);
		flip_perimeter3(0);
		flip_perimeter3(1);
		flip_perimeter4(1);
		flip_perimeter4(0);
		_delay_ms(100);
		flip_perimeter1(3);
		flip_perimeter1(4);
		flip_perimeter2(2);
		flip_perimeter2(3);
		flip_perimeter3(1);
		flip_perimeter3(2);
		flip_perimeter4(0);
		flip_perimeter4(1);
		_delay_ms(100);
		flip_perimeter1(4);
		flip_perimeter1(3);
		flip_perimeter2(3);
		flip_perimeter2(4);
		flip_perimeter3(2);
		flip_perimeter3(3);
		flip_perimeter4(1);
		flip_perimeter4(2);
	}
	
	//fade into plane
	_delay_ms(100);
	flip_perimeter1(3);
	flip_perimeter1(2);
	flip_perimeter2(4);
	flip_perimeter2(3);
	flip_perimeter3(3);
	flip_perimeter3(4);
	flip_perimeter4(2);
	flip_perimeter4(3);
	_delay_ms(100);
	flip_perimeter1(2);
	flip_perimeter1(1);
	flip_perimeter2(3);
	flip_perimeter2(2);
	flip_perimeter3(4);
	flip_perimeter3(3);
	flip_perimeter4(3);
	flip_perimeter4(4);
	_delay_ms(100);
	flip_perimeter1(1);
	flip_perimeter1(0);
	flip_perimeter2(2);
	flip_perimeter2(1);
	flip_perimeter3(3);
	flip_perimeter3(2);
	flip_perimeter4(4);
	flip_perimeter4(3);
	_delay_ms(100);
	flip_perimeter1(0);
	flip_perimeter1(1);
	flip_perimeter2(1);
	flip_perimeter2(0);
	flip_perimeter3(2);
	flip_perimeter3(1);
	flip_perimeter4(3);
	flip_perimeter4(2);
	_delay_ms(100);
	flip_perimeter1(1);
	flip_perimeter1(2);
	flip_perimeter2(0);
	flip_perimeter2(1);
	flip_perimeter3(1);
	flip_perimeter3(0);
	flip_perimeter4(2);
	flip_perimeter4(1);
	_delay_ms(100);
	flip_perimeter2(1);
	flip_perimeter2(2);
	flip_perimeter3(0);
	flip_perimeter3(1);
	flip_perimeter4(1);
	flip_perimeter4(0);
	_delay_ms(100);
	flip_perimeter3(1);
	flip_perimeter3(2);
	flip_perimeter4(0);
	flip_perimeter4(1);
	_delay_ms(100);
	flip_perimeter4(1);
	flip_perimeter4(2);

	//expand plane to cube
	_delay_ms(100);
	for(uint8_t row = 0; row < 8; row++)
	{
		cube[1][row]=0;
		cube[3][row]=0;
	}
	_delay_ms(100);
	for(uint8_t row = 0; row < 8; row++)
	{
		cube[0][row]=0;
		cube[4][row]=0;
	}
}

void init(void)
{
	DDRD |= 0b11111111;		//enable PD as output for serial data (PD0-PD7: 1-8)
	PORTD &= 0b00000000;		//set output to zero
	
	DDRC |= 0b00111111;		//enable PC as output for layer multiplexing with 2N22222 (PC0-PC5: 6-1)
	PORTC &= ~0b00111111;		//set output to zero
	
	DDRB |= 0b00111111;		//enable PB as output:
							//PB0:		SRCLK	shift by one		(low/rising edge)
							//PB1:		RCLK	store for display	(low/rising edge)
							//PB2:		OE	display			(high/low for display)
							//PB3:		SRCLR	clear data		(high!)
							//PB4-5: 	layer multiplexing with 2N22222 (form 8 to 7)
							//PB6-7: 	oszillator
	PORTB &= ~0b00110011;	///setup
	PORTB |=  0b00001100;	///setup

	//setup timer1 interrupt on overflow
	TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00); 	//8bit timer with prescaler of 1024 (16mhz/1024/256 -> 61Hz)
	TIMSK0 |= (1 << TOIE0);					//enable timer overflow interrupt
	TCNT0 = 0;				//set timer value

	sei();					//enable interrupts

	clear_cube();
}

int main(void)
{
	init();
	_delay_ms(100);
	while( 1 )
	{
		fill_cube();
		_delay_ms(500);
		wave();
		_delay_ms(500);
		clear_cube();
		_delay_ms(500);
	}
	return 0;
}
