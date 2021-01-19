/*
 * definitions.h
 *
 *  Created on: Jan 16, 2021
 *      Author: jsmol
 */

/**
 * @file    definitions.h
 * @brief   Definicie adries prvkov vytahu
 * @details Definicie adries prvkoc vytahu, definicie vyuzivane na komunikaciu,
 * 			ine vyuzivane hlavickove subory.
 */
#include <fsl_lpsci.h>
#include <pin_mux.h>
#include <stdbool.h>
#include <time.h>
#include "clock_config.h"


#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

/***************************************************************************//**
 * Definicie pre komunikaciu
 ******************************************************************************/
#define DEMO_LPSCI UART0
#define DEMO_LPSCI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_LPSCI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_LPSCI_IRQn UART0_IRQn
#define DEMO_LPSCI_IRQHandler UART0_IRQHandler

/***************************************************************************//**
 * Adresy prvkov
 ******************************************************************************/
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


#endif /* DEFINITIONS_H_ */
