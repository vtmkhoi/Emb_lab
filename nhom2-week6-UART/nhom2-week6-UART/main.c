/*
 * nhom2-week6-UART.c
 *
 * Created: 1/5/2022 1:53:16 PM
 * Author : Group 2 - Vo Tan Minh Khoi
					  Ngac Bao Nam
					  Phung Thi Huong
 */ 

#define F_CPU 7372800UL

// USARTs Example

#include <stdint.h>
#include <avr/io.h>
#include <avr/myLCD.h>
#include <avr/interrupt.h>

// Define baud rate
#define USART0_BAUD         115200ul
#define USART0_UBBR_VALUE   ((F_CPU/(USART0_BAUD<<4))-1)

// Output Port pin LED_O
#define PORT_LED_O      PORTB
#define DDR_LED_O       DDRB
#define BIT_LED_O       6
uint8_t u8Data;
char a_buffer[16];
void USART0_vInit(void)
{
	// Set baud rate
	UBRR0H = (uint8_t)(USART0_UBBR_VALUE>>8);
	UBRR0L = (uint8_t)USART0_UBBR_VALUE;

	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C = (0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);

	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE0)|(1<<TXEN0);
}

void USART0_vSendByte(uint8_t u8Data)
{
	// Wait if a byte is being transmitted
	while((UCSR0A&(1<<UDRE0)) == 0) //
	{
		;
	}

	// Transmit data
	UDR0 = u8Data;
}

uint8_t USART0_vReceiveByte()
{
	// Wait until a byte has been received
	while((UCSR0A&(1<<RXC0)) == 0)
	{
		;
	}

	// Return received data
	return UDR0;
}

void Init_IO(void)
{
	// Set LED_O as output pin
	DDR_LED_O |= (1<<BIT_LED_O);
}

int main(void)
{
	init_LCD();
	Init_IO();
	//PORT_LED_O |= (1<<BIT_LED_O);

	clr_LCD();
	print_LCD("DATA: ");
	move_LCD(1,6);
	// Initialise USART
	USART0_vInit();
	
	// Send string
	USART0_vSendByte('hehe');
	USART0_vSendByte('\r');
	USART0_vSendByte('\n');
	// Repeat indefinitely
	sei();
	for(;;)
	{
		;
	}
}
ISR(USART0_RX_vect) {
	u8Data = USART0_vReceiveByte();
	clr_LCD();
	print_LCD("DATA: ");
	move_LCD(1,6);
	sprintf(a_buffer, "%d", u8Data);
	putChar_LCD(u8Data);
	if(u8Data=='a')  /*sending 'a' from PC*/
	{
		PORT_LED_O=(1<<BIT_LED_O);
		clr_LCD();
		print_LCD("DATA: ");
		move_LCD(1,6);
		sprintf(a_buffer, "%d", u8Data);
		putChar_LCD(u8Data);
		move_LCD(2,6);
	}
	if(u8Data=='b') /*sending 'b' from PC*/
	{
		PORT_LED_O&=~(1<<BIT_LED_O);
		clr_LCD();
		print_LCD("DATA: ");
		move_LCD(1,6);
		sprintf(a_buffer, "%d", u8Data);
		putChar_LCD(u8Data);
		move_LCD(2,6);
	}
}