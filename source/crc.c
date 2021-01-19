/*
 * crc.c
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

/**
 * @file    crc.c
 * @brief   Funkcia na vypocet CRC
 * @details Funkcia na vypocet CRC
 */
#include <crc.h>

/***************************************************************************//**
 * Funkcia vypocitava CRC
 * https://stackoverflow.com/questions/29214301/ios-how-to-calculate-crc-8-dallas-maxim-of-nsdata
 *
 * @param data Pole z ktoreho ma byt vypocitane CRC
 * @param size Dlzka dat
 * @return Vypocitane CRC
 ******************************************************************************/
unsigned char dallas_crc8(const unsigned char * data, const unsigned int size) {
	unsigned char crc = 0;
	for (unsigned int i = 0; i < size; ++i) {
		unsigned char inbyte = data[i];
		for (unsigned char j = 0; j < 8; ++j) {
			unsigned char mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix)
				crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}
