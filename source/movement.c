/*
 * movement.c
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

#include <movement.h>

void goUp(void) {
	//if (doorClosed) {
	dirUp = true;
	isMoving = true;
	//uint8_t crcElements[] = { 0xf1, 0x00, 0x02, 0x64, 0x00, 0x00, 0x00 };
	uint8_t msg[] =
			{ 0xa0, 0xf1, 0x00, 0x05, 0x02, 0x64, 0x00, 0x00, 0x00, 0x64 };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

void goSlowUp(void) {
	//uint8_t crcElements[] = { 0xf1, 0x00, 0x02, 0x32, 0x00, 0x00, 0x00 };
	uint8_t msg[] =
			{ 0xa0, 0xf1, 0x00, 0x05, 0x02, 0x32, 0x00, 0x00, 0x00, 0xB5 };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

void goSlowDown(void) {
	//uint8_t crcElements[] = { 0xf1, 0x00, 0x02, 0xce, 0xff, 0xff, 0xff };
	uint8_t msg[] =
			{ 0xa0, 0xf1, 0x00, 0x05, 0x02, 0xce, 0xff, 0xff, 0xff, 0xB0 };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

void closeDoor(void) {
	uint8_t msg[] = { 0xA0, 0xF0, 0x00, 0x01, 0x01, 0xD3 };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

void openDoor(void) {
	uint8_t msg[] = { 0xA0, 0xF0, 0x00, 0x01, 0x00, 0x8D };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

void stopMotor(void) {
	uint8_t msg[] = { 0xa0, 0xf1, 0x00, 0x01, 0x01, 0x78 };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	isMoving = false;
	if (dirDown)
		lastDirDown = true;
	else
		lastDirDown = false;
	dirDown = false;
	dirUp = false;
	displayData();
}

void goDown(void) {
	dirDown = true;
	isMoving = true;
	//uint8_t crcElements[] = { 0xf1, 0x00, 0x02, 0x9c, 0xff, 0xff, 0xff };
	uint8_t msg[] =
			{ 0xa0, 0xf1, 0x00, 0x05, 0x02, 0x9c, 0xff, 0xff, 0xff, 0x6f };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

void goToDefault(void) {
	lastDirDown = true;
	readyForNewData();
	isMoving = false;
	ready = false;
	closeDoor();
	delay(5);
	floor_0 = true;
	goDown();
}

