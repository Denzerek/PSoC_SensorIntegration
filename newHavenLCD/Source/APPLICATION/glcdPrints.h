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

#define lcdPrintLineOver(string,lineNum)	lcdPrint(string,lineNum,1)
#define lcdPrintLine(string,lineNum)		lcdPrint(string,lineNum,0)
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




#endif /* SOURCE_APPLICATION_GLCDPRINTS_H_ */
