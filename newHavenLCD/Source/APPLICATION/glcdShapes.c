#include "glcdShapes.h"
#include "lcdDriver.h"
#include "newHavenLCD.h"


typedef struct{
    char Letter;
    uint8_t LCDPixelData[6];
}LCDChar_s;

LCDChar_s LCDChar[]={
    {'H',{0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00}},
    {'E',{0x7F, 0x49, 0x49, 0x49, 0x41, 0x00}},
    {'L',{0x7F, 0x40, 0x40, 0x40, 0x40, 0x00}},
    {'O',{0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00}},
    {'W',{0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00}},
    {'R',{0x7F, 0x09, 0x19, 0x29, 0x46, 0x00}},
    {'D',{0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00}}
};

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
        setPixel(128 - offset,i);
    }
}

void drawHollowCircle(int cx, int cy,int r)
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
