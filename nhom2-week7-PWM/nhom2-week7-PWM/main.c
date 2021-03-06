/*
 * nhom2-week7-PWM.c
 *
 * Created: 6/1/2022 9:36:35 PM
 * Author : Group 2 Vo Tan Minh Khoi
				    Ngac Bao Nam
					Phung Thi Huong
 */ 
 
// PWM Example
#define F_CPU 7372800UL
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

// Output Port pin LED_O
#define PORT_LED_O         PORTB
#define DDR_LED_O          DDRB
#define BIT_LED_O          6
// Define frequency
#define PWM_MAX_DUTY_CYCLE 0x3FF

void PWM_vInit(void)
{
    /* 
       Start Timer 1 with clock prescaler CLK/8 and phase correct 
       10-bit PWM mode. Output on PB6 (OC1B). Resolution is 1.09 us.
       Frequency is 450 Hz.
    */
    TCCR3A =  (0<<COM3A1)|(0<<COM3A0)|(1<<COM3B1)|(0<<COM3B0)
             |(0<<COM3C1)|(0<<COM3C0)|(1<<WGM31) |(1<<WGM30);

    TCCR3B =  (0<<ICNC3) |(0<<ICES3) |(0<<WGM33) |(0<<WGM32)
             |(0<<CS32)  |(1<<CS31)  |(0<<CS30);

    // Reset counter
    TCNT3 = 0;

    // Set duty cycle to 0%
    OCR3B  = 0;
	// set top value
	//ICR3 = 10000;
}
void Io_init(void){
	DDRD |= 0x00;
	DDRE |= (1<<PE4)|(1<<PE2)|(1<<PE3);
	PORTE &=~ (1<<PE4)|(1<<PE2)|(1<<PE3);
}
void PWM_vSetDutyCycle(uint16_t u16DutyCycle)
{
    // Clip parameter to maximum value
    if (u16DutyCycle > PWM_MAX_DUTY_CYCLE)
    {
        u16DutyCycle = PWM_MAX_DUTY_CYCLE;
    }

    OCR3B = u16DutyCycle;
}

int main(void)
{
	//uint16_t incrflag;
	uint16_t ledIntensity = 0;
	
    // Set LED_O as output pin
    DDR_LED_O |= (1<<BIT_LED_O);
	DDRE |= (1<<PE4);
	
    // Initialise PWM
    PWM_vInit();
	Io_init();
    // Set duty cycle to 1%
	
		 //PWM_vSetDutyCycle(PWM_MAX_DUTY_CYCLE/100);
		 _delay_ms(10);
	
   
    // Repeat indefinitely
    while(1)
    {
	   PWM_vSetDutyCycle(1024);
	   PORTE|=(1<<PE2);
	   PORTE&=~(1<<PE3);
	   _delay_ms(5000);
	   
	   PWM_vSetDutyCycle(512);
	  PORTE|=(1<<PE2);
	  PORTE&=~(1<<PE3);
	  _delay_ms(5000);

	   PWM_vSetDutyCycle(256);
	   PORTE|=(1<<PE2);
	   PORTE&=~(1<<PE3);
	   _delay_ms(5000);

	   PWM_vSetDutyCycle(128);
	   PORTE|=(1<<PE2);
	   PORTE&=~(1<<PE3);
	   _delay_ms(5000);

	   PWM_vSetDutyCycle(0);
	   PORTE|=(1<<PE2);
	   PORTE&=~(1<<PE3);
	   _delay_ms(5000);
	        	 
	   PWM_vSetDutyCycle(128);
	   PORTE &=~(1<<PE2);
	   PORTE|=(1<<PE3);
	   _delay_ms(5000);

	   PWM_vSetDutyCycle(256);
	   PORTE &=~(1<<PE2);
	   PORTE|=(1<<PE3);
	   _delay_ms(5000);
	   	     
	   PWM_vSetDutyCycle(512);
	   PORTE &=~(1<<PE2);
	   PORTE|=(1<<PE3);
	   _delay_ms(5000);
	   
    }
}