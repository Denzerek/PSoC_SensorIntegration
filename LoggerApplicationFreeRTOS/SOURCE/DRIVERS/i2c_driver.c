#include "i2c_driver.h"
#include "i2c.h"

typedef struct{
    uint32_t i2cEvent;
    char* i2cMessage;
}i2cEvents_s;

volatile static bool hardwareErrorFlag;

i2cEvents_s i2cErrEvents[] = {
    {CY_SCB_I2C_MASTER_WR_IN_FIFO_EVENT,"CY_SCB_I2C_MASTER_WR_IN_FIFO_EVENT"},
    {CY_SCB_I2C_MASTER_WR_CMPLT_EVENT,"CY_SCB_I2C_MASTER_WR_CMPLT_EVENT"},
    {CY_SCB_I2C_MASTER_RD_CMPLT_EVENT,"CY_SCB_I2C_MASTER_RD_CMPLT_EVENT"},
    {CY_SCB_I2C_MASTER_ERR_EVENT,"CY_SCB_I2C_MASTER_ERR_EVENT"},

};

i2cEvents_s i2cEvents[] = {
    {CY_SCB_I2C_MASTER_MANUAL_ABORT_START,"CY_SCB_I2C_MASTER_MANUAL_ABORT_START"},
    {CY_SCB_I2C_MASTER_MANUAL_BUS_ERR,"CY_SCB_I2C_MASTER_MANUAL_BUS_ERR"},
    {CY_SCB_I2C_MASTER_MANUAL_ARB_LOST,"CY_SCB_I2C_MASTER_MANUAL_ARB_LOST"},
    {CY_SCB_I2C_MASTER_MANUAL_NAK,"CY_SCB_I2C_MASTER_MANUAL_NAK"},
    {CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK,"CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK"},
    {CY_SCB_I2C_MASTER_MANUAL_TIMEOUT,"CY_SCB_I2C_MASTER_MANUAL_TIMEOUT"},
    {CY_SCB_I2C_MASTER_NOT_READY,"CY_SCB_I2C_MASTER_NOT_READY"},
    {CY_SCB_I2C_BAD_PARAM,"CY_SCB_I2C_BAD_PARAM"},
    {CY_SCB_I2C_MASTER_ADDR_NAK,"CY_SCB_I2C_MASTER_ADDR_NAK"},
    {CY_SCB_I2C_MASTER_WR_IN_FIFO,"CY_SCB_I2C_MASTER_WR_IN_FIFO"},
    {CY_SCB_I2C_MASTER_ARB_LOST,"CY_SCB_I2C_MASTER_ARB_LOST",}
};



void i2cStatusDecode( uint32_t i2cStatus)
{
    for(int i = 0; i < (sizeof(i2cEvents)/sizeof(i2cEvents_s)) ; i++)
    {
        if((i2cEvents[i].i2cEvent & i2cStatus)  == i2cEvents[i].i2cEvent)
        {
            I2CDRIVER_PRINT(i2cEvents[i].i2cMessage);
        }
    }
}



i2cDriverState_t i2c_interrupt_eventsCallback(uint32_t event)
{
    if((CY_SCB_I2C_MASTER_ERR_EVENT & event ) == CY_SCB_I2C_MASTER_ERR_EVENT)
    {
        i2cStatusDecode(i2c_scb3_master_get_status());
        I2CDRIVER_PRINTF("I2C hardware ERROR %X",i2c_scb3_master_get_status());
        hardwareErrorFlag = true;
        // Cy_SCB_ClearMasterInterrupt();
        // i2c_scb3_master_clearInterrupt(CY_SCB_I2C_MASTER_ERR_EVENT);
        // i2c_scb3_master_clearInterrupt(i2c_scb3_master_get_status());
    }
}

void i2c_driver_init()
{
    i2c_init(i2c_interrupt_eventsCallback);
}



