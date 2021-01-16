/*
 * ledInfo.h
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

#ifndef LEDINFO_H_
#define LEDINFO_H_

#include <definitions.h>

extern bool dirDown, dirUp;
extern uint8_t actLimSwitch;

void displayData(void);
void ledControl(uint8_t ledId, bool state);

#endif /* LEDINFO_H_ */
