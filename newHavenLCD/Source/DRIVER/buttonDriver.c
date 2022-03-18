/*
 * buttonDriver.c
 *
 *  Created on: 18-Mar-2022
 *      Author: AustinA
 */




#include "buttonDriver.h"


uint8_t buttonPressed = false;
void userButtonISR()
{
	buttonPressed = true;
	clearButtonInterrupt();
}

void userButtonInit()
{
	buttonInit(&userButtonISR);
}

uint8_t getButtonPressStatus()
{
	uint8_t temp = buttonPressed;
	buttonPressed = false;
	return temp;
}
