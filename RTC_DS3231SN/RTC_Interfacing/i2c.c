#include "i2c.h"


#define RTC_SLAVE_ADDRESS      0x68

cy_stc_scb_i2c_context_t cy_stc_scb_i2c_context;

volatile static uint32_t i2cStatus = 0;

typedef struct{
    uint32_t i2cEvent;
    char* i2cMessage;
}i2cEvents_s;

i2cEvents_s i2cEvents[] = {
    {CY_SCB_I2C_MASTER_MANUAL_ABORT_START,"CY_SCB_I2C_MASTER_MANUAL_ABORT_START"},
    {CY_SCB_I2C_MASTER_MANUAL_BUS_ERR,"CY_SCB_I2C_MASTER_MANUAL_BUS_ERR"},
    {CY_SCB_I2C_MASTER_MANUAL_ARB_LOST,"CY_SCB_I2C_MASTER_MANUAL_ARB_LOST"},
    {CY_SCB_I2C_MASTER_MANUAL_NAK,"CY_SCB_I2C_MASTER_MANUAL_NAK"},
    {CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK,"CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK"},
    {CY_SCB_I2C_MASTER_MANUAL_TIMEOUT,"CY_SCB_I2C_MASTER_MANUAL_TIMEOUT"},
    {CY_SCB_I2C_MASTER_NOT_READY,"CY_SCB_I2C_MASTER_NOT_READY"},
    {CY_SCB_I2C_BAD_PARAM,"CY_SCB_I2C_BAD_PARAM"},
    {0x00,"NONE"},
};



void i2cStatusDecode( cy_en_scb_i2c_status_t i2cStatus)
{
    for(int i = 0; i2cEvents[i].i2cEvent != 0x00 ; i++)
    {
        if((i2cEvents[i].i2cEvent & i2cStatus)  == i2cEvents[i].i2cEvent)
        {
            printf(i2cEvents[i].i2cMessage);
        }
    }
}

void i2cCallback(uint32_t event)
{
   i2cStatus |= event;
   printf("event %X\n",event);
}
void I2C_Isr(void)
{
    Cy_SCB_I2C_MasterInterrupt(scb_3_HW, &cy_stc_scb_i2c_context);
}

void i2c_init()
{

    Cy_SCB_I2C_Init(scb_3_HW,&scb_3_config,&cy_stc_scb_i2c_context);
    
    /* Populate configuration structure (code specific for CM4) */
    const cy_stc_sysint_t i2cIntrConfig =
    {
        .intrSrc      = scb_3_IRQ,
        .intrPriority = 7,
    };
    /* Hook interrupt service routine and enable interrupt */
    (void) Cy_SysInt_Init(&i2cIntrConfig, &I2C_Isr);

    NVIC_EnableIRQ(i2cIntrConfig.intrSrc);

    Cy_SCB_I2C_Enable(scb_3_HW);


}


void i2c_readBurst(uint8_t slaveAddress,uint8_t* registerAddressBuffer,uint8_t registerAddressSize,uint8_t* readDataBuffer,uint8_t readSize)
{
    cy_stc_scb_i2c_master_xfer_config_t transfer;
    /* Configure write transaction */
    transfer.slaveAddress = slaveAddress;
    transfer.buffer       = registerAddressBuffer;
    transfer.bufferSize   = registerAddressSize;
    transfer.xferPending  = true; /* Do not generate Stop condition at the end of transaction */
    /* Initiate write transaction.
    * The Start condition is generated to begin this transaction.
    */
    i2cStatusDecode( Cy_SCB_I2C_MasterWrite(SCB3, &transfer, &cy_stc_scb_i2c_context));
    /* Wait for transaction completion */
    while (0UL != (CY_SCB_I2C_MASTER_BUSY & Cy_SCB_I2C_MasterGetStatus(SCB3, &cy_stc_scb_i2c_context)))
    {
    }
    /* Configure read transaction */
    transfer.buffer       = readDataBuffer;
    transfer.bufferSize   = readSize;
    transfer.xferPending  = false; /* Generate Stop condition the end of transaction */
    /* Initiate read transaction.
    * The ReStart condition is generated to begin this transaction because
    * previous transaction was completed without Stop.
    */
    i2cStatusDecode(Cy_SCB_I2C_MasterRead(SCB3, &transfer, &cy_stc_scb_i2c_context));
    /* Wait for transaction completion */
    while (0UL != (CY_SCB_I2C_MASTER_BUSY & Cy_SCB_I2C_MasterGetStatus(SCB3, &cy_stc_scb_i2c_context)))
    {
    }
}


void i2c_readByte(uint8_t address,uint8_t registerAddress,uint8_t* readData)
{
    cy_stc_scb_i2c_master_xfer_config_t transfer;
    uint8_t writeBuffer[1UL] = {registerAddress};
    /* Configure write transaction */
    transfer.slaveAddress = address;
    transfer.buffer       = registerAddress;
    transfer.bufferSize   = 1;
    transfer.xferPending  = true; /* Do not generate Stop condition at the end of transaction */
    printf("I2C Write \r\n");
    /* Initiate write transaction.
    * The Start condition is generated to begin this transaction.
    */
    i2cStatusDecode( Cy_SCB_I2C_MasterWrite(SCB3, &transfer, &cy_stc_scb_i2c_context));
    /* Wait for transaction completion */
    while (0UL != (CY_SCB_I2C_MASTER_BUSY & Cy_SCB_I2C_MasterGetStatus(SCB3, &cy_stc_scb_i2c_context)))
    {
    }
    /* Configure read transaction */
    transfer.buffer       = readData;
    transfer.bufferSize   = 1;
    transfer.xferPending  = false; /* Generate Stop condition the end of transaction */
    printf("I2C Read \r\n");
    /* Initiate read transaction.
    * The ReStart condition is generated to begin this transaction because
    * previous transaction was completed without Stop.
    */
    i2cStatusDecode(Cy_SCB_I2C_MasterRead(SCB3, &transfer, &cy_stc_scb_i2c_context));
    /* Wait for transaction completion */
    while (0UL != (CY_SCB_I2C_MASTER_BUSY & Cy_SCB_I2C_MasterGetStatus(SCB3, &cy_stc_scb_i2c_context)))
    {
    }
}
