/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Empty PSoC6 Application
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "newHavenLCD.h"
#include "lcdDriver.h"
#include "coolshapes.h"


int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    Cy_GPIO_Write(CYBSP_LED_RGB_GREEN_PORT, CYBSP_LED_RGB_GREEN_PIN, 0);

    __enable_irq();

    lcdInit();
    CyDelay(1000);
    clearLCD();

    concentricRectDemo();
    print("  SHILPA FIGHTING!!! ");
    drawCircle(40,15,5);
    drawCircle(80,25,5);
    drawCircle(70,55,5);
    drawCircle(10,49,5);
    drawCircle(120,10,5);
//
//
    for(int i = 0;i<128;i++)
    {
		for(int j = 0;j<i;j++){
			setPixel(i+j,i);
			setPixel(127-i-j,i);
		}
    }
    setAllPixelsTo(0xFF);
    print2();
    CyDelay(1000);

    setAllPixelsTo(0x00);
	LCDHalfSelect(LCD_HALF_1);
    displayState(DISPLAY_OFF);
	LCDHalfSelect(LCD_HALF_2);
    displayState(DISPLAY_OFF);


    //    for(int i = 0;i<128;i++)
    //    {
    //    	for(int j = 0;j<i;j++)
    //    	{
    //    		setPixel(i,j);
    //    	}
    //    }

    //
    //        for(int i = 0;i<128;i++)
    //        {
    //			for(int j = 0;j<i;j++){
    //				setPixel(i+j,i);
    //				setPixel(i,i+j);
    //			}
    //        }


    print("  CHIPPY FIGHTING!!! ");
    for (;;)
    {
		LCDHalfSelect(LCD_HALF_1);
        displayState(DISPLAY_ON);
		LCDHalfSelect(LCD_HALF_2);
        displayState(DISPLAY_ON);
        Cy_GPIO_Write(CYBSP_LED_RGB_GREEN_PORT, CYBSP_LED_RGB_GREEN_PIN, 0);
        CyDelay(500);
		LCDHalfSelect(LCD_HALF_1);
        displayState(DISPLAY_OFF);
		LCDHalfSelect(LCD_HALF_2);
        displayState(DISPLAY_OFF);
        Cy_GPIO_Write(CYBSP_LED_RGB_GREEN_PORT, CYBSP_LED_RGB_GREEN_PIN, 1);
        CyDelay(500);
    }
}

/* [] END OF FILE */
