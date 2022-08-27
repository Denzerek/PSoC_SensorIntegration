/*
 * tft_api.h
 *
 *  Created on: 27-Aug-2022
 *      Author: AustinA
 */

#ifndef TFT_DRIVER_H_
#define TFT_DRIVER_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"


typedef struct{
	uint8_t res1:8;

	uint8_t ST24:1;
	uint8_t MH:1;
	uint8_t RGB:1;
	uint8_t ML:1;
	uint8_t MV:1;
	uint8_t MX:1;
	uint8_t MY:1;
	uint8_t BSTON:1;

	uint8_t NORON:1;
	uint8_t SLOUT:1;
	uint8_t PTLON:1;
	uint8_t IDMON:1;
	uint8_t IFPF0:1;
	uint8_t IFPF1:1;
	uint8_t IFPF2:1;
	uint8_t ST23:1;

	uint8_t GCS2:1;
	uint8_t TEON:1;
	uint8_t DISON:1;
	uint8_t ST11:1;
	uint8_t ST12:1;
	uint8_t INVON:1;
	uint8_t ST14:1;
	uint8_t ST15:1;

	uint8_t ST0:1;
	uint8_t ST1:1;
	uint8_t ST2:1;
	uint8_t ST3:1;
	uint8_t ST4:1;
	uint8_t TEM:1;
	uint8_t GCS0:1;
	uint8_t GCS1:1;
}tft_display_status_t;



typedef struct{
#if 0
	uint8_t MY:1;
	uint8_t MX:1;
	uint8_t MV:1;
	uint8_t ML:1;
	uint8_t RGB:1;
	uint8_t MH:1;
	uint8_t none:2;

#else

	uint8_t none:2;
	uint8_t MH:1;
	uint8_t RGB:1;
	uint8_t ML:1;
	uint8_t MV:1;
	uint8_t MX:1;
	uint8_t MY:1;
#endif
}madctl_t;

typedef enum{
	TFT_RGB_62K = 0x5,
	TFT_RGB_262K = 0x6,
}rgb_intr_format_e;

typedef enum{
	TFT_BIT_PER_PIXEL_12 = 0x3,
	TFT_BIT_PER_PIXEL_16 = 0x5,
	TFT_BIT_PER_PIXEL_18 = 0x6,
	TFT_BIT_PER_PIXEL_16M = 0x7,
}bits_per_pixel_e;

typedef struct{
	uint8_t res3:2;
	uint8_t BL: 1;
	uint8_t DD: 1;
	uint8_t res2:1;
	uint8_t BCTRL:1;
	uint8_t res1:2;
}dctrl_t;

void tft_row_addr_set(uint16_t YS, uint16_t YE);
void tft_col_addr_set(uint16_t XS, uint16_t XE);
void tft_printDisplayStatus();
void tft_getDisplayStatus(uint8_t* data);
void tft_memoryDataAccessControl(madctl_t* madctl );
void tft_turn_off_display();
void tft_turn_on_display();
void tft_turn_off_inversion();
void tft_turn_on_inversion();
void tft_interface_pixel_format(rgb_intr_format_e rgb_intr_format,bits_per_pixel_e bits_per_pixel);
void tft_sw_reset();
void tft_wake_up_from_sleep();
void tft_turn_off_idle_mode();
void tft_turn_on_idle_mode();
void tft_init_sequence();
void tft_go_to_sleep();
void tft_set_brightness(uint8_t displayBrightness);
uint8_t tft_get_brightness();

#endif /* TFT_DRIVER_H_ */
