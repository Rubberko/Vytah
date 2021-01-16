/*
 * movement.h
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */


#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <definitions.h>

extern bool dirUp, isMoving, dirDown, lastDirDown, floor_0;

void goUp(void);
void goSlowUp(void);
void goSlowDown(void);
void closeDoor(void);
void openDoor(void);
void stopMotor(void);
void goDown(void);
void goToDefault(void);


#endif /* MOVEMENT_H_ */
