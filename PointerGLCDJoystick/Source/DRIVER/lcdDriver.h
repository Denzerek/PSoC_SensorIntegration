/*
 * lcdDriver.h
 *
 *  Created on: 16-Mar-2022
 *      Author: AustinA
 */

#ifndef LCDDRIVER_H_
#define LCDDRIVER_H_


#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "lcdGPIO.h"


typedef enum{
	CLEAR_PIXEL,
	SET_PIXEL
}pixelState_e;


#define setLCDPixel(x,y)	LCDPixelManipulate( x, y,SET_PIXEL)
#define clearLCDPixel(x,y)	LCDPixelManipulate( x, y,CLEAR_PIXEL)



#define lcdPage1Select()	setLCDHalf(LCD_HALF_1);
#define lcdPage2Select()	setLCDHalf(LCD_HALF_2);

void setLcdXaddress(uint8_t page);
void setLCDYAddress(uint8_t page);
uint8_t getLCDData(uint8_t x, uint8_t y,lcdHalf_e halfselect);
void setLCDData(uint8_t writeData);
void setLCDHalf(lcdHalf_e lcdHalf);


uint8_t getLCDContexData();
void LCDPixelManipulate(uint8_t x,uint8_t y,pixelState_e clear);
void setAllPixelsTo(uint8_t data);

void printContinuousBytes(uint8_t *array);

void clearLCD();

#endif /* LCDDRIVER_H_ */
