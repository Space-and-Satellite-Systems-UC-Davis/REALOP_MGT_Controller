#include "Coils.h"

coils_t Coil_0;
coils_t Coil_1;
coils_t Coil_2;

void coils_gpioSetup() {
    gpio_mode(Coil_0.dir_port, Coil_0.dir_pin, GPIO_MODER_Output, -1, -1, GPIO_PUPDR_PULL_DOWN);
    gpio_mode(Coil_1.dir_port, Coil_1.dir_pin, GPIO_MODER_Output, -1, -1, GPIO_PUPDR_PULL_DOWN);
    gpio_mode(Coil_2.dir_port, Coil_2.dir_pin, GPIO_MODER_Output, -1, -1, GPIO_PUPDR_PULL_DOWN);
}


void coils_setup() {
    Coil_0.dir_pin = 1;
    Coil_0.dir_port = GPIOA;
    Coil_0.en_pwm = DRV0_PWM1;

    
    Coil_1.dir_pin = 11;
    Coil_1.dir_port = GPIOB;
    Coil_1.en_pwm = DRV1_PWM1;
    
    
    Coil_2.dir_pin = 9;
    Coil_2.dir_port = GPIOA;
    Coil_2.en_pwm = DRV2_PWM1;
    
    pwm_init();
    coils_gpioSetup();
}

void coils_setDir(COILS coil, DIRECTION dir) {
    switch(coil) {
        case COIL0:
            gpio_set(Coil_0.dir_port, Coil_0.dir_pin, dir);
            break;
        case COIL1:
            gpio_set(Coil_1.dir_port, Coil_1.dir_pin, dir);
            break;
        case COIL2:
            gpio_set(Coil_2.dir_port, Coil_2.dir_pin, dir);
            break;
    }
}

void coils_setDuty(COILS coil, int duty) {
    switch(coil) {
        case COIL0:
            pwm_setDutyCycle(Coil_0.en_pwm, duty);
            break;
        case COIL1:
            pwm_setDutyCycle(Coil_1.en_pwm, duty);
            break;
        case COIL2:
            pwm_setDutyCycle(Coil_2.en_pwm, duty);
            break;
    }
}

void coils_enablePWM(COILS coil) {
    switch(coil) {
        case COIL0:
            pwm_enableChannel(Coil_0.en_pwm);
            break;
        case COIL1:
            pwm_enableChannel(Coil_1.en_pwm);
            break;
        case COIL2:
            pwm_enableChannel(Coil_2.en_pwm);
            break;
    }
}

void coils_disablePWM(COILS coil) {
    switch(coil) {
        case COIL0:
            pwm_disableChannel(Coil_0.en_pwm);
            break;
        case COIL1:
            pwm_disableChannel(Coil_1.en_pwm);
            break;
        case COIL2:
            pwm_disableChannel(Coil_2.en_pwm);
            break;
    }
}

void coils_on(COILS coil) {
    switch(coil) {
        case COIL0:
            gpio_high(Coil_0.dir_port, Coil_0.dir_pin);
            break;
        case COIL1:
            gpio_high(Coil_1.dir_port, Coil_1.dir_pin);
            break;
        case COIL2:
            gpio_high(Coil_2.dir_port, Coil_2.dir_pin);
            break;
    }
}

void coils_off(COILS coil) {
    switch(coil) {
        case COIL0:
            gpio_low(Coil_0.dir_port, Coil_0.dir_pin);
            break;
        case COIL1:
            gpio_low(Coil_1.dir_port, Coil_1.dir_pin);
            break;
        case COIL2:
            gpio_low(Coil_2.dir_port, Coil_2.dir_pin);
            break;
    }
}