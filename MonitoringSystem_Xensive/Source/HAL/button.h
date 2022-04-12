/*
 * button.h
 *
 *  Created on: 18-Mar-2022
 *      Author: AustinA
 */

#ifndef SOURCE_HAL_BUTTON_H_
#define SOURCE_HAL_BUTTON_H_


#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"


void buttonInit(cy_israddress BUTTON_Isr);
void clearButtonInterrupt();

#endif /* SOURCE_HAL_BUTTON_H_ */
