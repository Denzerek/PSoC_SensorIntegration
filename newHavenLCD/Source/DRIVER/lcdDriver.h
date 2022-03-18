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
#include "newHavenLCD.h"

#define lcdPage1Select()	setLCDHalf(LCD_HALF_1);
#define lcdPage2Select()	setLCDHalf(LCD_HALF_2);

void setLcdXaddress(uint8_t page);
void setLCDYAddress(uint8_t page);
uint8_t getLCDData(uint8_t x, uint8_t y,lcdHalf_e halfselect);
void setLCDData(uint8_t writeData);
void setLCDHalf(lcdHalf_e lcdHalf);


uint8_t getLCDContexData();
void setPixel(uint8_t x,uint8_t y);
void clearLCD();
void clearPixel(uint8_t x,uint8_t y);

void printContinuousBytes(uint8_t *array);

#endif /* LCDDRIVER_H_ */
