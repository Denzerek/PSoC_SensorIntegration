/*
 * sarADC.h
 *
 *  Created on: 07-Apr-2022
 *      Author: AustinA
 */

#ifndef SARADC_H_
#define SARADC_H_



#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"


void sarADC_Init();
uint32_t sarADC_GetResultWord( uint32_t channel);
void sarADC_StartConversion_Continuous();
void sarADC_StartConversion_Single();
uint8_t sarADC_ConversionStatus();
uint16_t getChannel_1_Result();
uint16_t getChannel_0_Result();


#endif /* SARADC_H_ */
