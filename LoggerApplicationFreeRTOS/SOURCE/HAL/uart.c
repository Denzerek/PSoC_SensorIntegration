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

/*******************************************************************************
 *        Header Files
 *******************************************************************************/
#include "common.h"
#include "uart.h"


/*******************************************************************************
 *        Variable Definitions
 *******************************************************************************/
/* Allocate context for UART operation */
cy_stc_scb_uart_context_t uartContext;

uint8_t uartRecieveBuffer[20];



/******************************************************************************
 *                          Function Definitions
 ******************************************************************************/
void UART_Isr(void)
{
    Cy_SCB_UART_Interrupt(UART_HW, &uartContext);
}




void UART_Init(cy_cb_scb_uart_handle_events_t callback)
{

	/* Configure UART to operate */
	(void) Cy_SCB_UART_Init(UART_HW, &UART_config, &uartContext);


	/* Populate configuration structure (code specific for CM4) */
	cy_stc_sysint_t uartIntrConfig =
	{
	    .intrSrc      = UART_IRQ,
	    .intrPriority = 6u,
	};
	/* Hook interrupt service routine and enable interrupt */
	(void) Cy_SysInt_Init(&uartIntrConfig, &UART_Isr);
	NVIC_EnableIRQ(uartIntrConfig.intrSrc);

	//Set the Transmission done interrupt for SCB
	Cy_SCB_SetTxInterruptMask(UART_HW,CY_SCB_TX_INTR_UART_DONE);

	//Set the Receive not empty interrupt for SCB
	Cy_SCB_SetRxInterruptMask(UART_HW,CY_SCB_RX_INTR_NOT_EMPTY);


	//Register the callback for the interrupt events
	Cy_SCB_UART_RegisterCallback(UART_HW,callback,&uartContext);

	/* Enable UART to operate */
	Cy_SCB_UART_Enable(UART_HW);
}



void UART_Transmit(char* transmitData)
{

	/* Start transmit operation (do not check status) */
	(void) Cy_SCB_UART_Transmit(UART_HW, transmitData,strlen(transmitData), &uartContext);

}


uint32_t UART_Receive()
{
	return Cy_SCB_ReadRxFifo(UART_HW);
}


/* [] END OF FILE */
