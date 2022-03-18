#include "glcdShapes.h"
#include "lcdDriver.h"
#include "newHavenLCD.h"



void drawBorder(uint8_t offset)
{
    for(int i = offset; i < 128 - offset;i++)
    {
        setPixel(i,offset);
        setPixel(i,63 - offset);
    }

    
    for(int i = offset; i < 64 - offset;i++)
    {
        setPixel(offset,i);
        setPixel(127 - offset,i);
    }
}


void drawBarGraph(uint8_t x,uint8_t width,uint8_t height)
{

	uint8_t val2 = x-width/2;

	for(int j = 0 ;j<height;j++)
	for(int  i =val2; i < (val2+width) ; i++)
	{
		setPixel(i,j);
	}
}


void drawBarGraphY(uint8_t x,uint8_t y,uint8_t width,uint8_t height)
{

	uint8_t val2 = x-width/2;

	for(int j = y ;j<height+y;j++)
	for(int  i =val2; i < (val2+width) ; i++)
	{
		setPixel(i,j);
	}
}


void drawCircle(int cx, int cy,int r)
{
    int x, y;  // Coordinates inside the rectangle
 
    // Draw a square of size N*N.
    for (int i = 0; i < 128; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            // Start from the left most corner point
            x = i-cx;
            y = j-cy;
 
            // If this point is inside the circle, print it
            if (x*x + y*y <= (r*r+1 ))
                setPixel(i,j);
        }
    }
}
