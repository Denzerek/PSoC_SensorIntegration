/*
 * newHavenLCD.h
 *
 *  Created on: 15-Mar-2022
 *      Author: AustinA
 */

#ifndef NEWHAVENLCD_H_
#define NEWHAVENLCD_H_


#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"


#define SET_RS()	Cy_GPIO_Write(RS_PORT, RS_PIN, 1u);
#define CLEAR_RS()	Cy_GPIO_Write(RS_PORT, RS_PIN, 0u);
#define SET_RW()	Cy_GPIO_Write(RW_PORT, RW_PIN, 1u);
#define CLEAR_RW()	Cy_GPIO_Write(RW_PORT, RW_PIN, 0u);
#define SET_DB0()	Cy_GPIO_Write(DB0_PORT, DB0_PIN, 1u);
#define CLEAR_DB0()	Cy_GPIO_Write(DB0_PORT, DB0_PIN, 0u);
#define SET_DB1()	Cy_GPIO_Write(DB1_PORT, DB1_PIN, 1u);
#define CLEAR_DB1()	Cy_GPIO_Write(DB1_PORT, DB1_PIN, 0u);
#define SET_DB2()	Cy_GPIO_Write(DB2_PORT, DB2_PIN, 1u);
#define CLEAR_DB2()	Cy_GPIO_Write(DB2_PORT, DB2_PIN, 0u);
#define SET_DB3()	Cy_GPIO_Write(DB3_PORT, DB3_PIN, 1u);
#define CLEAR_DB3()	Cy_GPIO_Write(DB3_PORT, DB3_PIN, 0u);
#define SET_DB4()	Cy_GPIO_Write(DB4_PORT, DB4_PIN, 1u);
#define CLEAR_DB4()	Cy_GPIO_Write(DB4_PORT, DB4_PIN, 0u);
#define SET_DB5()	Cy_GPIO_Write(DB5_PORT, DB5_PIN, 1u);
#define CLEAR_DB5()	Cy_GPIO_Write(DB5_PORT, DB5_PIN, 0u);
#define SET_DB6()	Cy_GPIO_Write(DB6_PORT, DB6_PIN, 1u);
#define CLEAR_DB6()	Cy_GPIO_Write(DB6_PORT, DB6_PIN, 0u);
#define SET_DB7()	Cy_GPIO_Write(DB7_PORT, DB7_PIN, 1u);
#define CLEAR_DB7()	Cy_GPIO_Write(DB7_PORT, DB7_PIN, 0u);
#define SET_CS1()	Cy_GPIO_Write(CS1_PORT, CS1_PIN, 1u);
#define CLEAR_CS1()	Cy_GPIO_Write(CS1_PORT, CS1_PIN, 0u);
#define SET_CS2()	Cy_GPIO_Write(CS2_PORT, CS2_PIN, 1u);
#define CLEAR_CS2()	Cy_GPIO_Write(CS2_PORT, CS2_PIN, 0u);
#define SET_RES()	Cy_GPIO_Write(RES_PORT, RES_PIN, 1u);
#define CLEAR_RES()	Cy_GPIO_Write(RES_PORT, RES_PIN, 0u);
#define SET_LCD_E()	Cy_GPIO_Write(LCD_E_PORT, LCD_E_PIN, 1u);
#define CLEAR_LCD_E()	Cy_GPIO_Write(LCD_E_PORT, LCD_E_PIN, 0u);

void lcdInit();

#endif /* NEWHAVENLCD_H_ */
