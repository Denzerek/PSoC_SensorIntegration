/*
 * lcdScreens.h
 *
 *  Created on: 18-Mar-2022
 *      Author: AustinA
 */

#ifndef SOURCE_APPLICATION_LCDSCREENS_H_
#define SOURCE_APPLICATION_LCDSCREENS_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"



void lcdSwitchScreens();
bool lcdScreenUpdateRequired();
void updateCurrentScreen();

void CirclesDemo();
void InfineonLogoScreen();
void ConcentriRectangleDemo();
void BarGrsphDemo();
void fontDemo();
void lcdOverWritePrintDemo();
void lcdOverWriteInvertPrintDemo();
void tempPressureMonitor();

#endif /* SOURCE_APPLICATION_LCDSCREENS_H_ */
