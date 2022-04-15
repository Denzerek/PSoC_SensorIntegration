/*
 * sarADC.c
 *
 *  Created on: 07-Apr-2022
 *      Author: AustinA
 */




#include "sarADC.h"


static volatile uint16_t ADCResult[2];

void dma_Init()
{

    Cy_DMA_Descriptor_Init(&JDMA_Descriptor_0, &JDMA_Descriptor_0_config);

    Cy_DMA_Descriptor_SetSrcAddress(&JDMA_Descriptor_0, SAR->CHAN_RESULT);

    Cy_DMA_Descriptor_SetDstAddress(&JDMA_Descriptor_0, &ADCResult);

    Cy_DMA_Channel_Init(JDMA_HW, JDMA_CHANNEL, &JDMA_channelConfig);

    Cy_DMA_Channel_Enable(JDMA_HW, JDMA_CHANNEL);

    Cy_DMA_Enable(JDMA_HW);
}


void sarADC_Init()
{

    Cy_SysAnalog_Init(&pass_0_aref_0_config);

    Cy_SysAnalog_Enable();

    cy_en_sar_status_t status;
    status = Cy_SAR_Init(SAR_HW, &SAR_config);
    if (CY_SAR_SUCCESS == status)
    {
    	printf("SAR Init SUCCESS \r\n");
        /* Turn on the SAR hardware. */
        Cy_SAR_Enable(SAR);
        /* Begin continuous conversions. */
        Cy_SAR_StartConvert(SAR, CY_SAR_START_CONVERT_CONTINUOUS);
    }

    dma_Init();
}

uint16_t getChannel_0_Result()
{
	return ADCResult[0];
}


uint16_t getChannel_1_Result()
{
	return ADCResult[1];
}



uint8_t sarADC_ConversionStatus()
{
	return Cy_SAR_IsEndConversion(SAR_HW,CY_SAR_RETURN_STATUS );
}

void sarADC_StartConversion_Single()
{
	 Cy_SAR_StartConvert(SAR_HW,CY_SAR_START_CONVERT_SINGLE_SHOT);
}

void sarADC_StartConversion_Continuous()
{
	 Cy_SAR_StartConvert(SAR_HW,CY_SAR_START_CONVERT_CONTINUOUS);
}

uint32_t sarADC_GetResultWord( uint32_t channel)
{
	return Cy_SAR_GetResult32(SAR_HW, channel);
}
