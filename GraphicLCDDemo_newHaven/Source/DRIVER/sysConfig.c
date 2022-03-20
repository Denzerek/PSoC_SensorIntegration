/*
 * sysConfig.c
 *
 *  Created on: 19-Mar-2022
 *      Author: AustinA
 */


#include "sysConfig.h"
#include "timerDriver.h"
#include "lcdDriver.h"
#include "buttonDriver.h"

void systemInit()
{

    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    Cy_GPIO_Write(CYBSP_LED_RGB_GREEN_PORT, CYBSP_LED_RGB_GREEN_PIN, 0);
    userButtonInit();


    __enable_irq();
    timerDriver_Init();

    lcdGPIO_Init();
}
