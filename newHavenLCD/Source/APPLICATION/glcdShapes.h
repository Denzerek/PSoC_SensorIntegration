/*
 * glcdShapes.h
 *
 *  Created on: 16-Mar-2022
 *      Author: AustinA
 */

#ifndef GLCDSHAPES_H_
#define GLCDSHAPES_H_


#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "lcdDriver.h"

void drawCircle(int cx, int cy,int r);
void drawBorder(uint8_t offset);
void drawBarGraph(uint8_t x,uint8_t width,uint8_t height);
void drawBarGraphY(uint8_t x,uint8_t y,uint8_t width,uint8_t height);

#endif /* GLCDSHAPES_H_ */
