/*
 * led.h
 *
 *	- August 17, 2024
 *		Author	: Darsh
 *		Log		: Copied from IntelliSat
 */

#ifndef REALOP1_LED_H_
#define REALOP1_LED_H_

#include <globals.h>
#include <GPIO/gpio.h>

/***************************** LED INITIALIZERS ******************************/

/*
 * Configures the GPIO ports and pins for all the LEDs
 *
 * 	@param   None
 * 	@returns None
 */
void led_init();

/******************************* LED TOGGLERS ********************************/

/**
 * Blinks the heartbeat LED every so often.
 * Meant to be called every 1ms
 */
void blinky();


#endif /* REALOP1_LED_H_ */
