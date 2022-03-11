/*
 * dma.c
 *
 *  Created on: 11-Mar-2022
 *      Author: AustinA
 */




#include "dma.h"

cy_stc_sysint_t UDMA_INT_cfg =
{
    .intrSrc      = (IRQn_Type)UDMA_IRQ,
    .intrPriority = 7u,
};

void dmaStartTransfer()
{
    Cy_DMA_Channel_Enable(UDMA_HW, UDMA_CHANNEL);
}

void dmaInit(cy_israddress *dmaTxInterruptHandler)
{
    cy_en_dma_status_t dma_init_status;

    /* Init descriptor */
    dma_init_status = Cy_DMA_Descriptor_Init(&UDMA_Descriptor_0, &UDMA_Descriptor_0_config);
    if (dma_init_status!=CY_DMA_SUCCESS)
    {
        CY_ASSERT(0);
    }
    dma_init_status = Cy_DMA_Channel_Init(UDMA_HW, UDMA_CHANNEL, &UDMA_channelConfig);
    if (dma_init_status!=CY_DMA_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Set source and destination for descriptor 1 */
    Cy_DMA_Descriptor_SetDstAddress(&UDMA_Descriptor_0, (uint32_t *) &(UART_HW->TX_FIFO_WR));

    /* Set next descriptor to NULL to stop the chain execution after descriptor 1
    *  is completed.
    */
    Cy_DMA_Descriptor_SetNextDescriptor(Cy_DMA_Channel_GetCurrentDescriptor(UDMA_HW, UDMA_CHANNEL), &UDMA_Descriptor_0);

    /* Initialize and enable the interrupt from UDMA */
    Cy_SysInt_Init  (&UDMA_INT_cfg, dmaTxInterruptHandler);
    NVIC_EnableIRQ(UDMA_INT_cfg.intrSrc);

    /* Enable DMA interrupt source */
    Cy_DMA_Channel_SetInterruptMask(UDMA_HW, UDMA_CHANNEL, CY_DMA_INTR_MASK);

    /* Enable Data Write block but keep channel disabled to not trigger
    *  descriptor execution because TX FIFO is empty and SCB keeps active level
    *  for DMA.
    */
    Cy_DMA_Enable(UDMA_HW);


    Cy_DMA_Channel_SetDescriptor(UDMA_HW, UDMA_CHANNEL, &UDMA_Descriptor_0);
}


void setDMASource(uint32_t* source)
{
    Cy_DMA_Descriptor_SetSrcAddress(&UDMA_Descriptor_0, (uint32_t *) source);

}
