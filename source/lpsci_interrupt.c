/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "fsl_lpsci.h"

#include "clock_config.h"
#include "pin_mux.h"
#include <time.h>
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPSCI UART0
#define DEMO_LPSCI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_LPSCI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_LPSCI_IRQn UART0_IRQn
#define DEMO_LPSCI_IRQHandler UART0_IRQHandler

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 16

#define BUTTON_0 0xc0
#define BUTTON_1 0xc1
#define BUTTON_2 0xc2
#define BUTTON_3 0xc3
#define BUTTON_4 0xc4

#define BUTTON_0_ELEVATOR 0xb0
#define BUTTON_1_ELEVATOR 0xb1
#define BUTTON_2_ELEVATOR 0xb2
#define BUTTON_3_ELEVATOR 0xb3
#define BUTTON_4_ELEVATOR 0xb4

#define LIMIT_SWITCH_0 0xe0
#define LIMIT_SWITCH_1 0xe1
#define LIMIT_SWITCH_2 0xe2
#define LIMIT_SWITCH_3 0xe3
#define LIMIT_SWITCH_4 0xe4

#define DOOR 0xf0
#define MOTOR 0xf1
/*! @brief Ring buffer to save received data. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_tipString[] =
		"LPSCI functional API interrupt example\r\nBoard receives characters then sends them out\r\nNow please input:\r\n";

/*
 Ring buffer for data input and output, in this example, input data are saved
 to ring buffer in IRQ handler. The main function polls the ring buffer status,
 if there are new data, then send them out.
 Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
 Ring buffer empty: (rxIndex == txIndex)
 */
uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */
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

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_LPSCI_IRQHandler(void) {
	if ((kLPSCI_RxDataRegFullFlag) & LPSCI_GetStatusFlags(DEMO_LPSCI)) {
		data[dataIndex] = LPSCI_ReadByte(DEMO_LPSCI);
		/*
		 if (dataIndex == (data[3] + 4)) {
		 dataReady = true;
		 }
		 dataIndex++;
		 */

		//if (dataIndex == 0 && !((data[0] & 0xFE) == 0xA0)) {
		//return;
		//}
		if (dataIndex == 3) {
			recvDataSize = data[dataIndex];
		}
		if (dataIndex == recvDataSize + 4) {
			dataReady = true;
		}
		dataIndex++;

	}
}

