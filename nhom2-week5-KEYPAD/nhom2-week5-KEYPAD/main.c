/*
 * TH5_KeyPad_EEPROM.c
 *
 * Created: 04/07/2020 07.52.04
 * Author : ACER
 */ 

#define F_CPU	7372800UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/myLCD.h>
#include <avr/interrupt.h>

#define KEYPAD_DDR      DDRC
#define KEYPAD_PORT     PORTC
#define KEYPAD_PIN      PINC

#define PORT_BUZ_O      PORTB
#define DDR_BUZ_O       DDRB
#define BIT_BUZ_0		7


uint8_t scan_code[4]={0x0E,0x0D,0x0B,0x07};
uint8_t ascii_code[4][4]={  '7','8','9','/',
                            '4','5','6','*',
                            '1','2','3','-',
                            'N','0','=','+'};

uint8_t key;
uint8_t checkpad();

//doan chuong trinh con doc keypad-------------------------------------------------------------------
uint8_t checkpad(){
	uint8_t i,j,keyin;
	for(i=0;i<4;i++){              
			KEYPAD_PORT=0xFF-(1<<(i+4)); 
			_delay_us(10);
			keyin=KEYPAD_PIN & 0x0F;
			if(keyin!=0x0F)
					for(j=0;j<4;j++)
							if(keyin==scan_code[j])
							{
								return ascii_code[j][i];
							}

	}        
	return 0;
}
uint8_t EE_u8ReadByte(uint16_t u16Address)
{
	// Wait until EEPROM is ready
	while(EECR&(1<<EEWE))
	{
		;
	}
	// Set address
	EEARL = u16Address&0xff;
	EEARH = (u16Address>>8)&0xff;
	// Start read operation
	EECR |= (1<<EERE);
	// Return data
	return EEDR;
}

void EE_vWriteByte(uint16_t u16Address, uint8_t u8Data)
{
	uint8_t u8Sreg;

	// Wait until EEPROM is ready
	while(EECR&(1<<EEWE))
	{
		;
	}
	// Set address
	EEARL = u16Address&0xff;
	EEARH = (u16Address>>8)&0xff;
	// Set data to write
	EEDR = u8Data;
	// Save status of global interrupts
	u8Sreg = SREG;
	// Disable interrupts
	cli();
	// Start write operation
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
	// Restore status of global interrupts
	SREG = u8Sreg;
}

void unPower()
{
	uint8_t u8Data;
	u8Data = EE_u8ReadByte(0x000C);
	move_LCD(1,8);
	putChar_LCD(u8Data);
}

int main(){
	//---khai bao huong cho cac chan ket noi keypad
	KEYPAD_PORT=0x0F;                           //*
	KEYPAD_DDR=0xF0;                            //*
	//*********************************************
	init_LCD(); 
	clr_LCD();
	DDR_BUZ_O |= (1<<BIT_BUZ_0);
	
	uint8_t u8Data;
	u8Data = EE_u8ReadByte(0x000C);
	move_LCD(1,8);
	putChar_LCD(u8Data);
	while(1)
	{
		key=checkpad(); //doc keypad
		PORT_BUZ_O |= (1<<BIT_BUZ_0);
		
		if(key)		//
		{ 		
			if(key != 0x3D)
			{
				move_LCD(1,1);
				putChar_LCD(key);
				EE_vWriteByte(0x000C,key);
			}
			if(key == 0x3D)
			{
				u8Data = EE_u8ReadByte(0x000C);
				move_LCD(1,8);
				putChar_LCD(u8Data);
				PORT_BUZ_O &= ~(1<<BIT_BUZ_0);
				_delay_ms(1000);
				PORT_BUZ_O |= (1<<BIT_BUZ_0);
			}
		}
	};
}