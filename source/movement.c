/*
 * movement.c
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

/**
 * @file    movement.c
 * @brief   Funkcie na riadenie pohybu vytahu
 * @details Funkcie na riadenie pohybov dveri a motora.
 *
 */
#include <movement.h>

/***************************************************************************//**
 * Funkcia posle vytah smerom hore, nastavi flagy potrebne pre dalsie funkcie
 ******************************************************************************/
void goUp(void) {
	dirUp = true;
	isMoving = true;
	uint8_t msg[] =
			{ 0xa0, 0xf1, 0x00, 0x05, 0x02, 0x64, 0x00, 0x00, 0x00, 0x64 };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

/***************************************************************************//**
 * Funkcia posle vytah smerom dolu, nastavi flagy potrebne pre dalsie funkcie
 ******************************************************************************/
void goDown(void) {
	dirDown = true;
	isMoving = true;
	uint8_t msg[] =
			{ 0xa0, 0xf1, 0x00, 0x05, 0x02, 0x9c, 0xff, 0xff, 0xff, 0x6f };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

/***************************************************************************//**
 * Funkcia zastavi motor vytahu, nastavi flagy potrebne pre dalsie funkcie
 ******************************************************************************/
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

/***************************************************************************//**
 * Funkcia zatvori dvere vytahu
 ******************************************************************************/
void closeDoor(void) {
	uint8_t msg[] = { 0xA0, 0xF0, 0x00, 0x01, 0x01, 0xD3 };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

/***************************************************************************//**
 * Funkcia otvori dvere vytahu
 ******************************************************************************/
void openDoor(void) {
	uint8_t msg[] = { 0xA0, 0xF0, 0x00, 0x01, 0x00, 0x8D };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

/***************************************************************************//**
 * Funkcia inicializuje vytah na prizemie a nastavi potrebne premenne
 ******************************************************************************/
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

