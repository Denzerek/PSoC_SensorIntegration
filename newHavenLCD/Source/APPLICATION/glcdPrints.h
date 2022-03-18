/*
 * glcdPrints.h
 *
 *  Created on: 17-Mar-2022
 *      Author: AustinA
 */

#ifndef SOURCE_APPLICATION_GLCDPRINTS_H_
#define SOURCE_APPLICATION_GLCDPRINTS_H_


#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "lcdDriver.h"


#include <stdarg.h>

#define lcdPrintLineOver(string,lineNum)	lcdPrint(string,lineNum,true,false)
#define lcdPrintLine(string,lineNum)		lcdPrint(string,lineNum,false,false)
#define lcdPrintLineI(string,lineNum)		lcdPrint(string,lineNum,false,true)
#define lcdPrintLineOverI(string,lineNum)	lcdPrint(string,lineNum,true,true)
#define FONT_SIZE	6
typedef struct{
    char Letter;
    uint8_t LCDPixelData[FONT_SIZE];
}LCDChar_s;


typedef enum{
	LCD_LINE_1,
	LCD_LINE_2,
	LCD_LINE_3,
	LCD_LINE_4,
	LCD_LINE_5,
	LCD_LINE_6,
	LCD_LINE_7,
	LCD_LINE_8,
}lcdLineNum_e;


void lcdPrint(char * stringD,lcdLineNum_e lcdLineNum,uint8_t overWrite,uint8_t invertFlag);


#endif /* SOURCE_APPLICATION_GLCDPRINTS_H_ */
