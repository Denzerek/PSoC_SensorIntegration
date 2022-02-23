#include "i2c_driver.h"
#include "i2c.h"

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
    {CY_SCB_I2C_SUCCESS,"CY_SCB_I2C_SUCCESS"},
};



void i2cStatusDecode( cy_en_scb_i2c_status_t i2cStatus)
{
    if(CY_SCB_I2C_SUCCESS == i2cStatus){return;}
    for(int i = 0; i < (sizeof(i2cEvents)/sizeof(i2cEvents_s)) ; i++)
    {
        if((i2cEvents[i].i2cEvent & i2cStatus)  == i2cEvents[i].i2cEvent)
        {
            I2CDRIVER_PRINT(i2cEvents[i].i2cMessage);
            return;
        }
    }
    
    I2CDRIVER_PRINTF("Unknown I2C Error %X",i2cStatus);
}

void i2c_driver_init()
{
    i2c_init();
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
    i2cStatusDecode( i2c_scb3_master_write(&transfer));

    /* Wait for transaction completion */
    while (0UL != (CY_SCB_I2C_MASTER_BUSY & i2c_scb3_master_get_status()))
    {
        vTaskDelay(1);
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
    }
}


void i2c_writeBurst(uint8_t address,uint8_t registerAddress,uint8_t* writeData,uint8_t size)
{
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
    }

    free(writeBuffer);
}


void i2c_writeByte(uint8_t address,uint8_t registerAddress,uint8_t writeData)
{
    
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
    }
}



void i2c_readByte(uint8_t address,uint8_t registerAddress,uint8_t* readData)
{
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
    }
}