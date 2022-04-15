/*
 * timer.c
 *
 *  Created on: 19-Mar-2022
 *      Author: AustinA
 */




#include "timer.h"


void clearTimerInterrupt(uint32_t intrSource)
{
	Cy_TCPWM_ClearInterrupt(TIMER_HW, TIMER_NUM,  Cy_TCPWM_GetInterruptMask(TIMER_HW, TIMER_NUM));
}

void timerInit(cyisraddress Timer_Isr)
{

    if (CY_TCPWM_SUCCESS != Cy_TCPWM_Counter_Init(TIMER_HW, TIMER_NUM, &TIMER_config))
    {
        /* Handle possible errors */
    	printf("Timer Init ERROR \r\n");
    	CyDelay(100);
    	CY_ASSERT(0);
    }


    /* Populate configuration structure (code specific for CM4) */
    cy_stc_sysint_t TIMERntrConfig =
    {
        .intrSrc      = TIMER_IRQ,
        .intrPriority = 4,
    };
    /* Hook interrupt service routine and enable interrupt */
    (void) Cy_SysInt_Init(&TIMERntrConfig, Timer_Isr);
    NVIC_EnableIRQ(TIMERntrConfig.intrSrc);

    Cy_TCPWM_SetInterruptMask(TIMER_HW,TIMER_NUM,CY_TCPWM_INT_ON_TC);

    /* Enable the initialized counter */
    Cy_TCPWM_Counter_Enable(TIMER_HW, TIMER_NUM);

    /* Then start the counter */
    Cy_TCPWM_TriggerStart_Single(TIMER_HW, TIMER_NUM);

    if( Cy_TCPWM_Counter_GetStatus	(TIMER_HW, TIMER_NUM))
    {
    	Cy_TCPWM_Counter_GetCounter(TIMER_HW, TIMER_NUM);
    }
}
