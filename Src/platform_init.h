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
#include <print_scan.h>
#include <PWM/pwm_timer.h>
#include <Intercomm/mgt_handler.h>
#include <Sleep/sleep.h>
#include <RTC/rtc.h>


/**
 * Configures the system's various features,
 * such as clocks, protocol hardware, and more.
 *
 * @returns None
 */
void init_platform() {
	SCB->CPACR|= 0x00F00000; // Enable the Floating-Point Unit for full access
	init_coreClocks();
	pwm_init();

	led_init();
	printer_init();
	systick_init();
	init_mgtHandler();
	usart_init(USART1, 9600);
  coils_setup();
  adc_init();

}

#endif // REALOP1_PLATFORM_INIT_H
