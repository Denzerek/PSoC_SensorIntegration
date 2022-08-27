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


typedef enum{
	SCREEN_INIT,
	SCREEN_UPDATE,
}screenStates_t;

typedef enum{
	SCREEN_MIN,
	InfineonLogoScreen_n,
	joystickCalibration_n,
	SCREEN_MAX
}lcdScreens_n;

typedef struct{
	uint8_t currentScreen;
	void (*screenHandler)();
	bool screenUpdate;
}lcdScreens_s;

void lcdSwitchScreens();
bool lcdScreenUpdateRequired();
void updateCurrentScreen();

void joystickCalibration();
void InfineonLogoScreen();

#endif /* SOURCE_APPLICATION_LCDSCREENS_H_ */
