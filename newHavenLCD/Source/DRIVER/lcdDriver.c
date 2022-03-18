/*
 * lcdDriver.c
 *
 *  Created on: 16-Mar-2022
 *      Author: AustinA
 */




#include "lcdDriver.h"


#define LCD_PAGE_SIZE	63
#define LCD_COL_SIZE	128
#define LCD_ROW_SIZE	8
static volatile uint8_t glcdContextBuffer[LCD_ROW_SIZE][LCD_COL_SIZE];


void printContinuousBytes(uint8_t *array)
{
	uint8_t pageSelect = 0;
	uint32_t arrayCounter = 0;
	uint8_t xaddress = 0,yaddress = 0;
	lcdPage1Select();
	setXAddress(xaddress);
	setYAddress(yaddress);
	for(int i = 0; i < 16 ; i++)
	{
		for(int j = 0; j < 64; j++)
		{
			lcdDataWrite(array[arrayCounter++]);
		}
		if(((i+1)%2 ) == 0)
		{
			xaddress++;
		}
		if(pageSelect)
		{
			lcdPage1Select();
		}
		else
		{
			lcdPage2Select();
		}
		setYAddress(0);
		setXAddress(xaddress);
		pageSelect = !pageSelect;
	}

}

void setPixel(uint8_t x,uint8_t y)
{

	if(y > 63 || x > 128)
	{
		return;
	}

    y = 63 - y;
	uint8_t yaddress = (x>(LCD_PAGE_SIZE))?(x-LCD_PAGE_SIZE -1):x;
	uint8_t xaddress = (y==0)?0:((y/8));

#define SPEED_BOOSTER
#ifdef SPEED_BOOSTER
	uint8_t pixelToSet =  0x01 << (y%8);
	if((glcdContextBuffer[xaddress][x] & pixelToSet) == pixelToSet)
	{
		return;
	}
#endif

	if(x <= 63)
	{
		LCDHalfSelect(LCD_HALF_1);
	}
	if(x > 63 && x < 128)
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
