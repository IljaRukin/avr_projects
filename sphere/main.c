#define F_CPU 8000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>

//40LEDs == 2layers times 20LEDs, organized in 5x 8bit numbers

//hardware interrupt (hall sensor) => set on_time and time interrupts for displaying of the layers
//transition of the sphere can be executed in between
//note: division by power of two is done by shifting n digits to the right

#define POWER_OF_TWO 7
#define NR_SLICES 128 //2^7=128

volatile uint8_t sphere[NR_SLICES][5] = {{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000100,0b00000000},{0b00000000,0b00000000,0b01100000,0b00001110,0b00000000},{0b00010000,0b00000000,0b10100000,0b00011111,0b00000000},{0b00010000,0b00000000,0b11110000,0b00011111,0b00000000},{0b00110000,0b00000000,0b00110000,0b00111111,0b00000000},{0b11111000,0b00000000,0b10010000,0b01111111,0b00000000},{0b01111000,0b00001100,0b10010000,0b01111111,0b00000000},{0b00111000,0b00001110,0b10000000,0b00011111,0b00000000},{0b00111000,0b00001001,0b10000000,0b00001111,0b00000000},{0b10111000,0b00001001,0b11000110,0b00001111,0b00000000},{0b10111000,0b00001011,0b00010011,0b00001111,0b00000000},{0b10111000,0b00000111,0b00011101,0b00011110,0b00000000},{0b11111000,0b00110111,0b00011100,0b00011110,0b00000000},{0b11111000,0b01111111,0b00011000,0b00011110,0b00000000},{0b11111000,0b01111111,0b00111000,0b00011100,0b00000000},{0b11111000,0b01111111,0b00100011,0b00000000,0b00000000},{0b11111000,0b11111111,0b00111011,0b00000000,0b00000000},{0b11111100,0b11111111,0b00011101,0b00000000,0b00000000},{0b11111000,0b11111111,0b00001111,0b00000000,0b00000000},{0b11111000,0b11111111,0b00000000,0b00000000,0b00000000},{0b11111000,0b11111111,0b00000000,0b00000000,0b00000000},{0b11111100,0b01111111,0b00000000,0b00000000,0b00000000},{0b11111100,0b01111111,0b00000000,0b00000000,0b00000000},{0b11111100,0b01111111,0b00000000,0b00000000,0b00000000},{0b11111100,0b11111111,0b00000100,0b00000000,0b00000000},{0b11111100,0b11111111,0b00000011,0b00000000,0b00000000},{0b11111100,0b11111111,0b00000011,0b00000000,0b00000000},{0b11111100,0b11111111,0b00000001,0b00000000,0b00000000},{0b11111100,0b01111111,0b00000000,0b00000000,0b00000000},{0b11111100,0b01111111,0b00000000,0b00000000,0b00000000},{0b11111100,0b00111111,0b00000000,0b00000000,0b00000000},{0b11111110,0b00111111,0b00000000,0b00000000,0b00000000},{0b11111100,0b00111111,0b00000000,0b00000000,0b00000000},{0b11111000,0b11111111,0b00000000,0b00000000,0b00000000},{0b11111000,0b11111111,0b00000000,0b00000000,0b00000000},{0b11111000,0b11111111,0b00000011,0b00000000,0b00000000},{0b11111000,0b11111011,0b10000111,0b00000011,0b00000000},{0b11111000,0b11111111,0b00001111,0b00000111,0b00000000},{0b11111100,0b11111111,0b00011111,0b00000010,0b00000000},{0b11111000,0b11111111,0b11111111,0b00000000,0b00000000},{0b11110000,0b11111111,0b11111111,0b00000001,0b00000000},{0b11110000,0b11111101,0b11111111,0b00000011,0b00000000},{0b11111000,0b11111111,0b11111111,0b00000111,0b00000000},{0b11111000,0b11110101,0b11111111,0b00001111,0b00000000},{0b11111000,0b11110111,0b11111111,0b00011111,0b00000000},{0b11111000,0b11110111,0b11111111,0b00011111,0b00000000},{0b11111010,0b11110111,0b11111111,0b00011111,0b00000000},{0b11111010,0b11110111,0b11111111,0b00001111,0b00000000},{0b11111010,0b11110111,0b11111111,0b00000111,0b00000000},{0b11110010,0b11111011,0b00111111,0b00000001,0b00000000},{0b11100000,0b11111111,0b00000111,0b00000000,0b00000000},{0b10100000,0b11111011,0b00000111,0b00000000,0b00000000},{0b10000000,0b11111011,0b00000111,0b00000000,0b00000000},{0b11000000,0b11111111,0b00000111,0b00000000,0b00000000},{0b11000000,0b11111110,0b00000111,0b00000000,0b00000000},{0b11000000,0b11111110,0b00000111,0b00000000,0b00000000},{0b00010010,0b11110100,0b00000111,0b00000000,0b00000000},{0b00010110,0b11100000,0b00000111,0b00000000,0b00000000},{0b00011110,0b11100000,0b00000011,0b00000000,0b00000000},{0b00001110,0b00000000,0b00000001,0b00000000,0b00000000},{0b00001110,0b00000000,0b00000000,0b00000000,0b00000000},{0b00001110,0b00000000,0b00000000,0b00000000,0b00000000},{0b00011110,0b00000000,0b00000000,0b00000000,0b00000000},{0b00011110,0b00000000,0b00000000,0b00000000,0b00000000},{0b00011110,0b00000000,0b00000000,0b00000000,0b00000000},{0b00111110,0b00000000,0b00000000,0b00000000,0b00000000},{0b00011110,0b00000000,0b01100000,0b00000000,0b00000000},{0b00000010,0b00000000,0b11100000,0b00000011,0b00000000},{0b00000010,0b00000000,0b11110000,0b00000011,0b00000000},{0b00011010,0b00000000,0b11110000,0b00000011,0b00000000},{0b11011010,0b00000001,0b11110000,0b00001111,0b00000000},{0b11111110,0b00000000,0b11111000,0b00011111,0b00000000},{0b11111110,0b00000001,0b11111100,0b00111111,0b00000000},{0b11101100,0b00000001,0b11111100,0b00111111,0b01000000},{0b11101100,0b00000001,0b11111100,0b11111111,0b11000000},{0b11011100,0b00000011,0b11111110,0b11111111,0b10000000},{0b11011100,0b00000011,0b11111110,0b11111111,0b00000000},{0b11011100,0b00000111,0b11111110,0b00111111,0b00000000},{0b11011100,0b10001111,0b11111110,0b00000001,0b00000000},{0b11111100,0b00001111,0b11111110,0b00000000,0b00000000},{0b11111100,0b01001111,0b11111100,0b00000000,0b00000000},{0b11111100,0b01111111,0b00111000,0b00000000,0b00000000},{0b11111100,0b00011111,0b00000010,0b00000000,0b00000000},{0b11111000,0b00011111,0b00000011,0b00000000,0b00000000},{0b11111000,0b10011111,0b00000001,0b00000000,0b00000000},{0b11111000,0b10011111,0b00000001,0b00000000,0b00000000},{0b11111000,0b10111111,0b00000001,0b00000000,0b00000000},{0b11111000,0b11111111,0b00000000,0b00000000,0b00000000},{0b11111000,0b11111111,0b00000000,0b00000000,0b00000000},{0b11111000,0b11111111,0b00000000,0b00000000,0b00000000},{0b11111000,0b01111111,0b00000000,0b00000000,0b00000000},{0b11111000,0b00111111,0b00000000,0b00000000,0b00000000},{0b01111000,0b00111110,0b00000000,0b00000000,0b00000000},{0b00111000,0b00001110,0b00000000,0b00000000,0b00000000},{0b00111000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00111000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00110000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00110000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00100000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000}};

