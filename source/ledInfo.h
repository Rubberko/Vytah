/*
 * ledInfo.h
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

/**
 * @file    ledInfo.h
 * @brief   Hlavicky funkcii suboru ledInfo.c
 * @details Hlavicky funkcii, deklaracie premennych urcujucich smer jazdy kabiny
 * 			a aktualny limit switch
 */
#ifndef LEDINFO_H_
#define LEDINFO_H_

#include <definitions.h>

/***************************************************************************//**
 * Deklaracie premennych
 ******************************************************************************/
extern bool dirDown, dirUp;
extern uint8_t actLimSwitch;

/***************************************************************************//**
 * Hlavicky funkcii
 ******************************************************************************/
void displayData(void);
void ledControl(uint8_t ledId, bool state);

#endif /* LEDINFO_H_ */
