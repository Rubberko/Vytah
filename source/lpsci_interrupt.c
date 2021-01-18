/**
 * @file    lpsci_interrupt.c
 * @brief   Hlavny spustaci subor
 * @details Subor obsahuje funkciu hlavnu funkciu main a spravcu preruseni
 */

#include "board.h"

#include <vars.h>
#include <movement.h>
#include <ledInfo.h>
#include <crc.h>
#include <dataHandling.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/



/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_LPSCI_IRQHandler(void) {
	if ((kLPSCI_RxDataRegFullFlag) & LPSCI_GetStatusFlags(DEMO_LPSCI)) {
		data[dataIndex] = LPSCI_ReadByte(DEMO_LPSCI);

		if (dataIndex == 3) {
			recvDataSize = data[dataIndex];
		}
		if (dataIndex == recvDataSize + 4) {
			dataReady = true;
		}
		dataIndex++;

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
		if (!isMoving) {
			unservedFloors();
		}
	}
}
