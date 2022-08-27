/*
 * lcdScreens.c
 *
 *  Created on: 18-Mar-2022
 *      Author: AustinA
 */




#include "lcdScreens.h"
#include "glcdShapes.h"
#include "glcdPrints.h"
#include "lcdDriver.h"
#include "timerDriver.h"
#include "joyStickTask.h"


static lcdScreens_n currentScreen;
screenStates_t screenStateAfterSwitch;
char tempBuff[100];



lcdScreens_s lcdScreens[] = {
		{InfineonLogoScreen_n,InfineonLogoScreen,false},
		{joystickCalibration_n,joystickCalibration,true},
};





void lcdSwitchScreens()
{
	currentScreen ++;
	if(currentScreen == ((sizeof(lcdScreens)/sizeof(lcdScreens_s))))
		currentScreen = SCREEN_MIN + 1;
	screenStateAfterSwitch = SCREEN_INIT;
}


bool lcdScreenUpdateRequired()
{
	return lcdScreens[currentScreen].screenUpdate;
}

void updateCurrentScreen()
{
	lcdScreens[currentScreen].screenHandler();
}


void InfineonLogoScreen()
{
	extern unsigned char infineonSplash [];

    printContinuousBytes(infineonSplash);
}


void joystickCalibration()
{
	switch(screenStateAfterSwitch)
	{
		case SCREEN_INIT:
			lcdPrintLine(" RAW:  X      Y    ", 1);
			lcdPrintLine(" CAL:  X      Y    ", 4);
			screenStateAfterSwitch = SCREEN_UPDATE;
			joyStickCalibrate();
			break;
		case SCREEN_UPDATE:
			sprintf(tempBuff,"     %4d    %4d    ", getJoystickRaw_X(),getJoystickRaw_Y());
			lcdPrintLine(tempBuff, 2);

			sprintf(tempBuff,"     %4d    %4d    ", getJoystickX(),getJoystickY());
			lcdPrintLine(tempBuff, 5);
			break;
	}
	
}



