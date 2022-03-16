/*
 * newHavenLCD.c
 *
 *  Created on: 15-Mar-2022
 *      Author: AustinA
 */




#include "newHavenLCD.h"

#define DELAY_FACTOR 10
#define LCD_DELAY(x)	for(int i = 0;i < (x + DELAY_FACTOR); i++)__asm volatile("NOP");


void byteWrite(uint8_t data)
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

typedef enum{
	DISPLAY_OFF,
	DISPLAY_ON,
}displayState_e;



void lcdInstructionSet(uint8_t state)
{
	CLEAR_LCD_E();
	CLEAR_RS();
	CLEAR_RW();
	CyDelay(1);
	SET_LCD_E();
	CyDelay(1);

	byteWrite(state);

	CyDelay(1);

	CLEAR_LCD_E();

}


void lcdDataWrite(uint8_t writeData)
{
	CLEAR_LCD_E();
	LCD_DELAY(22);
	SET_RS();
	CLEAR_RW();
	LCD_DELAY(14);
	SET_LCD_E();
	LCD_DELAY(5);

	byteWrite(writeData);
	LCD_DELAY(21);


	CLEAR_LCD_E();

}




void displayState(displayState_e displayState)
{
	uint8_t state = 0x3E;
	state |= (displayState & 0x01);
	lcdInstructionSet(state);

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

	lcdInstructionSet(state);
}


void setXAddress(uint8_t page)
{
	if(page > 7)
	{
		/*Invalid Y address */
		page = 0;
	}

	uint8_t state = 0xB8;
	state |= (page & 0x07);

	lcdInstructionSet(state);

}

void setYAddress(uint8_t address)
{
	if(address > 63)
	{
		/*Invalid Y address */
		address = 0;
	}

	uint8_t state = 0x40;
	state |= (address & 0x3F);

	lcdInstructionSet(state);

}


void lcdInit()
{
	SET_RES();

	CLEAR_CS2();
	SET_CS1();

	displayState(DISPLAY_ON);

	setXAddress(5);
	uint8_t togg = 0;

	for(int i = 0;i<64 ;)
	{
		setYAddress(i);
		for(int j=0;j<4;j++)
		{
			if(togg)
			{
				lcdDataWrite(0xFF);
			}
			else
			{
				lcdDataWrite(0x00);
			}
			i++;
		}
		togg = !togg;
	}





}
