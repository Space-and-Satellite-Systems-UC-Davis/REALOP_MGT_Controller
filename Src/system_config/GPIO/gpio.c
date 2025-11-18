/*
 * gpio.c
 *
 * 	- August 17, 2024
 * 		Author : Darsh
 * 		Log    : Copied from IntelliSat
 */

#include "gpio.h"

void gpio_high(GPIO_TypeDef * port, int pin) {
	port->BSRR = 1 << pin;
}

void gpio_low(GPIO_TypeDef * port, int pin) {
	port->BSRR = 1 << (pin+16);
}

void gpio_set(GPIO_TypeDef * port, int pin, int value) {
	if (value) {
		gpio_high(port, pin);
	} else {
		gpio_low(port, pin);
	}
}

int gpio_read(GPIO_TypeDef * port, int pin) {
	if (port->IDR & (1 << pin)) {
		return 1;
	} else {
		return 0;
	}

	// Prevent from reading junk data if pin is not set to an actual pin
	return -1;
}

void gpio_mode(GPIO_TypeDef * port, int pin, enum gpio_modes mode, int open_drain, int speed, int pull) {
	port->MODER = (port->MODER & ~(0x3 << (pin*2))) | ((mode & 0x3) << (pin*2));
	
	if (open_drain != -1)
		port->OTYPER = (port->OTYPER & ~(0x1 << pin)) | ((open_drain & 0x1) << pin);

	if (speed != -1)	
		port->OSPEEDR = (port->OSPEEDR & ~(0x3 << (pin*2))) | ((speed & 0x3) << (pin*2));

	if (pull != -1)		
		port->PUPDR = (port->PUPDR & ~(0x3 << (pin*2))) | ((pull & 0x3) << (pin*2));
}

void gpio_af(GPIO_TypeDef * port, uint8_t pin, uint8_t afn) {
	port->MODER = (port->MODER & ~(0x3 << (pin*2))) | ((0x2) << (pin*2));
	if (pin > 7) {
		port->AFR[1] = (port->AFR[1] & ~(0xF << ((pin-8)*4))) | ((afn & 0xF) << ((pin-8)*4));
	} else {
		port->AFR[0] = (port->AFR[0] & ~(0xF << (pin*4))) | ((afn & 0xF) << (pin*4));
	}
	gpio_low(port, pin);
}
