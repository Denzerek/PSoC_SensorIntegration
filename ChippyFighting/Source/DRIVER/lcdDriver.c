/*
 * lcdDriver.c
 *
 *  Created on: 16-Mar-2022
 *      Author: AustinA
 */




#include "lcdDriver.h"
#include "newHavenLCD.h"


#define LCD_PAGE_SIZE	63
#define LCD_COL_SIZE	128
#define LCD_ROW_SIZE	8
static volatile uint8_t glcdContextBuffer[LCD_ROW_SIZE][LCD_COL_SIZE];



void setPixel(uint8_t x,uint8_t y)
{

	if(y > 63 || x > 127)
	{
		return;
	}

    y = 63 - y;
	uint8_t yaddress = (x>(LCD_PAGE_SIZE))?(x-LCD_PAGE_SIZE -1):x;
	uint8_t xaddress = (y==0)?0:((y/8));


	if(x <= 63)
	{
		LCDHalfSelect(LCD_HALF_1);
	}
	if(x > 63 && x < 127)
	{
		LCDHalfSelect(LCD_HALF_2);
	}
	setXAddress(xaddress);
	
	setYAddress(yaddress);
	
	volatile uint8_t pixel = 0;
	pixel = glcdContextBuffer[xaddress][x];
	pixel |= 0x01 << (y%8);
	lcdDataWrite(pixel);

	glcdContextBuffer[xaddress][x] = pixel;

}


void clearPixel(uint8_t x,uint8_t y)
{

	if(y > 63 || x > 127)
	{
		return;
	}

    y = 63 - y;
	uint8_t yaddress = (x>(LCD_PAGE_SIZE))?(x-LCD_PAGE_SIZE -1):x;
	uint8_t xaddress = (y==0)?0:((y/8));


	if(x <= 63)
	{
		LCDHalfSelect(LCD_HALF_1);
	}
	if(x > 63 && x < 127)
	{
		LCDHalfSelect(LCD_HALF_2);
	}
	setXAddress(xaddress);

	setYAddress(yaddress);

	volatile uint8_t pixel = 0;
	pixel = glcdContextBuffer[xaddress][x];
	pixel = !(!pixel | (0x01 << (y%8)));
	lcdDataWrite(pixel);

	glcdContextBuffer[xaddress][x] = pixel;

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
	memset(glcdContextBuffer,0x00,sizeof(glcdContextBuffer));
}
