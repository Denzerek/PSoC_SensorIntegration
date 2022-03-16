#include "glcdShapes.h"
#include "lcdDriver.h"
#include "newHavenLCD.h"

void drawBorder(uint8_t offset)
{
    LCDDisplay_off();
    for(int i = offset; i < 128 - offset;i++)
    {
        setPixel(i,offset);
        CyDelay(1);
        setPixel(i,63 - offset);
        CyDelay(1);
    }

    
    for(int i = offset; i < 64 - offset;i++)
    {
        setPixel(offset,i);
        CyDelay(1);
        setPixel(128 - offset,i);
        CyDelay(1);
    }
    LCDDisplay_on();
}

#if 0
void drawCircle(int r)
{
    // Consider a rectangle of size N*N
    // int N = 2*r+1;
 
    int x, y;  // Coordinates inside the rectangle
 
    // Draw a square of size N*N.
    for (int i = 0; i < 128; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            // Start from the left most corner point
            x = i-r;
            y = j-r;
 
            // If this point is inside the circle, print it
            if (x*x + y*y <= r*r+1 )
                setPixel(i,j);
            else // If outside the circle, print space
                printf(" ");
            printf(" ");
        }
        printf("\n");
    }
}
#else
void drawCircle(uint8_t x, uint8_t y, uint8_t radius)
{

    uint32_t xSquare = 0;
    uint32_t ySquare = 0;
    uint32_t radiusSquare = radius*radius;
    
    for(int i = 0;i< 128;i++)
    {
        for (int j = 0; j< 64;j++)
        {
            
            xSquare = (x - i) * (x - i);
            ySquare = (y - j) * (y - j);
            if( (xSquare + ySquare) == radiusSquare)
            {
                setPixel(i,j);
            }
        }
    }


}

#endif