i2cDriverState_t i2c_readBurst(uint8_t slaveAddress,uint8_t* registerAddressBuffer,uint8_t registerAddressSize,uint8_t* readDataBuffer,uint8_t readSize)
{
    if(hardwareErrorFlag){return I2C_DRIVER_RW_FAILED;}
    cy_stc_scb_i2c_master_xfer_config_t transfer;
    /* Configure write transaction */
    transfer.slaveAddress = slaveAddress;
    transfer.buffer       = registerAddressBuffer;
    transfer.bufferSize   = registerAddressSize;
    transfer.xferPending  = true; /* Do not generate Stop condition at the end of transaction */
    /* Initiate write transaction.
    * The Start condition is generated to begin this transaction.
    */
    i2cStatusDecode( i2c_scb3_master_write(&transfer));

    /* Wait for transaction completion */
    while (0UL != (CY_SCB_I2C_MASTER_BUSY & i2c_scb3_master_get_status()))
    {
        vTaskDelay(10);
    }
    /* Configure read transaction */
    transfer.buffer       = readDataBuffer;
    transfer.bufferSize   = readSize;
    transfer.xferPending  = false; /* Generate Stop condition the end of transaction */
    /* Initiate read transaction.
    * The ReStart condition is generated to begin this transaction because
    * previous transaction was completed without Stop.
    */
    i2cStatusDecode(i2c_scb3_master_read(&transfer));
    /* Wait for transaction completion */
    while (0UL != (CY_SCB_I2C_MASTER_BUSY & i2c_scb3_master_get_status()))
    {
        vTaskDelay(10);
    }
    return I2C_DRIVER_RW_SUCCESS;
}



i2cDriverState_t i2c_writeBurst(uint8_t address,uint8_t registerAddress,uint8_t* writeData,uint8_t size)
{
    if(hardwareErrorFlag){return I2C_DRIVER_RW_FAILED;}
    cy_stc_scb_i2c_master_xfer_config_t transfer;
    uint8_t *writeBuffer = (uint8_t*)calloc(size,sizeof(uint8_t));
    writeBuffer[0] = registerAddress;
    memcpy((writeBuffer + 1), writeData,size);
    /* Configure write transaction */
    transfer.slaveAddress = address;
    transfer.buffer       = writeBuffer;
    transfer.bufferSize   = size + 1;
    transfer.xferPending  = false; /* Do not generate Stop condition at the end of transaction */
    /* Initiate write transaction.
    * The Start condition is generated to begin this transaction.
    */
    i2cStatusDecode( i2c_scb3_master_write(&transfer));
    /* Wait for transaction completion */
    while (0UL != (CY_SCB_I2C_MASTER_BUSY & i2c_scb3_master_get_status()))
    {
        vTaskDelay(10);
    }
    free(writeBuffer);
    return I2C_DRIVER_RW_SUCCESS;
}



i2cDriverState_t i2c_writeByte(uint8_t address,uint8_t registerAddress,uint8_t writeData)
{
    if(hardwareErrorFlag){return I2C_DRIVER_RW_FAILED;}
    cy_stc_scb_i2c_master_xfer_config_t transfer;
    uint8_t writeBuffer[2UL] = {registerAddress,writeData};
    /* Configure write transaction */
    transfer.slaveAddress = address;
    transfer.buffer       = writeBuffer;
    transfer.bufferSize   = 2;
    transfer.xferPending  = false; /* Do not generate Stop condition at the end of transaction */
    /* Initiate write transaction.
    * The Start condition is generated to begin this transaction.
    */
    i2cStatusDecode( i2c_scb3_master_write(&transfer));
    /* Wait for transaction completion */
    while (0UL != (CY_SCB_I2C_MASTER_BUSY & i2c_scb3_master_get_status()))
    {
        vTaskDelay(10);
    }
    return I2C_DRIVER_RW_SUCCESS;
}



i2cDriverState_t i2c_readByte(uint8_t address,uint8_t registerAddress,uint8_t* readData)
{
    if(hardwareErrorFlag){*readData = 0x00;return;}
    cy_stc_scb_i2c_master_xfer_config_t transfer;
    uint8_t readByte = 0;
    uint8_t writeBuffer[1UL] = {registerAddress};
    /* Configure write transaction */
    transfer.slaveAddress = address;
    transfer.buffer       = writeBuffer;
    transfer.bufferSize   = 1;
    transfer.xferPending  = true; /* Do not generate Stop condition at the end of transaction */
    /* Initiate write transaction.
    * The Start condition is generated to begin this transaction.
    */
    i2cStatusDecode( i2c_scb3_master_write(&transfer));
    /* Wait for transaction completion */
    while (0UL != (CY_SCB_I2C_MASTER_BUSY & i2c_scb3_master_get_status()))
    {
        vTaskDelay(10);
    }
    /* Configure read transaction */
    transfer.buffer       = readData;
    transfer.bufferSize   = 1;
    transfer.xferPending  = false; /* Generate Stop condition the end of transaction */
    /* Initiate read transaction.
    * The ReStart condition is generated to begin this transaction because
    * previous transaction was completed without Stop.
    */
    i2cStatusDecode( i2c_scb3_master_read(&transfer));
    /* Wait for transaction completion */
    while (0UL != (CY_SCB_I2C_MASTER_BUSY & i2c_scb3_master_get_status()))
    {
        vTaskDelay(10);
    }
    return I2C_DRIVER_RW_SUCCESS;
}