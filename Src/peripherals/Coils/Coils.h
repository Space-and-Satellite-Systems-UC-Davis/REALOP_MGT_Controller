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

void coils_setup();

void coils_setDir(COILS coil, DIRECTION dir);

void coils_setDuty(COILS coil, int duty);

void coils_enablePWM(COILS coil);

void coils_disablePWM(COILS coil);

void coils_on(COILS coil);

void coils_off(COILS coil);

//TODO: Complete once ADC is written
uint16_t coils_readCurrent(COILS coils);
