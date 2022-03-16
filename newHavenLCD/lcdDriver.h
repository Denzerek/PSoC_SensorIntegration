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

void setPixel(uint8_t x,uint8_t y);
void clearLCD();


#endif /* LCDDRIVER_H_ */
