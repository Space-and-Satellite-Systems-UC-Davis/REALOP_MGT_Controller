/*
 * core_config.h
 *
 *  - August 17, 2024
 *  	Author	: Darsh
 *  	Log		: Copied from IntelliSat, modified for Radio Controller
 */

#ifndef REALOP1_CORE_CONFIG_H_
#define REALOP1_CORE_CONFIG_H_

#include "stm32l431xx.h"
#include <globals.h>

/**
 * Initializes the clocks of the micro-controller.
 *
 * This function sets up the various clock sources and
 * frequencies for the micro-controller.
 *
 * @param   None
 * @returns None
 */
void init_coreClocks();

#endif /* REALOP1_CORE_CONFIG_H_ */
