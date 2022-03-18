/*
 * glcdPrints.c
 *
 *  Created on: 17-Mar-2022
 *      Author: AustinA
 */



#include "glcdPrints.h"


extern const uint32_t lengthofLCDCharMapMap;
extern const LCDChar_s LCDCharMap[];




void lcdPrint(char * stringD,lcdLineNum_e lcdLineNum,uint8_t overWrite)
{
	uint8_t pageCounter = 0;
//	char stringD[20];
//	sprintf(stringD,string);

	lcdPage1Select();
	setLcdXaddress(lcdLineNum);
	setLCDYAddress(0);
	for(int j = 0;j<strlen(stringD);j++)
	{
		if(stringD[j] == '\r')
		{
			lcdPage1Select();
			setLCDYAddress(0);
			pageCounter = 0;
		}
		else
		if(stringD[j] == '\n')
		{
			lcdPage1Select();
			pageCounter = 0;
			lcdLineNum+=1;
			setLcdXaddress(lcdLineNum);
			setLCDYAddress(0);
		}
		else
		for(int i = 0;i< (lengthofLCDCharMapMap);i++)
		{
			if(stringD[j] == LCDCharMap[i].Letter)
			{
				for(int k = 0;k < FONT_SIZE ;k++)
				{
					if(overWrite)
					{
						setLCDData(LCDCharMap[i].LCDPixelData[k] | getLCDContexData());
					}
					else
					{
						setLCDData(LCDCharMap[i].LCDPixelData[k]);
					}
					pageCounter++;
					if(pageCounter == 64)
					{
						lcdPage2Select();
						setLcdXaddress(lcdLineNum);
						setLCDYAddress(0);
					}
					if(pageCounter > 125)
					{
						lcdPage1Select();
						pageCounter = 0;
						lcdLineNum+=1;
						setLcdXaddress(lcdLineNum);
						setLCDYAddress(0);
					}
				}
			}
		}
	}
}
