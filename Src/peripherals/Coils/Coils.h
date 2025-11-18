#pragma once

#include "stm32l431xx.h"
#include "PWM/pwm_timer.h"
#include "GPIO/gpio.h"

typedef struct COIL_STRUCT {
    GPIO_TypeDef* dir_port;
    int dir_pin;
    PWM_Channels en_pwm;
} coils_t;

typedef enum COILS_PICKER {
    COIL0,
    COIL1,
    COIL2
} COILS;

typedef enum DIRECTION_ENUM {LOW, HIGH} DIRECTION;

/**
 * Sets up the coils and their constants
 */
void coils_setup();

/**
 * Sets the direction of the current and is a LOW or HIGH
 * 
 * @param coil The coil number
 * @param dir direction of the coil
 */
void coils_setDir(COILS coil, DIRECTION dir);

/**
 * Sets the duty of the current 0-100
 * 
 * @param coil The coil number
 * @param duty duty of the coil
 */
void coils_setDuty(COILS coil, int duty);

/**
 * Enabled the PWM for the coil
 * 
 * @param coil The coil number
 */
void coils_enablePWM(COILS coil);

/**
 * Disables the PWM for the coil
 * 
 * @param coil The coil number
 */
void coils_disablePWM(COILS coil);

/**
 * Turn the coil on with gpio_high
 * 
 * @param coil The coil number
 */
void coils_on(COILS coil);

/**
 * Turn the coil off with gpio_low
 * 
 * @param coil The coil number
 */
void coils_off(COILS coil);

//TODO: Complete once ADC is written
uint16_t coils_readCurrent(COILS coils);
