/*
 * lcdScreens.c
 *
 *  Created on: 18-Mar-2022
 *      Author: AustinA
 */




#include "lcdScreens.h"
#include "glcdShapes.h"
#include "glcdPrints.h"


void CirclesDemo();
void InfineonLogoScreen();
void ConcentriRectangleDemo();
void BarGrsphDemo();
void fontDemo();
typedef enum{
	SCREEN_MIN,
	fontDemo_n,
	InfineonLogoScreen_n,
	ConcentriRectangleDemo_n,
	CirclesDemo_n,
	BarGrsphDemo_n,
	SCREEN_MAX
}lcdScreens_n;

typedef struct{
	uint8_t screenNum;
	void (*screenHandler)();
}lcdScreens_s;

lcdScreens_s lcdScreens[] = {
		{InfineonLogoScreen_n,InfineonLogoScreen},
		{fontDemo_n,fontDemo},
		{ConcentriRectangleDemo_n,ConcentriRectangleDemo},
		{CirclesDemo_n,CirclesDemo},
		{BarGrsphDemo_n,BarGrsphDemo},
};

void lcdSwitchScreens()
{
	static lcdScreens_n screenNum;

	if(screenNum < SCREEN_MAX)
	{
		lcdScreens[screenNum].screenHandler();
	}
	else
	{
		screenNum = SCREEN_MIN + 1;
		lcdScreens[screenNum].screenHandler();
	}
	screenNum++;
}


void ConcentriRectangleDemo()
{

    drawBorder(0);
    drawBorder(8);
    drawBorder(16);
    drawBorder(24);
    drawBorder(35);
}


void InfineonLogoScreen()
{

	extern const uint32_t infineonSplashSize;
	extern const unsigned char infineonSplash [];

    printContinuousBytes(infineonSplash);
}


void fontDemo()
{
	lcdPrintLine("This is a demo\nDemonstratin GLCD\n !!<>?@#", LCD_LINE_1);
}

void BarGrsphDemo()
{
	drawBarGraph(6,3,20);


	drawBarGraph(16,5,50);


	drawBarGraph(40,10,63);


	drawBarGraph(80,20,40);


	drawBarGraphY(120,6,15,30);
}

void CirclesDemo()
{
	uint8_t r = 0,i = 0, pr = 0;
	drawCircle(1,32,1);

	drawCircle(7,32,5);


	drawCircle(26,32,10);


	drawCircle(59,32,20);


	drawCircle(100,32,20);
}


