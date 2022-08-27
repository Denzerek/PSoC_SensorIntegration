/*
 * LCD_ScreenControl.c
 *
 *  Created on: 19-Mar-2022
 *      Author: AustinA
 */


#include "LCD_ScreenControl.h"
#include "lcdScreens.h"
#include "buttonDriver.h"
#include "timerDriver.h"
#include "glcdShapes.h"


typedef enum{
	LCD_SPLASH,
	LCD_IDLE,
	LCD_BORDER_SET,
	LCD_SWITCH_SCREEN,
	LCD_SCREEN_UPDATE,
	LCD_SCREEN_UPDATE_START,
	LCD_SCREEN_UPDATE_WAIT

}lcdScreenState_e;

lcdScreenState_e lcdScreenState = LCD_SPLASH;
void ScreenDisplay_Task()
{

	drawBorder(0);

    switch(lcdScreenState)
    {
    case LCD_IDLE:
    	if(getButtonPressStatus())
        	lcdScreenState = LCD_SWITCH_SCREEN;
		else
		if(lcdScreenUpdateRequired())
			lcdScreenState = LCD_SCREEN_UPDATE;

    	break;
    case LCD_SPLASH:
    	InfineonLogoScreen();
    	lcdScreenState = LCD_BORDER_SET;
    	break;

    case LCD_BORDER_SET:
    	drawBorder(0);
    	lcdScreenState = LCD_IDLE;
		break;

    case LCD_SWITCH_SCREEN:
        clearLCD();
    	lcdSwitchScreens();
    	lcdScreenState = LCD_SCREEN_UPDATE;
    	break;

	case LCD_SCREEN_UPDATE:
		updateCurrentScreen();
    	lcdScreenState = LCD_SCREEN_UPDATE_START;
		break;

	case LCD_SCREEN_UPDATE_START:
		startScreenUpdateTimer();
    	lcdScreenState = LCD_SCREEN_UPDATE_WAIT;
		break;

	case LCD_SCREEN_UPDATE_WAIT:
		if(getScreenUpdateTimerStatus())
			lcdScreenState = LCD_BORDER_SET;	
	break;

    }
}
