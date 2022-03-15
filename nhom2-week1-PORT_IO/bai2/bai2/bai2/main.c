/*
 * Chop tat LED chu ky 0.5s
 */ 

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 7372800 

#define PORT_LED_O PORTB
#define DDR_LED_O DDRB
#define BIT_LED_O 6

#define PORT_BUZZER_O PORTB
#define DDR_BUZZER_O DDRB
#define BIT_BUZZER_O 7

void Init_IO(void){
	DDR_LED_O |= (1<<BIT_LED_O);
	DDR_BUZZER_O |= (1<<BIT_BUZZER_O);
}

int main(void)
{
	Init_IO();
	for(;;){
		PORT_LED_O |= (1<<BIT_LED_O);
		PORT_BUZZER_O &= ~(1<<BIT_BUZZER_O);
		_delay_ms(500);
		PORT_LED_O &= ~(1<<BIT_LED_O);
		PORT_BUZZER_O |= (1<<BIT_BUZZER_O);
		_delay_ms(500);
	}
	
}

