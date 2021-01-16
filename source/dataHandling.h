/*
 * dataHandling.h
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

#ifndef DATAHANDLING_H_
#define DATAHANDLING_H_

#include <definitions.h>

extern volatile uint8_t data[256], dataIndex, recvDataSize;
extern volatile bool dataReady;
extern uint8_t actLimSwitch, prevLimSwitch;
extern bool floor_0, floor_1, floor_2, floor_3, floor_4, doorClosed, isMoving;

void dataProcessing(uint8_t device);
void sendAck(uint8_t device);
void goToFloor(uint8_t floor);
void processAck(uint8_t device);
void delay(int millis);
void readyForNewData(void);

#endif /* DATAHANDLING_H_ */
