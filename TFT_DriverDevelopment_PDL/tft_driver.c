/***************************************************************************//**
* \file st7789v_pdl.c
*
* \brief
*    This is display software i8080 interface source file
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"


/*******************************************************************************
 * Changes data bus GPIO pins drive mode to digital Hi-Z with enabled input
 * buffer.
 *******************************************************************************/
static inline void data_io_set_input(void)
{
    /* enable input */

    Cy_GPIO_SetDrivemode(TFT_DISP_DB8_PORT, TFT_DISP_DB8_NUM, CY_GPIO_DM_HIGHZ);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB9_PORT, TFT_DISP_DB9_NUM, CY_GPIO_DM_HIGHZ);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB10_PORT, TFT_DISP_DB10_NUM, CY_GPIO_DM_HIGHZ);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB11_PORT, TFT_DISP_DB11_NUM, CY_GPIO_DM_HIGHZ);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB12_PORT, TFT_DISP_DB12_NUM, CY_GPIO_DM_HIGHZ);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB13_PORT, TFT_DISP_DB13_NUM, CY_GPIO_DM_HIGHZ);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB14_PORT, TFT_DISP_DB14_NUM, CY_GPIO_DM_HIGHZ);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB15_PORT, TFT_DISP_DB15_NUM, CY_GPIO_DM_HIGHZ);

}


/*******************************************************************************
 * Changes data bus GPIO pins drive mode to strong drive with disabled input
 * buffer.
 *******************************************************************************/
static inline void data_io_set_output(void)
{
    Cy_GPIO_SetDrivemode(TFT_DISP_DB8_PORT, TFT_DISP_DB8_NUM, CY_GPIO_DM_STRONG);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB9_PORT, TFT_DISP_DB9_NUM, CY_GPIO_DM_STRONG);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB10_PORT, TFT_DISP_DB10_NUM, CY_GPIO_DM_STRONG);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB11_PORT, TFT_DISP_DB11_NUM, CY_GPIO_DM_STRONG);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB12_PORT, TFT_DISP_DB12_NUM, CY_GPIO_DM_STRONG);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB13_PORT, TFT_DISP_DB13_NUM, CY_GPIO_DM_STRONG);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB14_PORT, TFT_DISP_DB14_NUM, CY_GPIO_DM_STRONG);
	Cy_GPIO_SetDrivemode(TFT_DISP_DB15_PORT, TFT_DISP_DB15_NUM, CY_GPIO_DM_STRONG);
}


/*******************************************************************************
 * Writes one byte of data to the software i8080 interface.
 *******************************************************************************/
static inline void write_data(uint8_t data)
{

    Cy_GPIO_Write(TFT_DISP_DB8_PORT, TFT_DISP_DB8_NUM,  data     & 0x01);
    Cy_GPIO_Write(TFT_DISP_DB9_PORT, TFT_DISP_DB9_NUM, (data>>1) & 0x01);
    Cy_GPIO_Write(TFT_DISP_DB10_PORT, TFT_DISP_DB10_NUM, (data>>2) & 0x01);
    Cy_GPIO_Write(TFT_DISP_DB11_PORT, TFT_DISP_DB11_NUM, (data>>3) & 0x01);
    Cy_GPIO_Write(TFT_DISP_DB12_PORT, TFT_DISP_DB12_NUM, (data>>4) & 0x01);
    Cy_GPIO_Write(TFT_DISP_DB13_PORT, TFT_DISP_DB13_NUM, (data>>5) & 0x01);
    Cy_GPIO_Write(TFT_DISP_DB14_PORT, TFT_DISP_DB14_NUM, (data>>6) & 0x01);
    Cy_GPIO_Write(TFT_DISP_DB15_PORT, TFT_DISP_DB15_NUM, (data>>7) & 0x01);

    Cy_GPIO_Write(TFT_DISP_WR_L_PORT, TFT_DISP_WR_L_NUM, 0);
    Cy_GPIO_Write(TFT_DISP_WR_L_PORT, TFT_DISP_WR_L_NUM, 1);
}


/*******************************************************************************
 * Reads one byte of data from the software i8080 interface.
 *******************************************************************************/
