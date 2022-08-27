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


static volatile uint8_t glcdContextX;
static volatile uint8_t glcdContextY;
static volatile lcdHalf_e glcdContextHalf;
static volatile uint8_t glcdContextBuf1[LCD_ROW_SIZE][LCD_PAGE_SIZE+1];
static volatile uint8_t glcdContextBuf2[LCD_ROW_SIZE][LCD_PAGE_SIZE+1];




void setLcdXaddress(uint8_t page)
{
	lcdGPIO_setXAddress(page);
	glcdContextX = page;
}


void setLCDYAddress(uint8_t line)
{
	lcdGPIO_setYAddress(line);
	glcdContextY = line;
}

uint8_t getLCDData(uint8_t x, uint8_t y,lcdHalf_e halfselect)
{
	uint8_t readData;
	switch(halfselect)
	{
	case LCD_HALF_1:
		readData = glcdContextBuf1[x][y] ;
		break;
	case LCD_HALF_2:
		readData = glcdContextBuf2[x][y] ;
		break;
	}
	return readData;

}


uint8_t getLCDContexData()
{
	uint8_t readData = 0;
	switch(glcdContextHalf)
	{
	case LCD_HALF_1:
		readData = glcdContextBuf1[glcdContextX][glcdContextY] ;
		break;
	case LCD_HALF_2:
		readData = glcdContextBuf2[glcdContextX][glcdContextY] ;
		break;
	}
	return readData;
}

void setLCDData(uint8_t writeData)
{
	lcdGPIO_DataWrite(writeData);
	switch(glcdContextHalf)
	{
	case LCD_HALF_1:
		glcdContextBuf1[glcdContextX][glcdContextY] = writeData;
		break;
	case LCD_HALF_2:
		glcdContextBuf2[glcdContextX][glcdContextY] = writeData;
		break;
	}
	glcdContextY++;
	if(glcdContextY > 63)
	{
		glcdContextY = 0;
	}
}


void setLCDHalf(lcdHalf_e lcdHalf)
{
	lcdGPIO_halfSelect(lcdHalf);
	glcdContextHalf = lcdHalf;
}



void printContinuousBytes(uint8_t *array)
{
	uint8_t pageSelect = 0;
	uint32_t arrayCounter = 0;
	uint8_t xaddress = 0,yaddress = 0;
	lcdPage1Select();
	setLcdXaddress(xaddress);
	setLCDYAddress(yaddress);
	for(int i = 0; i < 16 ; i++)
	{
		for(int j = 0; j < 64; j++)
		{
			setLCDData(array[arrayCounter++]);
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
		setLCDYAddress(0);
		setLcdXaddress(xaddress);
		pageSelect = !pageSelect;
	}

}

void LCDPixelManipulate(uint8_t x,uint8_t y,pixelState_e pixelState)
{

	if(y > 63 || x > 128)
	{
		return;
	}

    y = 63 - y;
	uint8_t yaddress = (x>(LCD_PAGE_SIZE))?(x-LCD_PAGE_SIZE -1):x;
	uint8_t xaddress = (y==0)?0:((y/8));


	volatile uint8_t pixel = 0, halfselect;
	if(x <= 63)
	{
		halfselect = LCD_HALF_1;
	}
	if(x > 63 && x < 128)
	{
		halfselect = LCD_HALF_2;
	}

	/*
	 *
	 * pixel to set = 1<<4  // 0001 0000
	 * pixel = 1100 0100
	 *
	 * nptoset= 0001 0000
	 * npixel = 0010 1011
	 */
#define SPEED_BOOSTER
#ifdef SPEED_BOOSTER
	uint8_t pixelToSet =  0x01 << (y%8);


	pixel = getLCDData(xaddress,yaddress,halfselect);

	switch(pixelState)
	{

	case CLEAR_PIXEL:
		if((pixelToSet & ~(pixel)) == pixelToSet)
			return;

		pixel = ~(~(pixel) | pixelToSet);
		break;

	case SET_PIXEL:
		if((pixelToSet & pixel) == pixelToSet)
			return;
		pixel |= pixelToSet;
		break;

	}
#endif
	setLCDHalf(halfselect);

	setLcdXaddress(xaddress);

	setLCDYAddress(yaddress);

	setLCDData(pixel);

}




void setAllPixelsTo(uint8_t data)
{
	for(int j = 0; j< 2;j++)
	{
		setLCDHalf(j);

		lcdGPIO_Display_on_off(DISPLAY_OFF);
	}
	for(int j = 0; j< 2;j++)
	{
		setLCDHalf(j);
		for(int k = 0;k<8;k++)
		{;
			setLcdXaddress(k);
			setLCDYAddress(0);
			for(int i = 0;i<64 ;i++)
			{
				setLCDData(data);
			}
		}
	}

	for(int j = 0; j< 2;j++)
	{
		setLCDHalf(j);
		lcdGPIO_Display_on_off(DISPLAY_ON);
	}
}

void clearLCD()
{
	setAllPixelsTo(0x00);
}


