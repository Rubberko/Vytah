/*
 * ledInfo.c
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

/**
 * @file    ledInfo.c
 * @brief   Funkcie na riadenie led a zobrazenie dat na dispeji
 * @details Funkcia na riadenie led v kabine vytahu aj na jednotlivych poschodiach,
 * 			funkcia na zobrazovanie aktualneho poschodia a smeru jazdy na displeji.
 */
#include <ledInfo.h>

/***************************************************************************//**
 * Funkcia zobrazuje na displej smer jazdy kabiny vytahu,
 * tiez zobrazuje aktualne poschodia na ktorom sa kabina nachadza.
 ******************************************************************************/
void displayData(void) {
	uint8_t tmp;
	switch (actLimSwitch) {
	case LIMIT_SWITCH_0:
		tmp = 80;
		break;
	case LIMIT_SWITCH_1:
		tmp = 49;
		break;
	case LIMIT_SWITCH_2:
		tmp = 50;
		break;
	case LIMIT_SWITCH_3:
		tmp = 51;
		break;
	case LIMIT_SWITCH_4:
		tmp = 52;
		break;

	}
	if (dirUp) {
		uint8_t crcElements[] = { 0x30, 0x00, 0x01, tmp };
		uint8_t msg[] = { 0xa0, 0x30, 0x00, 0x02, 0x01, tmp, dallas_crc8(
				crcElements, sizeof(crcElements)) };
		LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	} else if (dirDown) {
		uint8_t crcElements[] = { 0x30, 0x00, 0x02, tmp };
		uint8_t msg[] = { 0xa0, 0x30, 0x00, 0x02, 0x02, tmp, dallas_crc8(
				crcElements, sizeof(crcElements)) };
		LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	} else {
		uint8_t crcElements[] = { 0x30, 0x00, 0x03, tmp };
		uint8_t msg[] = { 0xa0, 0x30, 0x00, 0x02, 0x03, tmp, dallas_crc8(
				crcElements, sizeof(crcElements)) };
		LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	}
}

/***************************************************************************//**
 * Funkcia zasvecuje a zhasina led
 ******************************************************************************/
void ledControl(uint8_t ledId, bool state) {
	if (state) {
		uint8_t crcElements[] = { ledId, 0x00, 0x01 };
		uint8_t msg[] = { 0xA0, ledId, 0x00, 0x01, 0x01, dallas_crc8(
				crcElements, sizeof(crcElements)) };
		LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	} else {
		uint8_t crcElements[] = { ledId, 0x00, 0x00 };
		uint8_t msg[] = { 0xA0, ledId, 0x00, 0x01, 0x00, dallas_crc8(
				crcElements, sizeof(crcElements)) };
		LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	}
}
