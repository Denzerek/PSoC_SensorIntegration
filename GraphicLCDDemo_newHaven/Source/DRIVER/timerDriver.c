/*
 * timerDriver.c
 *
 *  Created on: 19-Mar-2022
 *      Author: AustinA
 */





#include "timerDriver.h"


#define HEARTBEAT_TIME	100
static volatile uint32_t heartBeatCounter;
static volatile bool heartBeatFlag = false;


#define LCD_SCREEN_UPDATE_TIMER	30
static volatile uint32_t screenUpdateCounter;
static volatile bool screenUpdateCounterFlag = false;



#define RANDOME_TIMER	300
static volatile uint32_t randomCounter;
static volatile bool randomCounterFlag = true;


void timerHandler()
{
	clearTimerInterrupt(1);


	heartBeatCounter++;
	if(heartBeatCounter > (HEARTBEAT_TIME * 1000))
	{
		heartBeatCounter = 0;
		Cy_GPIO_Write(HEARTBEAT_PORT, HEARTBEAT_PIN, !Cy_GPIO_Read(HEARTBEAT_PORT, HEARTBEAT_PIN));
	}

	if(!screenUpdateCounterFlag)
		screenUpdateCounter--;
	if(!screenUpdateCounter)
		screenUpdateCounterFlag = true;

		
	if(!screenUpdateCounterFlag)
		screenUpdateCounter--;
	if(!screenUpdateCounter)
		screenUpdateCounterFlag = true;


		
	if(!randomCounterFlag)
		randomCounter--;
	if(!randomCounter)
		randomCounterFlag = true;
}

void startrandomTimer(uint32_t timeInMs)
{
	randomCounter = timeInMs * 1000;
	randomCounterFlag = false;
}

bool getrandomTimerStatus()
{
	return randomCounterFlag;
}

void startScreenUpdateTimer()
{
	screenUpdateCounter = LCD_SCREEN_UPDATE_TIMER * 1000;
	screenUpdateCounterFlag = false;
}

bool getScreenUpdateTimerStatus()
{
	return screenUpdateCounterFlag;
}


void timerDriver_Init()
{
	timerInit(&timerHandler);
}
