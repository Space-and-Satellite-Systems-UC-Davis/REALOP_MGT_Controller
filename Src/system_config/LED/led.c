/*
 * led.c
 *
 *	- August 17, 2024
 *		Author	: Darsh
 *		Log		: Copied from IntelliSat
 */

#include "led.h"

/***************************** LED INITIALIZERS ******************************/

void led_init() {

}

/******************************* LED TOGGLERS ********************************/

void blinky() {
	static int counter = 0;

	if (counter++ > 1000) {
		counter = 0;
	}
	if (counter > 900) {
		// on
	} else {
		// off
	}
}
