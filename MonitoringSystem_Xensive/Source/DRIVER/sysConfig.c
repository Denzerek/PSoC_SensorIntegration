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
#include "dps3xxDriver.h"
#include "cy_retarget_io.h"

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

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX,
                                    CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence to clear screen. */
    printf("\x1b[2J\x1b[;H \r");
    printf("=========================================================\n\r");
    printf("  PSoC 6 MCU:  Interfacing DPS3xx Pressure Sensor \r\n");
    printf("=========================================================\n\n\r");

    dps3xx_Init();
}
