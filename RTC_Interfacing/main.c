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
#include "common.h"
#include "rtc_driver.h"


//#define GREEN_LED    CYBSP_USER_LED4
//#define BLUE_LED    CYBSP_USER_LED5
//#define RED_LED    CYBSP_USER_LED3
//#define LED_2       CYBSP_USER_LED2
#define LED_1       CYBSP_USER_LED1


void retargetIOInit()
{
    /* Initialize the retarget-io */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, 115200);
    
    /* \x1b[2J\x1b[;H - ANSI ESC sequence to clear screen. */
    printf("\x1b[2J\x1b[;H \r");
    printf("=========================================================\n\r");
    printf(" RTC Interfacing with PSoC 6 \r\n");
    printf("=========================================================\n\n\r");
}
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    __enable_irq();

    retargetIOInit();

//    cyhal_gpio_init(GREEN_LED,CYHAL_GPIO_DIR_OUTPUT,CYHAL_GPIO_DRIVE_STRONG,0);
//    cyhal_gpio_init(BLUE_LED,CYHAL_GPIO_DIR_OUTPUT,CYHAL_GPIO_DRIVE_STRONG,0);
//    cyhal_gpio_init(RED_LED,CYHAL_GPIO_DIR_OUTPUT,CYHAL_GPIO_DRIVE_STRONG,0);
//    cyhal_gpio_init(LED_2,CYHAL_GPIO_DIR_OUTPUT,CYHAL_GPIO_DRIVE_STRONG,0);
//    cyhal_gpio_init(LED_1,CYHAL_GPIO_DIR_OUTPUT,CYHAL_GPIO_DRIVE_STRONG,0);

    i2c_init();

    uint8_t write[4]={0,0,0,0};
    uint8_t read[4]={0,0,0,0};
    // rtc_setTime(2021,11,14,7,1,59,55);
    printf("\r\n\n");
    for (;;)
    { 
        // i2c_readBurst(0x68,write,1,read,4);
        //printf("\r %d %d %d %d | %d%d sec",read[3],read[2],read[1],read[0],read[0]>>4,read[0]&0x0F);
        printf("\r%d Year %d month %d Date %d Day %d hr %d min %d  sec",rtc_getYear(),rtc_getMonth(),rtc_getDate(),rtc_getDay(),rtc_getHours(),rtc_getMinutes(),rtc_getSeconds());
//        rtc_displayAllTime();
        fflush(stdout);
        CyDelay(100);
    }
}

/* [] END OF FILE */