volatile uint8_t display_layer = 0;
volatile uint16_t on_time = 0xffff;

void turn_LEDs_off(void)
{
	PORTB |= 0b11111111;
	PORTC |= 0b00011111;
	PORTD |= 0b11111011;
}

void turn_LEDs_on(void)
{
	PORTB &= ~0b11111111;
	PORTC &= ~0b00011111;
	PORTD &= ~0b11111011;
}

void display_first_half(void)
{
	turn_LEDs_off();
	//select group of 20 LEDs
	PORTC |= (1 << PC5);
	//PORTC &= ~(1 << PC5);
	
	PORTB &= ~((sphere[display_layer][0] & 0b00000001) << 0);//PB0
	PORTB &= ~((sphere[display_layer][0] & 0b00000100) >> 1);//PB1
	PORTB &= ~((sphere[display_layer][0] & 0b00010000) >> 2);//PB2
	PORTB &= ~((sphere[display_layer][0] & 0b01000000) >> 3);//PB3
	PORTB &= ~((sphere[display_layer][1] & 0b00000001) << 4);//PB4
	PORTB &= ~((sphere[display_layer][1] & 0b00000100) << 3);//PB5
	PORTB &= ~((sphere[display_layer][1] & 0b00010000) << 2);//PB6
	PORTB &= ~((sphere[display_layer][1] & 0b01000000) << 1);//PB7
	
	PORTC &= ~((sphere[display_layer][2] & 0b00000001) << 0);//PC0
	PORTC &= ~((sphere[display_layer][2] & 0b00000100) >> 1);//PC1
	PORTC &= ~((sphere[display_layer][2] & 0b00010000) >> 2);//PC2
	PORTC &= ~((sphere[display_layer][2] & 0b01000000) >> 3);//PC3
	PORTC &= ~((sphere[display_layer][3] & 0b00000001) << 4);//PC4
	
	PORTD &= ~((sphere[display_layer][3] & 0b00000100) >> 2);//PD0
	PORTD &= ~((sphere[display_layer][3] & 0b00010000) >> 3);//PD1
	PORTD &= ~((sphere[display_layer][3] & 0b01000000) >> 3);//PD3
	PORTD &= ~((sphere[display_layer][4] & 0b00000001) << 4);//PD4
	PORTD &= ~((sphere[display_layer][4] & 0b00000100) << 3);//PD5
	PORTD &= ~((sphere[display_layer][4] & 0b00010000) << 2);//PD6
	PORTD &= ~((sphere[display_layer][4] & 0b01000000) << 1);//PD7
}

