/*
 * dataHandling.c
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

#include <dataHandling.h>

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

void sendAck(uint8_t device) {
	uint8_t crcElements[] = { device, 0x00 };
	uint8_t msg[] = { 0xa1, device, 0x00, 0x00, dallas_crc8(crcElements,
			sizeof(crcElements)) };
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
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

/*
 https://www.geeksforgeeks.org/time-delay-c/
 */
void delay(int millis) {
	for (long i = 0; i < millis * 10000; i++)
		__asm("nop");
}

void readyForNewData(void) {
	dataReady = false;
	dataIndex = 0;
	recvDataSize = 0;
}
