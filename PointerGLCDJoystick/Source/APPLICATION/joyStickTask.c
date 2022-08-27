/*
 * joyStickTask.c
 *
 *  Created on: 15-Apr-2022
 *      Author: AustinA
 */




#include "joyStickTask.h"

static volatile uint16_t JS_x_ref;
static volatile uint16_t JS_y_ref;

void joyStickCalibrate()
{
	JS_x_ref = getJoystickRaw_X();
	JS_y_ref = getJoystickRaw_Y();
}


int16_t getJoystickX()
{
	return (JS_x_ref - getJoystickRaw_X());
}

int16_t getJoystickY()
{
	return (JS_y_ref - getJoystickRaw_Y());

}
