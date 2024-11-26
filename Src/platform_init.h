/*
 * platform_init.h

 * 	- August 17, 2024
 * 		Author			: Darsh
 * 		Log				: Copied from IntelliSat, modified for Radio Controller
 */

#ifndef REALOP1_PLATFORM_INIT_H
#define REALOP1_PLATFORM_INIT_H

#include <globals.h>

#include <core_config.h>
#include <Timers/timers.h>
#include <SPI/spi.h>
#include <LED/led.h>
#include <UART/uart.h>
#include <print_scan.h>


/**
 * Configures the system's various features,
 * such as clocks, protocol hardware, and more.
 *
 * @returns None
 */
void init_platform() {
	SCB->CPACR|= 0x00F00000; // Enable the Floating-Point Unit for full access
	init_coreClocks();

	// init intercomm
	// init radio

	led_init();
	// printer_init();
	systick_init();
}

#endif // REALOP1_PLATFORM_INIT_H
