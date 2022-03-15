//Dùng nút nh?n k?t n?i chân v?i ng?t ngoài
//Khi nh?n nút thì ?èn USER LED ??o tr?ng thái và bi?n ??m t?ng lên 1 ??n v?
//In ra LCD bi?n ??m và reset v? 0 n?u l?n h?n 9
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/myLCD.h>

#define F_CPU	7372800UL

#define PORT_SWITCH_IP  PORTE
#define DDR_SWITCH_IP   DDRE
#define PIN_SWITCH_IP   PINE
#define BIT_SWITCH_IP	4


#define PORT_LED_O      PORTB
#define DDR_LED_O       DDRB
#define BIT_LED_O       6
volatile int8_t a=0;


void init_INT()
{

	EIMSK |= (1 << INT4);
	EICRB |= (1<< ISC41) | (0 << ISC40);
	sei();
}

void init_ports()
{
	DDR_LED_O |= (1<<BIT_LED_O);
	
	DDR_SWITCH_IP &= ~(1<<BIT_SWITCH_IP);
	PORT_SWITCH_IP |= (1<<BIT_SWITCH_IP);
}
int main(void)
{
	//PORT_LED_O |= (1<<BIT_LED_O);
	init_ports();
	char a_buffer[16];
	init_LCD();
	init_INT();
	while(1){
		//PORT_LED_O |= (1<<BIT_LED_O);
		//init_INT();
		//PORT_LED_O &= ~(1<<BIT_LED_O);
		move_LCD(1,1);
		sprintf(a_buffer, "%d", a);
		print_LCD(a_buffer);
		_delay_ms(1000);
	}
	return 0;
}
//	return 0;
//}
ISR(INT4_vect){
	if(PORT_LED_O&(1<<BIT_LED_O)){
		PORT_LED_O &= ~(1<<BIT_LED_O);
	}
	else
	{
		PORT_LED_O |= (1<<BIT_LED_O);
	}
	a++;
	if(a>9){
		a=0;
	}
}