void display_second_half(void)
{
	turn_LEDs_off();
	//select group of 20 LEDs
	//PORTC |= (1 << PC5);
	PORTC &= ~(1 << PC5);

	PORTB &= ~((sphere[display_layer][0] & 0b00000010) >> 1);//PB0
	PORTB &= ~((sphere[display_layer][0] & 0b00001000) >> 2);//PB1
	PORTB &= ~((sphere[display_layer][0] & 0b00100000) >> 3);//PB2
	PORTB &= ~((sphere[display_layer][0] & 0b10000000) >> 4);//PB3
	PORTB &= ~((sphere[display_layer][1] & 0b00000010) << 3);//PB4
	PORTB &= ~((sphere[display_layer][1] & 0b00001000) << 2);//PB5
	PORTB &= ~((sphere[display_layer][1] & 0b00100000) << 1);//PB6
	PORTB &= ~((sphere[display_layer][1] & 0b10000000) << 0);//PB7
	
	PORTC &= ~((sphere[display_layer][2] & 0b00000010) >> 1);//PC0
	PORTC &= ~((sphere[display_layer][2] & 0b00001000) >> 2);//PC1
	PORTC &= ~((sphere[display_layer][2] & 0b00100000) >> 3);//PC2
	PORTC &= ~((sphere[display_layer][2] & 0b10000000) >> 4);//PC3
	PORTC &= ~((sphere[display_layer][3] & 0b00000010) << 3);//PC4
	
	PORTD &= ~((sphere[display_layer][3] & 0b00001000) >> 3);//PD0
	PORTD &= ~((sphere[display_layer][3] & 0b00100000) >> 4);//PD1
	PORTD &= ~((sphere[display_layer][3] & 0b10000000) >> 4);//PD3
	PORTD &= ~((sphere[display_layer][4] & 0b00000010) << 3);//PD4

	PORTD &= ~((sphere[display_layer][4] & 0b00001000) << 2);//PD5
	PORTD &= ~((sphere[display_layer][4] & 0b00100000) << 1);//PD6
	PORTD &= ~((sphere[display_layer][4] & 0b10000000) << 0);//PD7
}

void fill_sphere(void)
{
	for(uint8_t slice = 0; slice < NR_SLICES; slice++)
	{
		for(uint8_t nr = 0; nr < 5; nr++)
		{
			sphere[slice][nr] = 0xff;
		}
	}
}

void clear_sphere(void)
{
	for(uint8_t slice = 0; slice < NR_SLICES; slice++)
	{
		for(uint8_t nr = 0; nr < 5; nr++)
		{
			sphere[slice][nr] = 0x00;
		}
	}
}

