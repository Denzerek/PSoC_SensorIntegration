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

#define lcdPage1Select()	LCDHalfSelect(LCD_HALF_1);
#define lcdPage2Select()	LCDHalfSelect(LCD_HALF_2);

void setPixel(uint8_t x,uint8_t y);
void clearLCD();
void clearPixel(uint8_t x,uint8_t y);

void printContinuousBytes(uint8_t *array);

#endif /* LCDDRIVER_H_ */
