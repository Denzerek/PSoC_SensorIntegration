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


typedef enum{
	SCREEN_MIN,
	fontDemo_n,
	InfineonLogoScreen_n,
	ConcentriRectangleDemo_n,
	CirclesDemo_n,
	BarGrsphDemo_n,
	lcdOverWritePrintDemo_n,
	lcdOverWriteInvertPrintDemo_n,
	tempPressureMonitor_n,
	SCREEN_MAX
}lcdScreens_n;

typedef struct{
	uint8_t currentScreen;
	void (*screenHandler)();
	bool screenUpdate;
}lcdScreens_s;

lcdScreens_s lcdScreens[] = {
		{InfineonLogoScreen_n,InfineonLogoScreen,false},
		{fontDemo_n,fontDemo,false},
		{tempPressureMonitor_n,tempPressureMonitor,true},
		{ConcentriRectangleDemo_n,ConcentriRectangleDemo,false},
		{CirclesDemo_n,CirclesDemo,false},
		{BarGrsphDemo_n,BarGrsphDemo,false},
		{lcdOverWritePrintDemo_n,lcdOverWritePrintDemo,false},
		{lcdOverWriteInvertPrintDemo_n,lcdOverWriteInvertPrintDemo,false},
};




static lcdScreens_n currentScreen;

void lcdSwitchScreens()
{
	currentScreen ++;
	if(currentScreen == SCREEN_MAX - 1)
		currentScreen = SCREEN_MIN + 1;
}


bool lcdScreenUpdateRequired()
{
	return lcdScreens[currentScreen].screenUpdate;
}

void updateCurrentScreen()
{
	lcdScreens[currentScreen].screenHandler();
}


void lcdOverWriteInvertPrintDemo()
{
	setAllPixelsTo(0xFF);
	lcdPrintLineI("This string inverts the background of the string its written",LCD_LINE_4);
}

void lcdOverWritePrintDemo()
{
	ConcentriRectangleDemo();

	lcdPrintLineOver("    Demonstrating string write over an image",LCD_LINE_2);

	lcdPrintLine("    This string does not write over image",LCD_LINE_5);
}

void ConcentriRectangleDemo()
{

    drawBorder(0);
    drawBorder(8);
    drawBorder(16);
    drawBorder(24);
    drawBorder(35);
}


void tempPressureMonitor()
{
	static uint8_t pressure,temperature;
	drawBorder(0);

	drawBorder(3);
	
	drawBarGraphY(32, 4,30, 55,pressure);
	drawBarGraphY(32+64, 4,30, 55,temperature);
	pressure = 1 + pressure%50;
	temperature = 1 + temperature%50;


	lcdPrintLineIAt(" KPa",32 - 13,LCD_LINE_2);
	lcdPrintLineIAt("DegreeC",32+57 - 14,LCD_LINE_2);


}

void InfineonLogoScreen()
{
	extern const unsigned char infineonSplash [];

    printContinuousBytes(infineonSplash);
}


void fontDemo()
{
	lcdPrintLine("This is a demo  \n\nDemonstratin of GLCD\n \n\n!!<>?@#", LCD_LINE_2);
}

void BarGrsphDemo()
{
	drawBarGraph(6,3,20);


	drawBarGraph(16,5,50);


	drawBarGraph(40,10,63);


	drawBarGraph(80,20,40);


	drawBarGraphY(120,6,15,60,30);
}

void CirclesDemo()
{
	
	drawCircle(1,32,1);

	drawCircle(7,32,5);


	drawCircle(26,32,10);


	drawCircle(59,32,20);


	drawCircle(100,32,20);
}


