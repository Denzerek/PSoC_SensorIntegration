/*
 * newHavenLCD.c
 *
 *  Created on: 15-Mar-2022
 *      Author: AustinA
 */




#include "newHavenLCD.h"


static void lcdInstructionSet(uint8_t state);
static void byteWrite(uint8_t data);

static void byteWrite(uint8_t data)
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



static void lcdInstructionSet(uint8_t state)
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


typedef enum{
	PIN_MODE_INPUT,
	PIN_MODE_OUTPUT,
}lcdDataPinState_e;

lcdDataPinState_e lcdDataPinState = PIN_MODE_INPUT;


void dataPinsStateCheckSet(lcdDataPinState_e setStateTo)
{
	if(setStateTo == lcdDataPinState)
		return;

	switch(setStateTo)
	{
		case PIN_MODE_INPUT:
		break;
		case PIN_MODE_OUTPUT:
		break;
		default:
		break;
	}

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

void LCDHalfSelect(lcdHalf_e halfSelect)
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
		setXAddress(k);
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

}


void LCDDisplayTest()
{
	LCDHalfSelect(LCD_HALF_1);
	displayState(DISPLAY_OFF);
	LCDDisplayBars();

	LCDHalfSelect(LCD_HALF_2);
	displayState(DISPLAY_OFF);
	LCDDisplayBars();

	LCDHalfSelect(LCD_HALF_1);
	displayState(DISPLAY_ON);
	LCDHalfSelect(LCD_HALF_2);
	displayState(DISPLAY_ON);
}


static void byteRead(uint8_t *data)
{
	*data |= Cy_GPIO_Read(DB0_PORT,DB0_PIN) << 0;
	*data |= Cy_GPIO_Read(DB1_PORT,DB1_PIN) << 1;
	*data |= Cy_GPIO_Read(DB2_PORT,DB2_PIN) << 2;
	*data |= Cy_GPIO_Read(DB3_PORT,DB3_PIN) << 3;
	*data |= Cy_GPIO_Read(DB4_PORT,DB4_PIN) << 4;
	*data |= Cy_GPIO_Read(DB5_PORT,DB5_PIN) << 5;
	*data |= Cy_GPIO_Read(DB6_PORT,DB6_PIN) << 6;
	*data |= Cy_GPIO_Read(DB7_PORT,DB7_PIN) << 7;
}
#if 0


void lcdDataRead(uint8_t * readData1, uint8_t* readData2)
{
	CLEAR_LCD_E();
	LCD_DELAY(22);
	SET_RS();
	SET_RW();
	LCD_DELAY(14);
	SET_LCD_E();
	LCD_DELAY(20);
	byteRead(readData1);
	LCD_DELAY(20);
	CLEAR_LCD_E();
	LCD_DELAY(2);
	byteRead(readData2);
}


void lcdInit()
{
	SET_RES();

	LCDHalfSelect(LCD_HALF_1);
	displayState(DISPLAY_ON);
	setYAddress(0);
	setXAddress(0);
	lcdDataWrite(0xAA);
	lcdDataWrite(0x66);
	lcdDataWrite(0xAA);
	CyDelay(2000);
	volatile uint8_t data = 0;
	uint8_t test = 0;
	setYAddress(1);
	setXAddress(0);
    lcdDataRead(&test,&data);
    lcdDataRead(&test,&data);

	LCDDisplayTest();
	
}
#else
#if 1


void lcdStatusRead(uint8_t * readData)
{
	CLEAR_LCD_E();
	LCD_DELAY(22);
	SET_RS();
	CLEAR_RW();
	LCD_DELAY(14);
	SET_LCD_E();
	LCD_DELAY(50);
	CLEAR_LCD_E();
	LCD_DELAY(14);
	SET_LCD_E();
	LCD_DELAY(20);
	byteRead(readData);
	LCD_DELAY(20);
	CLEAR_LCD_E();
	LCD_DELAY(2);
}

void lcdDataRead(uint8_t * readData)
{
	CLEAR_LCD_E();
	LCD_DELAY(22);
	SET_RS();
	SET_RW();
	LCD_DELAY(14);
	SET_LCD_E();
	LCD_DELAY(50);
	CLEAR_LCD_E();
	LCD_DELAY(14);
	SET_LCD_E();
	LCD_DELAY(20);
	byteRead(readData);
	LCD_DELAY(20);
	CLEAR_LCD_E();
	LCD_DELAY(2);
}

// void lcdDataRead(uint8_t * readData1)
// {
// 	CLEAR_LCD_E();
// 	LCD_DELAY(22);
// 	SET_RS();
// 	SET_RW();
// 	LCD_DELAY(14);
// 	SET_LCD_E();
// 	LCD_DELAY(20);
// 	byteRead(readData1);
// 	LCD_DELAY(20);
// 	CLEAR_LCD_E();
// 	LCD_DELAY(2);
// }


void lcdInit()
{
	SET_RES();


	LCDDisplayTest();
	
}
#else

void lcdDataRead(uint8_t * readData)
{
	CLEAR_LCD_E();
	LCD_DELAY(22);
	SET_RS();
	SET_RW();
	LCD_DELAY(14);
	SET_LCD_E();
	LCD_DELAY(50);
	CLEAR_LCD_E();
	LCD_DELAY(14);
	SET_LCD_E();
	LCD_DELAY(20);
	byteRead(readData);
	LCD_DELAY(20);
	CLEAR_LCD_E();
	LCD_DELAY(2);
}


void lcdInit()
{
	SET_RES();

	LCDDisplayTest();
	
}
#endif
#endif
