/*
 * tft_app.h
 *
 *  Created on: 27-Aug-2022
 *      Author: AustinA
 */

#ifndef TFT_APP_H_
#define TFT_APP_H_



#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "tft_driver.h"


#define VERBOSE_REG_DEBUG	1

void tft_printDisplayStatus();
void tft_print_disp_ID();
void tft_selfTest();
void tft_print_display_ctrl();
void tft_init_sequence();
void tft_brightness_read();

void pixelSet(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint32_t size);


#endif /* TFT_APP_H_ */
