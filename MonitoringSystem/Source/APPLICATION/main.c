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

#include "sysConfig.h"
#include "LCD_ScreenControl.h"
#include "xensiv_dps3xx_mtb.h"
#include "xensiv_dps3xx.h"
#include "cy_retarget_io.h"


xensiv_dps3xx_t DPS3xx_Sensor;
cyhal_i2c_t* i2c;

int main(void)
{
    cy_rslt_t result;
    uint32_t revisionID = 0;
    float pressure, temperature;

	systemInit();


    CyDelay(500);
    clearLCD();


    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX,
                                    CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    CyDelay(500);
    /* \x1b[2J\x1b[;H - ANSI ESC sequence to clear screen. */
    printf("\x1b[2J\x1b[;H \r");
    printf("=========================================================\n\r");
    printf("  PSoC 6 MCU:  Interfacing DPS3xx Pressure Sensor \r\n");
    printf("=========================================================\n\n\r");
    /* Initialize pressure sensor */
    result = xensiv_dps3xx_mtb_init_i2c(&DPS3xx_Sensor, i2c,
                                        XENSIV_DPS3XX_I2C_ADDR_DEFAULT);

    

    if (result != CY_RSLT_SUCCESS)
    {
        printf("\r\nFailed to initialize DPS3xx I2C\r\n");
        CY_ASSERT(0);
    }

    /* Retrieve the DPS3xx Revision ID and display the same */
    if(xensiv_dps3xx_get_revision_id(&DPS3xx_Sensor,(uint8_t*)&revisionID) == CY_RSLT_SUCCESS)
    {
        printf("DPS3xx Revision ID = %d\r\n\n",(uint8_t)revisionID);
    }
    else
    {
        printf("Failed to get Revision ID\r\n");
        CY_ASSERT(0);
    }

    for (;;)
    {
        /* Read the pressure and temperature data */
        if(xensiv_dps3xx_read(&DPS3xx_Sensor, &pressure, &temperature) == CY_RSLT_SUCCESS)
        {
            /* Display the pressure and temperature data in console*/
            printf("Pressure : %f mBar",pressure);
            printf(" \t Temperature: %f degreeCelcius \r\n",temperature);
        }
        else
        {
            printf("\n Failed to read temperature and pressure data.\r\n");
            CY_ASSERT(0);
        }

        /* Generate a delay of 1 second before next read */
        cyhal_system_delay_ms(1000);
    }

    for (;;)
    {

    	ScreenDisplay_Task();

    }
}

/* [] END OF FILE */
