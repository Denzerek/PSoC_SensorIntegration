/*
 * glcdPrints.c
 *
 *  Created on: 17-Mar-2022
 *      Author: AustinA
 */



#include "glcdPrints.h"


extern const uint32_t lengthofLCDCharMapMap;
extern const LCDChar_s LCDCharMap[];




void lcdPrint(char * stringD,uint8_t startX,lcdLineNum_e lcdLineNum,uint8_t overWrite,uint8_t invertFlag)
{
	uint8_t pageCounter = startX;
	uint8_t dataToWrite;

	if(startX > 63)
		{lcdPage2Select();}
	else
		{lcdPage1Select();}
	setLcdXaddress(lcdLineNum);
	setLCDYAddress(startX % 64);
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
					/* Set teh overWrite flag to write over any images already present in the LCD screen*/
					if(overWrite)
					{
						dataToWrite = LCDCharMap[i].LCDPixelData[k] | getLCDContexData();
					}
					/*If overWriteFlag is disabled, image section will be replaved by the string*/
					else
					{
						dataToWrite = LCDCharMap[i].LCDPixelData[k] ;
					}

					if(invertFlag)
					{
						dataToWrite = dataToWrite ^ 0xFF;
					}

					setLCDData(dataToWrite);


					/* Count the number of bytes written(basically x axis increment)*/
					pageCounter++;
					/* if x axis increment is greater than 64, switch LCD half to next one*/
					if(pageCounter == 64)
					{
						lcdPage2Select();
						setLcdXaddress(lcdLineNum);
						setLCDYAddress(0);
					}
					/* if x axis increment is greater than 128, one line on LCD has been fully traversed and next data to be
					 * written in next line of LCD*/
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
