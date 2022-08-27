/*
 * dps3xxDriver.c
 *
 *  Created on: 28-Mar-2022
 *      Author: AustinA
 */




#include "dps3xxDriver.h"
#include "xensiv_dps3xx_mtb.h"
#include "xensiv_dps3xx.h"
#include "i2c.h"

/*******************************************************************************
* Global Variables
********************************************************************************/
/* Context for dps310 */
xensiv_dps3xx_t DPS3xx_Sensor;

/* Declaration for i2c handler */
cyhal_i2c_t i2cm_HW;


void dps3xx_Init()
{
    cy_rslt_t result;

    i2cInit(&i2cm_HW);

    /* Initialize pressure sensor */
    result = xensiv_dps3xx_mtb_init_i2c(&DPS3xx_Sensor, &i2cm_HW,
                                        XENSIV_DPS3XX_I2C_ADDR_DEFAULT);
    if (result != CY_RSLT_SUCCESS)
    {
        printf("\r\n Failed to initialize DPS3xx I2C\r\n");
        return;
    }

    getDPS3xxRevision();
    printf(" DPS3xx Initialized\r\n");


}


uint8_t getDPS3xxRevision()
{
    uint32_t revisionID = 0;

    /* Retrieve the DPS3xx Revision ID and display the same */
    if(xensiv_dps3xx_get_revision_id(&DPS3xx_Sensor,(uint8_t*)&revisionID) == CY_RSLT_SUCCESS)
    {
        printf(" DPS3xx Revision ID = %d\r\n\n",(uint8_t)revisionID);
    }
    else
    {
        printf(" Failed to get Revision ID\r\n");
    }
    return revisionID;

}


uint8_t getDps3xx_Temp_pressure(float * temperature, float * pressure)
{

    /* Read the pressure and temperature data */
    if(xensiv_dps3xx_read(&DPS3xx_Sensor, pressure, temperature) == CY_RSLT_SUCCESS)
    {
    }
    else
    {
        printf("\n Failed to read temperature and pressure data.\r\n");
    }
}
