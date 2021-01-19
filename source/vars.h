/*
 * vars.h
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

/**
 * @file    vars.h
 * @brief   Deklaracie globalnych premennych
 * @details Deklaracie globalnych premennych, ktore su vyuzivane
 * 			pri riadeni vytahu.
 */
#ifndef VARS_H_
#define VARS_H_

/***************************************************************************//**
 * Deklaracie premennych
 ******************************************************************************/
volatile uint8_t data[256];
volatile uint8_t dataIndex;
volatile uint8_t recvDataSize;
volatile bool dataReady;
uint8_t actLimSwitch;
uint8_t prevLimSwitch;

bool ready;
bool doorClosed;
bool isMoving;
bool dirUp;
bool dirDown;
bool lastDirDown;
bool slowSpeed;
bool floor_0;
bool floor_1;
bool floor_2;
bool floor_3;
bool floor_4;

#endif /* VARS_H_ */
