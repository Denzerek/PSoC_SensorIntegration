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
    {'D',{0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00}},
	{'S',{0x46, 0x49, 0x49, 0x49, 0x31, 0x00}},
	{'I',{0x00, 0x41, 0x7F, 0x41, 0x00,  0x00}},
	{'P',{0x7F, 0x09, 0x09, 0x09, 0x06, 0x00}},
	{'A',{0x7C, 0x12, 0x11, 0x12, 0x7C,0x00}},
	{'F',{0x7F, 0x09, 0x09, 0x09, 0x01, 0x00}},
	{'G',{0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00}},
	{'T',{0x01, 0x01, 0x7F, 0x01, 0x01, 0x00}},
	{'N',{0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00}},
	{'!',{0x00, 0x00, 0x2f, 0x00, 0x00, 0x00}},
	{' ',{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	{'C',{0x3E, 0x41, 0x41, 0x41, 0x22,  0x00}},
	{'Y',{0x07, 0x08, 0x70, 0x08, 0x07,0x00}},
	{'R',{0x7F, 0x09, 0x19, 0x29, 0x46,0x00}},
	{'W',{0x3F, 0x40, 0x38, 0x40, 0x3F,0x00}},
	{'U',{0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00}},
	{':',{0x00, 0x36, 0x36, 0x00, 0x00, 0x00}} ,  // ,
	{'>',{0x00, 0x41, 0x22, 0x14, 0x08, 0x00}}   //
};


void print(char * string)
{
	uint8_t pageCounter = 0;
	char array[20];
	sprintf(array,string);

	LCDHalfSelect(LCD_HALF_1);
	setXAddress(3);
	setYAddress(0);
	for(int j = 0;j<strlen(array);j++)
	{
		for(int i = 0;i<(sizeof(LCDChar)/sizeof(LCDChar_s));i++)
		{
			if(array[j] == LCDChar[i].Letter)
			{
				for(int k = 0;k<6;k++)
				{
					lcdDataWrite(LCDChar[i].LCDPixelData[k]);
					pageCounter++;
					if(pageCounter == 64)
					{
						LCDHalfSelect(LCD_HALF_2);
//						displayState(DISPLAY_ON);
						setXAddress(3);
						setYAddress(0);
					}
				}
			}
		}
	}
}


void print2()
{
	uint8_t pageCounter = 0;
	char array[20];


	LCDHalfSelect(LCD_HALF_1);
	setXAddress(2);
	setYAddress(0);
	for(int i = 0;i<128;i++)
	{
		lcdDataWrite(0x00);
		pageCounter++;
		if(pageCounter == 64)
		{
			LCDHalfSelect(LCD_HALF_2);
//						displayState(DISPLAY_ON);
			setXAddress(2);
			setYAddress(0);
		}
	}

	print("     SCREW RAHUL :>  ");

	pageCounter = 0;
	LCDHalfSelect(LCD_HALF_1);
	setXAddress(4);
	setYAddress(0);
	for(int i = 0;i<128;i++)
	{
		lcdDataWrite(0x00);
		pageCounter++;
		if(pageCounter == 64)
		{
			LCDHalfSelect(LCD_HALF_2);
//						displayState(DISPLAY_ON);
			setXAddress(4);
			setYAddress(0);
		}
	}
}


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
