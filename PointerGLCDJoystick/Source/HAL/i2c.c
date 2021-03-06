#include "i2c.h"


cy_stc_scb_i2c_context_t cy_stc_scb_i2c_context;

volatile static uint32_t i2cStatus = 0;

void i2cCallback(uint32_t event)
{
   i2cStatus |= event;
   I2CHAL_PRINT("event %X\n",event);
}


void I2C_Isr(void)
{
    Cy_SCB_I2C_MasterInterrupt(CYBSP_I2C_HW, &cy_stc_scb_i2c_context);
}


cy_en_scb_i2c_status_t i2c_init(cy_cb_scb_i2c_handle_events_t eventCallback)
{

	cy_en_scb_i2c_status_t status;
	status = Cy_SCB_I2C_Init(CYBSP_I2C_HW,&CYBSP_I2C_config,&cy_stc_scb_i2c_context);
    if(CY_SCB_I2C_SUCCESS != status)
    {
        I2CHAL_PRINT("I2C Init error");
        return status;
    }

    /* Populate configuration structure (code specific for CM4) */
    const cy_stc_sysint_t i2cIntrConfig =
    {
        .intrSrc      = CYBSP_I2C_IRQ,
        .intrPriority = 7,
    };
    /* Hook interrupt service routine and enable interrupt */
    (void) Cy_SysInt_Init(&i2cIntrConfig, &I2C_Isr);

    NVIC_EnableIRQ(i2cIntrConfig.intrSrc);

    Cy_SCB_I2C_RegisterEventCallback(CYBSP_I2C_HW,eventCallback,&cy_stc_scb_i2c_context);

    Cy_SCB_I2C_Enable(CYBSP_I2C_HW);

    return status;


}


cy_en_scb_i2c_status_t CYBSP_I2C_master_write(cy_stc_scb_i2c_master_xfer_config_t *transfer)
{
    return Cy_SCB_I2C_MasterWrite(CYBSP_I2C_HW, transfer, &cy_stc_scb_i2c_context);
}

uint32_t CYBSP_I2C_master_get_status()
{
     return Cy_SCB_I2C_MasterGetStatus(CYBSP_I2C_HW, &cy_stc_scb_i2c_context);
}



cy_en_scb_i2c_status_t CYBSP_I2C_master_read(cy_stc_scb_i2c_master_xfer_config_t *transfer)
{
    return Cy_SCB_I2C_MasterRead(CYBSP_I2C_HW, transfer, &cy_stc_scb_i2c_context);
}


void CYBSP_I2C_master_clearInterrupt(uint32_t event)
{
    return Cy_SCB_ClearMasterInterrupt(CYBSP_I2C_HW, event);
}


