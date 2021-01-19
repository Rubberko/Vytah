/*
 * movement.h
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

/**
 * @file    movement.h
 * @brief   Hlavicky funkcii suboru movement.c
 * @details Hlavicky funkcii, deklaracie premennych (flagov) potrebnych
 * 			pre dalsie funkcie.
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <definitions.h>

/***************************************************************************//**
 * Deklaracie premennych
 ******************************************************************************/
extern bool dirUp, isMoving, dirDown, lastDirDown, floor_0, ready;

/***************************************************************************//**
 * Hlavicky funkcii
 ******************************************************************************/
void goUp(void);
void closeDoor(void);
void openDoor(void);
void stopMotor(void);
void goDown(void);
void goToDefault(void);


#endif /* MOVEMENT_H_ */
