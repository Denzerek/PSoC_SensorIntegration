/*
 * lcdDriver.c
 *
 *  Created on: 16-Mar-2022
 *      Author: AustinA
 */




#include "lcdDriver.h"
#include "newHavenLCD.h"
void setPixel(uint8_t x,uint8_t y)
{

	if(y > 63 || x > 128)
	{
		return;
	}

	LCDHalfSelect(LCD_HALF_1);
	displayState(DISPLAY_ON);
	LCDHalfSelect(LCD_HALF_2);
	displayState(DISPLAY_ON);

    y = 63 - y;

	if(x < 64)
	{
		LCDHalfSelect(LCD_HALF_1);
	}
	else if(x > 63 && x < 128)
	{
		LCDHalfSelect(LCD_HALF_2);
	}
	setXAddress((y==0)?0:((y/8)));
	
	setYAddress((x>63)?(x-63):x);
	
	volatile uint8_t pixel = 0;
	lcdDataRead(&pixel);
	lcdDataRead(&pixel);
	setYAddress((x>63)?(x-63):x);
	pixel |= 0x01 << (( (y % 8)));
	lcdDataWrite(pixel);

}


void setAllPixelsTo(uint8_t data)
{
	for(int j = 0; j< 2;j++)
	{
		LCDHalfSelect(j);
		displayState(DISPLAY_OFF);
	}
	for(int j = 0; j< 2;j++)
	{
		LCDHalfSelect(j);
		for(int k = 0;k<8;k++)
		{;
			setXAddress(k);
			setYAddress(0);
			for(int i = 0;i<64 ;i++)
			{
				lcdDataWrite(data);
			}
		}
	}
	
	for(int j = 0; j< 2;j++)
	{
		LCDHalfSelect(j);
		displayState(DISPLAY_ON);
	}
}

void clearLCD()
{
	setAllPixelsTo(0x00);
}
