/*
 * lcdGPIO.c
 *
 *  Created on: 15-Mar-2022
 *      Author: AustinA
 */




#include "lcdGPIO.h"



static void lcdGPIO_InstrSet(uint8_t state);
static void lcdGPIO_GPIOPortWrite(uint8_t data);



void lcdGPIO_cmdWrite( uint8_t cmd)
{
	lcdGPIO_GPIOPortWrite(cmd);
    CLEAR_RS();
    CLEAR_RW();
    SET_LCD_E();
	Cy_SysLib_DelayUs(1);
    CLEAR_LCD_E();
}



static void lcdGPIO_GPIOPortWrite(uint8_t data)
{
	Cy_GPIO_Write(DB0_PORT, DB0_PIN,( data & 0x01) >> 0);
	Cy_GPIO_Write(DB1_PORT, DB1_PIN,( data & 0x02) >> 1);
	Cy_GPIO_Write(DB2_PORT, DB2_PIN,( data & 0x04) >> 2);
	Cy_GPIO_Write(DB3_PORT, DB3_PIN,( data & 0x08) >> 3);
	Cy_GPIO_Write(DB4_PORT, DB4_PIN,( data & 0x10) >> 4);
	Cy_GPIO_Write(DB5_PORT, DB5_PIN,( data & 0x20) >> 5);
	Cy_GPIO_Write(DB6_PORT, DB6_PIN,( data & 0x40) >> 6);
	Cy_GPIO_Write(DB7_PORT, DB7_PIN,( data & 0x80) >> 7);
}


static void lcdGPIO_InstrSet(uint8_t state)
{
	CLEAR_LCD_E();
	LCD_DELAY(22);
	CLEAR_RS();
	CLEAR_RW();
	LCD_DELAY(14);
	SET_LCD_E();
	LCD_DELAY(5);
	lcdGPIO_GPIOPortWrite(state);
	LCD_DELAY(21);
	CLEAR_LCD_E();

}

void lcdGPIO_DataWrite(uint8_t writeData)
{
	CLEAR_LCD_E();
	LCD_DELAY(22);
	SET_RS();
	CLEAR_RW();
	LCD_DELAY(14);
	SET_LCD_E();
	LCD_DELAY(5);
	lcdGPIO_GPIOPortWrite(writeData);
	LCD_DELAY(21);
	CLEAR_LCD_E();
}





void lcdGPIO_Display_on_off(displayState_e displayState)
{
	uint8_t state = 0x3E;
	state |= (displayState & 0x01);
	lcdGPIO_InstrSet(state);

}

void setDisplayStartLine(uint8_t lineNo)
{
	if(lineNo > 63)
	{
		/*Invalid Y address */
		lineNo = 0;
	}

	uint8_t state = 0xC0;
	state |= (lineNo & 0x3F);

	lcdGPIO_InstrSet(state);
}


void lcdGPIO_setXAddress(uint8_t page)
{
	if(page > 7)
	{
		/*Invalid Y address */
		page = 0;
	}

	uint8_t state = 0xB8;
	state |= (page & 0x07);

	lcdGPIO_InstrSet(state);

}

void lcdGPIO_setYAddress(uint8_t address)
{
	if(address > 63)
	{
		/*Invalid Y address */
		address = 0;
	}

	uint8_t state = 0x40;
	state |= (address & 0x3F);

	lcdGPIO_InstrSet(state);

}

void lcdGPIO_halfSelect(lcdHalf_e halfSelect)
{
	switch(halfSelect)
	{
		case LCD_HALF_1:
			CLEAR_CS2();
			SET_CS1();
			break;
		case LCD_HALF_2:
			CLEAR_CS1();
			SET_CS2();
			break;
		default:
			break;
	}
}


void LCDDisplayBars()
{
	uint8_t togg = 0;

	for(int k = 0;k<8;k++)
	{
		lcdGPIO_setXAddress(k);
		for(int i = 0;i<64 ;)
		{
			lcdGPIO_setYAddress(i);
			for(int j=0;j<4;j++)
			{
				if(togg)
				{
					lcdGPIO_DataWrite(0xFF);
				}
				else
				{
					lcdGPIO_DataWrite(0x00);
				}
				i++;
			}
			togg = !togg;
		}
	}

}


void LCDDisplayTest()
{
	lcdGPIO_halfSelect(LCD_HALF_1);
	lcdGPIO_Display_on_off(DISPLAY_OFF);
	LCDDisplayBars();

	lcdGPIO_halfSelect(LCD_HALF_2);
	lcdGPIO_Display_on_off(DISPLAY_OFF);
	LCDDisplayBars();

	lcdGPIO_halfSelect(LCD_HALF_1);
	lcdGPIO_Display_on_off(DISPLAY_ON);
	lcdGPIO_halfSelect(LCD_HALF_2);
	lcdGPIO_Display_on_off(DISPLAY_ON);
}



void lcdGPIO_Init()
{
	SET_RES();

	LCDDisplayTest();
	
}