static inline uint8_t read_data(void)
{
    uint8_t data = 0u;

    Cy_GPIO_Write(TFT_DISP_RD_L_PORT, TFT_DISP_RD_L_NUM, 0);

    data |= Cy_GPIO_Read(TFT_DISP_DB8_PORT, TFT_DISP_DB8_NUM);
    data |= Cy_GPIO_Read(TFT_DISP_DB9_PORT, TFT_DISP_DB9_NUM) << 1;
    data |= Cy_GPIO_Read(TFT_DISP_DB10_PORT, TFT_DISP_DB10_NUM) << 2;
    data |= Cy_GPIO_Read(TFT_DISP_DB11_PORT, TFT_DISP_DB11_NUM) << 3;
    data |= Cy_GPIO_Read(TFT_DISP_DB12_PORT, TFT_DISP_DB12_NUM) << 4;
    data |= Cy_GPIO_Read(TFT_DISP_DB13_PORT, TFT_DISP_DB13_NUM) << 5;
    data |= Cy_GPIO_Read(TFT_DISP_DB14_PORT, TFT_DISP_DB14_NUM) << 6;
    data |= Cy_GPIO_Read(TFT_DISP_DB15_PORT, TFT_DISP_DB15_NUM) << 7;



    Cy_GPIO_Write(TFT_DISP_RD_L_PORT, TFT_DISP_RD_L_NUM, 1);

    return data;
}



/*******************************************************************************
 * Sets value of the display Reset pin.
 *******************************************************************************/
void st7789v_pdl_write_reset_pin(bool value)
{
    Cy_GPIO_Write(TFT_DISP_RST_L_PORT, TFT_DISP_RST_L_NUM, value);
}


/*******************************************************************************
 * Writes one byte of data to the software i8080 interface with the LCD_DC pin
 *******************************************************************************/
void st7789v_pdl_write_command(uint8_t data)
{
    Cy_GPIO_Write(TFT_DISP_D_C_PORT, TFT_DISP_D_C_NUM, 0);
    write_data(data);
}


/*******************************************************************************
 * Writes one byte of data to the software i8080 interface with the LCD_DC pin
 *******************************************************************************/
void st7789v_pdl_write_data(uint8_t data)
{
    Cy_GPIO_Write(TFT_DISP_D_C_PORT, TFT_DISP_D_C_NUM, 1);
    write_data(data);
}


/*******************************************************************************
 * Writes multiple command bytes to the software i8080 interface with the LCD_DC
 * pin set to 0.
 *******************************************************************************/
void st7789v_pdl_write_command_stream(uint8_t *data, int num)
{
    int i;

    Cy_GPIO_Write(TFT_DISP_D_C_PORT, TFT_DISP_D_C_NUM, 0);

    for (i = 0; i < num; i++)
    {
        write_data(data[i]);
    }
}


/*******************************************************************************
 * Writes multiple bytes of data to the software i8080 interface with the LCD_DC
 * pin set to 1.
 *******************************************************************************/
void st7789v_pdl_write_data_stream(uint8_t *data, int num)
{
    int i;

    Cy_GPIO_Write(TFT_DISP_D_C_PORT, TFT_DISP_D_C_NUM, 1);

    for (i = 0; i < num; i++)
    {
        write_data(data[i]);
    }
}


/*******************************************************************************
 * Reads one byte of data from the software i8080 interface with the LCD_DC pin
 * set to 1.
 *******************************************************************************/
uint8_t st7789v_pdl_read_data(void)
{
    uint8_t data;

    Cy_GPIO_Write(TFT_DISP_D_C_PORT, TFT_DISP_D_C_NUM, 1);

    data_io_set_input();
    data = read_data();
    data_io_set_output();

    return data;
}


/*******************************************************************************
 * Reads multiple bytes of data from the software i8080 interface with the LCD_DC
 * pin set to 1.
 *******************************************************************************/
void st7789v_pdl_read_data_stream(uint8_t *data, int num)
{
    Cy_GPIO_Write(TFT_DISP_D_C_PORT, TFT_DISP_D_C_NUM, 1);

    data_io_set_input();

    for (int i = 0; i < num; i++)
    {
        data[i] = read_data();
    }

    data_io_set_output();
}



/* [] END OF FILE */
