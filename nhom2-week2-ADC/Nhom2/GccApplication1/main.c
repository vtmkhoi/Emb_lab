#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/iom16a.h>
#include <avr/myLCD.h>


#define LED_DDR 	DDRB
#define LED_PORT	PORTB
#define LED_BIT		6

#define BUZZER_DDR DDRB
#define BUZZER_PORT PORTB
#define BUZZER_BIT 7

void ADC_Init(){
	ADMUX = (0 << REFS1)|(1 << REFS0)|(0 << ADLAR)
	|(0 << MUX4)|(0 << MUX3)|(0 << MUX2)|(0 << MUX1)|(1 << MUX0);	//Channel 1
	
	ADCSRA = (1 << ADEN)|(0 << ADSC)|(0 << ADFR)|(0 << ADIF)
	|(0 << ADIE)|(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0); // chia xung nhip 128
}

uint16_t ADC_Dig(char channel){
	//ADMUX = channel; //| ADC_VREF_TYPE;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA&(1 << ADSC))
	{
		;
	}
	return ADCW;
}

void init_IO(void){
	LED_DDR &=~ (1<<LED_BIT);
	BUZZER_DDR |= (1<<BUZZER_BIT);
}

int main(void){
	uint16_t dig_val;
	double voltage_val;
	int temp;
	ADC_Init();
	init_IO();
	init_LCD();
	clr_LCD();
	while(1){
		dig_val = ADC_Dig(1);
		voltage_val = (((double)dig_val/1023)*5*1000);	//mV
		temp = (int)(voltage_val/10);
		
		print_LCD("Hello world");
		move_LCD(2,1);
		print_LCD("TEMP: ");
		putChar_LCD(temp/10+48);
		putChar_LCD(temp%10+48);
		putChar_LCD(223);
		//print_LCD("C");
		if(temp >= 30){
			LED_PORT |= (1 << LED_BIT);
			BUZZER_PORT &=~ (1<<BUZZER_BIT);
		}
		else{
			LED_PORT &=~ (1 << LED_BIT);
			BUZZER_PORT |= (1<<BUZZER_BIT);
		}
		_delay_ms(1000);
		clr_LCD();
	}
	return 0;
}