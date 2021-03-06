/*
 * nhom2-week4-TIMER.c
 *
 * Created: 12/9/2021 1:06:27 PM
 * Author : Vo Tan Minh Khoi	
			Ngac Bao Nam		
			Phung Thi Huong
 */ 
/*
	*	Toggle led using timer, freq: 0.5s
	*	Press button --> turn on buzzer
	*	Display number blynk led on LCD
*/

//Define CPU Clocks
#define F_CPU 7372800UL

// Timers Example

#include <stdint.h>
#include <avr/io.h>
#include <avr/myLCD.h>
#include	<avr/interrupt.h> 
#include <avr/delay.h>
//#include <avr/myLCD.h>

// Boolean definitions
typedef uint8_t bool;
#define TRUE    1
#define FALSE   0

// Define pins LED, BUZZERs
#define PORT_LED_O PORTB
#define DDR_LED_O DDRB
#define BIT_LED_O 6

#define PORT_BUZZER_O PORTB
#define DDR_BUZZER_O DDRB
#define BIT_BUZZER_O 7

// Input Port pin SWITCH_IP
#define PORT_SWITCH_IP  PORTE
#define DDR_SWITCH_IP   DDRE
#define PIN_SWITCH_IP   PINE
#define BIT_SWITCH_IP	4

char buff;
volatile int8_t count = 0;
/*
* Init_IO
*/
void init_IO (void)
{
	// Set LED_O as output pin
	DDR_LED_O |= (1<<BIT_LED_O);
	
	// Set BUZZ_O as output pin
	DDR_BUZZER_O |=(1<<BIT_BUZZER_O);

	// Set SWITCH_IP as input pin
	DDR_SWITCH_IP &= ~(1<<BIT_SWITCH_IP);

	// Enable pull-up on SWITCH_IP
	PORT_SWITCH_IP |= (1<<BIT_SWITCH_IP);
}

void init_INT()
{

	EIMSK |= (1 << INT4);
	EICRB |= (1<< ISC41) | (0 << ISC40);
}

void TMR_vInit(void)
{
	/* Start timer 1 with clock prescaler CLK/1024 */
	/* Resolution is 139 us */
	/* Maximum time is 9.1 s */
	TCCR1A =  (0<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)
	|(0<<COM1C1)|(0<<COM1C0)|(0<<WGM11) |(0<<WGM10);

	TCCR1B =  (0<<ICNC1) |(0<<ICES1) |(0<<WGM13) |(0<<WGM12)
	|(1<<CS12)  |(0<<CS11)  |(1<<CS10);
}

void TMR_vStart(uint16_t u16DelayMs)
{
	// Calculate and set delay
	TCNT1  = (uint16_t) (0x10000 - ((F_CPU/1024)*u16DelayMs)/1000);

	// Clear timer overflow flag by writing a logical 1, other flags are unchanged
	TIFR = (1<<TOV1);
	// NOTE: This line is more efficient than using "TIFR |= (1<<TOV1);"
}

bool TMR_bHasExpired(void)
{
	// See if timer overflow flag is set
	if(TIFR&(1<<TOV1))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void TMR_vDelay(uint16_t u16DelayMs)
{
	TMR_vStart(u16DelayMs);
	while(!TMR_bHasExpired())
	{
		;
	}
}

void vBeep(void)
{
	count++;
	// Enable LED
/*	PORT_BUZZER_O &= ~(1<<BIT_BUZZER_O);*/
	PORT_LED_O	  |= (1<<BIT_LED_O);
	// Wait 100 ms
	TMR_vDelay(500);

	// Disable LED
/*	PORT_BUZZER_O |= (1<<BIT_BUZZER_O);*/
	PORT_LED_O	  &= ~(1<<BIT_LED_O);
}

int main(void)
{
	init_IO();
	init_LCD();
	init_INT();
	TMR_vInit();
	clr_LCD();
	print_LCD("Num: ");
	move_LCD(1,5);
	sei();
	// Disable Buzzer and Led
	PORT_BUZZER_O |= (1<<BIT_BUZZER_O);
	PORT_LED_O	  |= (0<<BIT_LED_O);
	// Initialise timer
	// Repeat indefinitely
	for(;;)
	{
		/*PORT_BUZZER_O |= (1<<BIT_BUZZER_O);*/
		vBeep();
		// Wait 1 s
		//PORT_BUZZER_O |= (1<<BIT_BUZZER_O);
		TMR_vDelay(500);
		move_LCD(1,5);
		char a_buffer[16];
		sprintf(a_buffer, "%d", count);
		print_LCD(a_buffer);
	}
}

ISR(INT4_vect)
{
	/*
	if(bit_is_clear(PINE, PE4))
	{
	PORT_BUZZER_O &= ~(1<<BIT_BUZZER_O);
	}
	else if(bit_is_set(PINE, PE4))
	{
		PORT_BUZZER_O |= (1<<BIT_BUZZER_O);
	}
	*/
	PORT_BUZZER_O &= ~(1<<BIT_BUZZER_O);
	_delay_ms(2000);
	PORT_BUZZER_O |= (1<<BIT_BUZZER_O);
	_delay_ms(2000);
}