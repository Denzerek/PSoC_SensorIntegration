/*
 * i2c.c
 *
 *  Created on: 06-Jul-2022
 *      Author: AustinA
 */




#include "i2c.h"




/* Define the I2C master configuration structure */
cyhal_i2c_cfg_t i2c_cfg_master = {
        CYHAL_I2C_MODE_MASTER,
        0,                          /* address is not used for master mode */
        I2C_MASTER_FREQUENCY
};




void i2cInit(cyhal_i2c_t *i2c_handler)
{

    cy_rslt_t result;

    /* Initialize i2c for pressure sensor */
    result = cyhal_i2c_init(&i2c_handler, CYBSP_I2C_SDA, CYBSP_I2C_SCL, NULL);
    if (result != CY_RSLT_SUCCESS)
    {
        printf("\r\nI2C initialization failed\r\n");
        return;
    }


    /* Configure i2c with master configurations */
    result = cyhal_i2c_configure(&i2c_handler, &i2c_cfg_master);
    if (result != CY_RSLT_SUCCESS)
    {
        printf("\r\n Failed to configure I2C\r\n");
        return;
    }


}
