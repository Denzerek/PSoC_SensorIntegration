/*
 * tft_api.c
 *
 *  Created on: 27-Aug-2022
 *      Author: AustinA
 */


#include <tft_driver.h>
#include <tft_hal.h>

#define BYTE_INVERT(var) 	((var & 0x01) << 7 )| ((var & 0x2 )<<5 ) | ((var &0x4) <<3) | ((var &0x8) <<1) | \
							( (var & 0x80) >> 7 )| ((var & 40 )>>5 ) | ((var &0x20) >> 3) | ((var &0x10) >>1)


void tft_getDisplayStatus(uint8_t* data)
{
	st7789v_pdl_write_command(0x09);
	st7789v_pdl_read_data_stream(data,5);
}

uint32_t tft_get_disp_ID()
{
	uint8_t temp1 = 0;
	uint8_t temp2 = 0;
	uint8_t temp3 = 0;
	st7789v_pdl_write_command(0x04);
	st7789v_pdl_read_data();			//	Dummy read
	temp1 = st7789v_pdl_read_data();
	temp1 = BYTE_INVERT(temp1);
	temp2 = st7789v_pdl_read_data();
	temp2 = BYTE_INVERT(temp2);
	temp3 = st7789v_pdl_read_data();
	temp3 = BYTE_INVERT(temp3);
	return temp1 | (temp2 << 8) | (temp3 << 16 );
}

void tft_memoryDataAccessControl(madctl_t* madctl )
{
	st7789v_pdl_write_command(0x36);
	st7789v_pdl_write_data(*((uint8_t*)madctl));
}

void tft_turn_on_display()
{
	st7789v_pdl_write_command(0x29);
}
void tft_turn_off_display()
{
	st7789v_pdl_write_command(0x28);
}

void tft_turn_on_inversion()
{
	st7789v_pdl_write_command(0x21);
}
void tft_turn_off_inversion()
{
	st7789v_pdl_write_command(0x20);
}

void tft_turn_on_idle_mode()
{
	st7789v_pdl_write_command(0x39);
}
void tft_turn_off_idle_mode()
{
	st7789v_pdl_write_command(0x38);
}

void tft_interface_pixel_format(rgb_intr_format_e rgb_intr_format,bits_per_pixel_e bits_per_pixel)
{
	st7789v_pdl_write_command(0x3A);
	st7789v_pdl_write_data(rgb_intr_format << 4 | bits_per_pixel);
}

void tft_sw_reset()
{
	st7789v_pdl_write_command(0x01);
}

void tft_go_to_sleep()
{
	st7789v_pdl_write_command(0x10);
}

void tft_wake_up_from_sleep()
{
	st7789v_pdl_write_command(0x11);
}

void tft_col_addr_set(uint16_t XS, uint16_t XE)
{
//	XS = (BYTE_INVERT(((uint8_t*)&XS)[1]) << 8) | BYTE_INVERT(((uint8_t*)&XS)[0]);
//	XE = (BYTE_INVERT(((uint8_t*)&XE)[1]) << 8) | BYTE_INVERT(((uint8_t*)&XE)[0]);
	uint8_t datastream[] = {
			((uint8_t*)&XS)[1],((uint8_t*)&XS)[0],
			((uint8_t*)&XE)[1],((uint8_t*)&XE)[0]
	};
	st7789v_pdl_write_command(0x2A);
	st7789v_pdl_write_data_stream(datastream,4);
}

void tft_row_addr_set(uint16_t YS, uint16_t YE)
{
//	YS = (BYTE_INVERT(((uint8_t*)&YS)[1]) << 8) | BYTE_INVERT(((uint8_t*)&YS)[0]);
//	YE = (BYTE_INVERT(((uint8_t*)&YE)[1]) << 8) | BYTE_INVERT(((uint8_t*)&YE)[0]);
	uint8_t datastream[] = {
			((uint8_t*)&YS)[1],((uint8_t*)&YS)[0],
			((uint8_t*)&YE)[1],((uint8_t*)&YE)[0]
	};
	st7789v_pdl_write_command(0x2B);
	st7789v_pdl_write_data_stream(datastream,4);
}

void tft_ram_write(uint8_t* data,uint32_t size)
{
	st7789v_pdl_write_data_stream(data,size);
}

void tft_set_brightness(uint8_t displayBrightness)
{
	st7789v_pdl_write_command(0x51);
	st7789v_pdl_write_data(displayBrightness);
}

uint8_t tft_get_brightness()
{
	st7789v_pdl_write_command(0x52);
	st7789v_pdl_read_data();
	return st7789v_pdl_read_data();
}

uint8_t tft_read_display_ctrl()
{
	st7789v_pdl_write_command(0x54);
	st7789v_pdl_read_data();
	return st7789v_pdl_read_data();
}


void tft_write_display_ctrl(dctrl_t* dctrl)
{
	st7789v_pdl_write_command(0x53);
	st7789v_pdl_write_data(*((uint8_t*)dctrl));
}





