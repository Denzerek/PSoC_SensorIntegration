/*
 * joyStickTask.h
 *
 *  Created on: 15-Apr-2022
 *      Author: AustinA
 */

#ifndef SOURCE_APPLICATION_JOYSTICKTASK_H_
#define SOURCE_APPLICATION_JOYSTICKTASK_H_

#include "sarADC.h"

#define getJoystickRaw_Y()		getChannel_1_Result()
#define getJoystickRaw_X()		getChannel_0_Result()


int16_t getJoystickX();
int16_t getJoystickY();
void joyStickCalibrate();


#endif /* SOURCE_APPLICATION_JOYSTICKTASK_H_ */