void shift_sphere(void)
{
/*	uint8_t temp = sphere[0][4];
	sphere[0][4] = sphere[0][3];
	sphere[0][3] = sphere[0][2];
	sphere[0][2] = sphere[0][1];
	sphere[0][1] = sphere[0][0];
	sphere[0][0] = temp;*/
	uint8_t temp = sphere[0][4] >> 7;
	sphere[0][4] = sphere[0][4] << 1;
	sphere[0][4] ^= sphere[0][3] >> 7;
	sphere[0][3] = sphere[0][3] << 1;
	sphere[0][3] ^= sphere[0][2] >> 7;
	sphere[0][2] = sphere[0][2] << 1;
	sphere[0][2] ^= sphere[0][1] >> 7;
	sphere[0][1] = sphere[0][1] << 1;
	sphere[0][1] ^= sphere[0][0] >> 7;
	sphere[0][0] = sphere[0][0] << 1;
	sphere[0][0] ^= temp;
}

ISR( INT0_vect )			//execute interrupt on external interrupt (magnet)
{
	//TIMSK1 |= (1 << OCIE1A)|(1 << OCIE1B);	//set timer compare and overflow interrupt
	on_time = TCNT1 >> POWER_OF_TWO;//set on time for display of one layer
	OCR1A = on_time >> 1;		//set compare value A (half of time for compare B)
	OCR1B = on_time;		//set compare value B
	display_layer = 0;		//start with layer 0
	TCNT1 = 0;			//set timer value to zero
}

ISR( TIMER1_COMPA_vect )		//execute interrupt on timer compare A => display first half
{
	display_first_half();
	OCR1A += on_time;	//set interrupt time for next layer
}


ISR( TIMER1_COMPB_vect )		//execute interrupt on timer compare B => display second half
{
	display_second_half();
	OCR1B += on_time;	//set interrupt time for next layer
	if (display_layer < NR_SLICES)
	{
		display_layer = display_layer +1;	//set number of next layer
	}
	else
	{
		display_layer = 0;
		OCR1A = on_time >> 1;
		OCR1B = on_time;
	}
}

ISR( TIMER1_OVF_vect )			//execute interrupt on timer overflow => turn leds off & disable compare interrupts
{
	//TIMSK1 &= ~ ((1 << OCIE1A)|(1 << OCIE1B));	//disable timer compare and overflow interrupt
	//turn_LEDs_off();
}

void init(void)
{
	//enable outputs
	DDRB |= 0b11111111;
	DDRC |= 0b00011111;
	DDRD |= 0b11111011;
	turn_LEDs_off();	//turn LEDs off
	//layer shifting pin PD5
	DDRC |= (1 << DDC5);
	PORTC |= (1 << PC5);	//select first group of LEDs
	
	//enable PD2 as input
	DDRD &= ~(1<<DDD2);
	// set pull-up resistor PD2 (for hall sensor)
	PORTD |= (1<<PD2);
	//set interrupt INT0
	EIMSK |= 1<<INT0;	//enable INT0
	EICRA |= (1<<ISC01)|(0<<ISC00); // interrupt on falling edge


/*	//8bit timer0, 1024 prescaler,compare output with OCR0A & OCR0B
	//256*1024/8mhz
	TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);
	//TCCR0B |=  (1 << FOC0A) | (1 << FOC0B);	//force compare match regardless of value
	//OCR0A = 0xff;	//set compare value
	//OCR0B = 0xff;	//set compare value
	TCNT0 = 0;		//set timer value to zero
	TIMSK0 |= (1 << OCIE0A)|(1 << OCIE0B);	//set timer compare interrupt*/


	//16bit timer1, compare output with OCR1A & OCR1B
	//256*1024/8mhz
	TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);	//prescaler: 1024
	//TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);	//prescaler: 256
	//TCCR1B |= (1 << CS12) | (1 << CS11) | (1 << CS10);	//prescaler: 64
	//TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);	//prescaler: 8
	//TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);	//prescaler: 1
	//TCCR1C |=  (1 << FOC1A) | (1 << FOC1B);	//force compare match regardless of value
	OCR1A = 0xffff;		//set maximum compare value (delay)
	OCR1B = 0xffff;		//set maximum compare value (delay)
	TCNT1 = 0;		//set timer value to zero
	TIMSK1 |= (1 << OCIE1A)|(1 << OCIE1B)|(1 << TOIE1);	//set timer compare and overflow interrupt


	sei();					//enable interrupts
}

int main(void)
{
	init();
	turn_LEDs_off();
	while( 1 )
	{
/*		fill_sphere();
		_delay_ms(2);
		display_first_half();
		_delay_ms(200);
		display_second_half();
		_delay_ms(200);

		clear_sphere();
		_delay_ms(2);
		display_first_half();
		_delay_ms(200);
		display_second_half();
		_delay_ms(200);
*/

		//turn_LEDs_off();
		//_delay_ms(2000);
	}
	return 0;
}
