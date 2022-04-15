/***********************************************************************************************//**
 * \file xensiv_dps3xx_mtb.c
 *
 * Description: This file contains ModusToolbox specific functions for the DPS3xx pressure sensors.
 ***************************************************************************************************
 * \copyright
 * Copyright 2021 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **************************************************************************************************/


#include "xensiv_dps3xx_mtb.h"
#include "cyhal_system.h"
#include "i2c_driver.h"


//--------------------------------------------------------------------------------------------------
// _xensiv_dps3xx_reg_read
//--------------------------------------------------------------------------------------------------
static cy_rslt_t _xensiv_dps3xx_mtb_reg_read(void* context, uint16_t timeout, uint8_t i2c_addr,
                                             uint8_t reg_addr, uint8_t* data, uint8_t length)
{

    cy_rslt_t rc = !CY_RSLT_SUCCESS;
	i2cDriverState_t status = i2c_readBurst(i2c_addr,reg_addr,1,data,length);
	if(status == I2C_DRIVER_RW_SUCCESS)
		rc = CY_RSLT_SUCCESS;
	else
		rc = 1;

	return rc;
}


//--------------------------------------------------------------------------------------------------
// _xensiv_dps3xx_reg_write
//--------------------------------------------------------------------------------------------------
static cy_rslt_t _xensiv_dps3xx_mtb_reg_write(void* context, uint16_t timeout, uint8_t i2c_addr,
                                              uint8_t reg_addr, uint8_t* data, uint8_t length)
{

    uint8_t write_data[5];
    i2cDriverState_t status;
    CY_ASSERT(length < sizeof(write_data));

    // check length
    cy_rslt_t rc = XENSIV_DPS3XX_RSLT_ERR_WRITE_TOO_LARGE;
    if (length < sizeof(write_data))
    {
//        write_data[0] = reg_addr;
//        memcpy(&write_data[1], data, length);
//        rc = cyhal_i2c_master_write(i2c_dev, i2c_addr, write_data, length + 1, timeout, true);

    	status = i2c_writeBurst(i2c_addr,reg_addr, data, length);
    	if(status == I2C_DRIVER_RW_SUCCESS)
    		rc = CY_RSLT_SUCCESS;
    	else
    		rc = 1;
    }
    return rc;
}


//--------------------------------------------------------------------------------------------------
// xensiv_dps3xx_mtb_init_i2c
//--------------------------------------------------------------------------------------------------
cy_rslt_t xensiv_dps3xx_mtb_init_i2c(xensiv_dps3xx_t* dev, cyhal_i2c_t* i2c_dev,
                                     xensiv_dps3xx_i2c_addr_t i2c_addr)
{
    xensiv_dps3xx_i2c_comm_t functions =
    {
        .read    = _xensiv_dps3xx_mtb_reg_read,
        .write   = _xensiv_dps3xx_mtb_reg_write,
        .delay   = CyDelay,//cyhal_system_delay_ms,
        .context = i2c_dev
    };
    return xensiv_dps3xx_init_i2c(dev, &functions, i2c_addr);
}


