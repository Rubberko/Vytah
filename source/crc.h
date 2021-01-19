/*
 * crc.h
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

/**
 * @file    crc.h
 * @brief   Hlavicka funkcie suboru crc.c
 * @details Hlavicka funkcie
 */
#ifndef CRC_H_
#define CRC_H_

#include <definitions.h>

/***************************************************************************//**
 * Hlavicky funkcii
 ******************************************************************************/
unsigned char dallas_crc8(const unsigned char * data, const unsigned int size);

#endif /* CRC_H_ */
