/*
 * button.c
 *
 *  Created on: 18-Mar-2022
 *      Author: AustinA
 */

#include "button.h"

void clearButtonInterrupt()
{
	Cy_GPIO_ClearInterrupt(CYBSP_SW2_PORT,CYBSP_SW2_NUM);
}
/* Assign BUTTON interrupt number and priority */
#define BUTTON_INTR_PRIORITY   (7U)
/* Populate configuration structure (code specific for CM4) */
cy_stc_sysint_t BUTTONIntrConfig =
{
    .intrSrc      = CYBSP_SW2_IRQ,
    .intrPriority = BUTTON_INTR_PRIORITY,
};

void buttonInit(cy_israddress BUTTON_Isr)
{

	/* Hook interrupt service routine and enable interrupt */
	(void) Cy_SysInt_Init(&BUTTONIntrConfig, BUTTON_Isr);
	NVIC_EnableIRQ(BUTTONIntrConfig.intrSrc);
}