void goDown(void) {
	dirDown = true;
	isMoving = true;
	//uint8_t crcElements[] = { 0xf1, 0x00, 0x02, 0x9c, 0xff, 0xff, 0xff };
	uint8_t msg[] =
			{ 0xa0, 0xf1, 0x00, 0x05, 0x02, 0x9c, 0xff, 0xff, 0xff, 0x6f };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

void goUp(void) {
	//if (doorClosed) {
	dirUp = true;
	isMoving = true;
	//uint8_t crcElements[] = { 0xf1, 0x00, 0x02, 0x64, 0x00, 0x00, 0x00 };
	uint8_t msg[] =
			{ 0xa0, 0xf1, 0x00, 0x05, 0x02, 0x64, 0x00, 0x00, 0x00, 0x64 };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	//} else {
	//	closeDoor();
	//	delay(150);
	//	goUp();
	//}
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

/*
 https://www.geeksforgeeks.org/time-delay-c/
 */
void delay(int millis) {
	for (long i = 0; i < millis * 10000; i++)
		__asm("nop");
}

/*
 https://stackoverflow.com/questions/29214301/ios-how-to-calculate-crc-8-dallas-maxim-of-nsdata
 */

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

void sendAck(uint8_t device) {
	uint8_t crcElements[] = { device, 0x00 };
	uint8_t msg[] = { 0xa1, device, 0x00, 0x00, dallas_crc8(crcElements,
			sizeof(crcElements)) };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
}

void dataProcessing(uint8_t device) {
	sendAck(data[2]);
	delay(5);
	displayData();
	switch (device) {
	case LIMIT_SWITCH_0:
		if (!(actLimSwitch == LIMIT_SWITCH_0)) {
			prevLimSwitch = actLimSwitch;
			actLimSwitch = LIMIT_SWITCH_0;
		}
		/*
		 if (actLimSwitch == 0) {
		 actLimSwitch = LIMIT_SWITCH_0;
		 } else {
		 prevLimSwitch = actLimSwitch;
		 actLimSwitch = LIMIT_SWITCH_0;
		 }
		 */
		if (floor_0 && data[4] == 0x01) {
			//goSlowDown();
			delay(20);
			stopMotor();
			floor_0 = false;
			ledControl(0x10, false);
			ledControl(0x20, false);
			delay(200);
			openDoor();
		}
		break;
	case LIMIT_SWITCH_1:
		if (!(actLimSwitch == LIMIT_SWITCH_1)) {
			prevLimSwitch = actLimSwitch;
			actLimSwitch = LIMIT_SWITCH_1;
		}
		if (floor_1 && data[4] == 0x01) {
			delay(20);
			stopMotor();
			floor_1 = false;
			ledControl(0x11, false);
			ledControl(0x21, false);
			delay(200);
			openDoor();
		}
		break;
	case LIMIT_SWITCH_2:
		if (!(actLimSwitch == LIMIT_SWITCH_2)) {
			prevLimSwitch = actLimSwitch;
			actLimSwitch = LIMIT_SWITCH_2;
		}
		if (floor_2 && data[4] == 0x01) {
			delay(20);
			stopMotor();
			floor_2 = false;
			ledControl(0x12, false);
			ledControl(0x22, false);
			delay(200);
			openDoor();
		}
		break;
	case LIMIT_SWITCH_3:
		if (!(actLimSwitch == LIMIT_SWITCH_3)) {
			prevLimSwitch = actLimSwitch;
			actLimSwitch = LIMIT_SWITCH_3;
		}
		if (floor_3 && data[4] == 0x01) {
			delay(20);
			stopMotor();
			floor_3 = false;
			ledControl(0x13, false);
			ledControl(0x23, false);
			delay(200);
			openDoor();
		}
		break;
	case LIMIT_SWITCH_4:
		if (!(actLimSwitch == LIMIT_SWITCH_4)) {
			prevLimSwitch = actLimSwitch;
			actLimSwitch = LIMIT_SWITCH_4;
		}
		if (floor_4 && data[4] == 0x01) {
			delay(20);
			stopMotor();
			floor_4 = false;
			ledControl(0x14, false);
			ledControl(0x24, false);
			delay(200);
			openDoor();
		}
		break;
	case BUTTON_0:
		closeDoor();
		delay(200);
		if (actLimSwitch > LIMIT_SWITCH_0) {
			goDown();
		} else {
			goUp();
		}
		ledControl(0x10, true);
		goToFloor(0);
		break;
	case BUTTON_1:
		closeDoor();
		ledControl(0x11, true);
		delay(200);
		if (actLimSwitch > LIMIT_SWITCH_1) {
			goDown();
		} else {
			goUp();
		}
		goToFloor(1);
		break;
	case BUTTON_2:
		closeDoor();
		ledControl(0x12, true);
		delay(200);
		if (actLimSwitch > LIMIT_SWITCH_2) {
			goDown();
		} else {
			goUp();
		}
		goToFloor(2);
		break;
	case BUTTON_3:
		closeDoor();
		ledControl(0x13, true);
		delay(200);
		if (actLimSwitch > LIMIT_SWITCH_3) {
			goDown();
		} else {
			goUp();
		}
		goToFloor(3);
		break;
	case BUTTON_4:
		closeDoor();
		ledControl(0x14, true);
		delay(200);
		if (actLimSwitch > LIMIT_SWITCH_4) {
			goDown();
		} else {
			goUp();
		}
		goToFloor(4);
		break;
	case BUTTON_0_ELEVATOR:
		closeDoor();
		ledControl(0x20, true);
		delay(200);
		if (actLimSwitch > LIMIT_SWITCH_0) {
			goDown();
		} else {
			goUp();
		}
		goToFloor(0);
		break;
	case BUTTON_1_ELEVATOR:
		closeDoor();
		ledControl(0x21, true);
		delay(200);
		if (actLimSwitch > LIMIT_SWITCH_1) {
			goDown();
		} else {
			goUp();
		}
		goToFloor(1);
		break;
	case BUTTON_2_ELEVATOR:
		closeDoor();
		ledControl(0x22, true);
		delay(200);
		if (actLimSwitch > LIMIT_SWITCH_2) {
			goDown();
		} else {
			goUp();
		}
		goToFloor(2);
		break;
	case BUTTON_3_ELEVATOR:
		closeDoor();
		ledControl(0x23, true);
		delay(200);
		if (actLimSwitch > LIMIT_SWITCH_3) {
			goDown();
		} else {
			goUp();
		}
		goToFloor(3);
		break;
	case BUTTON_4_ELEVATOR:
		closeDoor();
		ledControl(0x24, true);
		delay(200);
		if (actLimSwitch > LIMIT_SWITCH_4) {
			goDown();
		} else {
			goUp();
		}
		goToFloor(4);
		break;
	}

}

void goToFloor(uint8_t floor) {
	displayData();
	switch (floor) {
	case 0:
		floor_0 = true;
		delay(5);
		break;
	case 1:
		floor_1 = true;
		delay(5);
		break;
	case 2:
		floor_2 = true;
		delay(5);
		break;
	case 3:
		floor_3 = true;
		delay(5);
		break;
	case 4:
		floor_4 = true;
		delay(5);
		break;
	}
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

void readyForNewData(void) {
	dataReady = false;
	dataIndex = 0;
	recvDataSize = 0;
}

void goToDefault() {
	closeDoor();
	delay(5);
	floor_0 = true;
	goDown();
}

void processAck(uint8_t device) {
	switch (device) {
	case DOOR:
		if (doorClosed) {
			doorClosed = false;
		} else {
			doorClosed = true;
		}
		break;
	case MOTOR:
		if (isMoving) {
			isMoving = false;
		} else {
			isMoving = true;
		}
		break;
	}
}

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
/*!
 * @brief Main function
 */
int main(void) {
	lpsci_config_t config;

	BOARD_InitPins();
	BOARD_BootClockRUN();
	CLOCK_SetLpsci0Clock(0x1U);

	LPSCI_GetDefaultConfig(&config);
	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config.enableTx = true;
	config.enableRx = true;

	LPSCI_Init(DEMO_LPSCI, &config, DEMO_LPSCI_CLK_FREQ);

	LPSCI_EnableInterrupts(DEMO_LPSCI, kLPSCI_RxDataRegFullInterruptEnable);
	EnableIRQ(DEMO_LPSCI_IRQn);
	lastDirDown = true;
	readyForNewData();
	isMoving = false;
	ready = false;
	goToDefault();
	while (1) {

		if (dataReady) {
			if (data[0] == 0xa0) {
				dataProcessing(data[2]);
			} else if (data[0] == 0xa1) {
				processAck(data[2]);
			}
			delay(5);
			readyForNewData();
		}
		/*if ((floor_0 || floor_1 || floor_2 || floor_3 || floor_4) && (isMoving == false)) {
			if (lastDirDown)
				goDown();
			else
				goUp();
		}
		*/
	}
}
