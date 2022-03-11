#include "dma_driver.h"
#include "serialDriver.h"


volatile static uint8_t tx_dma_error;
void DMA_TX_ISR(void)
{
    /* Check interrupt cause to capture errors.
    *  Note that next descriptor is NULL to stop descriptor execution */
    if ((CY_DMA_INTR_CAUSE_COMPLETION    != Cy_DMA_Channel_GetStatus(UDMA_HW, UDMA_CHANNEL)) &&
        (CY_DMA_INTR_CAUSE_CURR_PTR_NULL != Cy_DMA_Channel_GetStatus(UDMA_HW, UDMA_CHANNEL)))
    {
        /* DMA error occurred while TX operations */
        tx_dma_error = 1;
    }

    if(CY_DMA_INTR_CAUSE_COMPLETION == Cy_DMA_Channel_GetStatus(UDMA_HW, UDMA_CHANNEL))
    {
        setDMASource(getSerialQueueSourceAddress());
        serialPreviousQueueReset();
    }
    Cy_DMA_Channel_ClearInterrupt(UDMA_HW, UDMA_CHANNEL);
}


void dmaDriver_init()
{
    dmaInit(DMA_TX_ISR);
	switchCurrentSerialQueue();
	setDMASource(getSerialQueueSourceAddress());
}