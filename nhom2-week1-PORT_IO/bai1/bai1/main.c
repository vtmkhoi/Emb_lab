/*
 * Calender_ver01.c
 *This version used ATMEGA128, display on LCD
 * Created: 8/29/2021 8:04:03 PM
 * Author : PC
 */ 
#define F_CPU 7372800UL

#include <avr/io.h>
#include <I2C_Master_H_file.h>
#include <util/delay.h>
#include <avr/myLCD.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define Device_Write_address 0xD0
#define Device_Read_address	0xD1
#define TimeFormat12	0x40
#define ampm	0x20

int second, minute, hour, day, date, month, year;

bool isitPM(char hour_)
{
	if (hour_ & (ampm))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*
FUNCTION FOR CLOCK
*/
void RTC_Clock_Write(char _hour, char _minute, char _second, char AMPM)
{
	_hour |= ampm;
	I2C_Start(Device_Write_address); //Start I2C communication
	I2C_Write(0);
	I2C_Write(_second);
	I2C_Write(_minute);
	I2C_Write(_hour);
	I2C_Stop();
}

/*
FUNCTION FOR CALENDAR
*/
void RTC_Calendar_Write(char _day, char _date, char _month, char _year)
{
	I2C_Start(Device_Write_address);
	I2C_Write(3);
	I2C_Write(_day);
	I2C_Write(_date);
	I2C_Write(_month);
	I2C_Write(_year);
	I2C_Stop();
}

/*
FUNCTION READ CLOCK
*/
void RTC_Clock_Read(char read_clock_address)
{
	I2C_Start(Device_Write_address);
	I2C_Write(read_clock_address);
	I2C_Repeated_Start(Device_Read_address);
	
	second = I2C_Read_Ack();
	minute = I2C_Read_Ack();
	hour   = I2C_Read_Ack();
	I2C_Stop();
}

void RTC_Calendar_Read(char read_calendar_address)
{
	I2C_Start(Device_Write_address);
	I2C_Write(read_calendar_address);
	I2C_Repeated_Start(Device_Read_address);
	
	day = I2C_Read_Ack();
	date = I2C_Read_Ack();
	month   = I2C_Read_Ack();
	year = I2C_Read_Ack();
	I2C_Stop();
}


int main(void)
{
	char buffer[20];
	char* days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	I2C_Init();
	init_LCD();
    while (1) 
    {
		RTC_Clock_Read(0);
		if (hour & TimeFormat12)
		{
			sprintf(buffer, "%02x:%02x:%02x  ", (hour & 0b00011111), minute, second);
			if(isitPM(hour))
			strcat(buffer, "PM");
			else
			strcat(buffer, "AM");
			print_LCD(buffer);
		}
		else
		{
			sprintf(buffer, "%02x:%02x:%02x  ", (hour & 0b00011111), minute, second);
			print_LCD(buffer);
		}
		RTC_Calendar_Read(3);
		sprintf(buffer, "%02x/%02x/%02x %3s ", date, month, year,days[day-1]);
		print_LCD(buffer);
    }
}

