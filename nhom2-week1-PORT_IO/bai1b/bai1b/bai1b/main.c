#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/delay.h>

// Output Port pin LED_O
#define PORT_LED_O      PORTE
#define DDR_LED_O       DDRE
#define BIT_LED_O       4

void Init_IO(void)
{
	// Set LED_O as output pin
	DDR_LED_O |= (1<<BIT_LED_O);
}
	
int main(void)
{
	
	// Repeat indefinitely
	Init_IO();
	
	while(1)
	{
		// Enable LED
		PORT_LED_O &= ~(1<<BIT_LED_O);
		_delay_ms(1000);
		PORT_LED_O |= (1<<BIT_LED_O);
		_delay_ms(1000);

	}
}
		