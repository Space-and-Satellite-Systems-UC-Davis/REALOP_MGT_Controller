#include "mgt_handler.h"

TIM_TypeDef *get_timer_from_number(int n) {
    switch (n) {
        case 0:
            return PWMTimerDRV0;
        case 1:
            return PWMTimerDRV1;
        case 2:
            return PWMTimerDRV2;
        default:
            return NULL; // fail hard to aid debugging. TODO: make this safer
    }
}

void handle_packet(USART_TypeDef *bus, char chunk[]) {
    int coil_number;
    int pwm;
    int percentage;
    int timer_number;
    switch (chunk[0]) {
          case 'S':
            coil_number = chunk[1] - '0';
            percentage = 10 * (chunk[3] - '0') + (chunk[4] - '0');
            coils_setDuty(coil_number, percentage);

            break;
          case 'C':
            coil_number = chunk[1] - '0';
            float payload[1];
            payload[0] = 42.0;
            // payload[0] = adc_readVoltage(adc_readCurrent(??, ??)) / RESISTANCE_VALUE_OHMS;
            // TODO: use adc peripheral calls to read the current
            crc_transmit(bus, payload, sizeof(float));
            break;
          case 'D':

            coils_disablePWM(COIL0);
            coils_disablePWM(COIL1);
            coils_disablePWM(COIL2);

            coils_off(COIL0);
            coils_off(COIL1);
            coils_off(COIL2);

            pwm_timerOff(PWMTimerDRV0);
            pwm_timerOff(PWMTimerDRV1);
            pwm_timerOff(PWMTimerDRV2);

            break;
          case 'T':
            timer_number = chunk[1] - '0';
            TIM_TypeDef *timer = get_timer_from_number(timer_number);
            pwm_timerOff(timer);
            break;
        }    
    return;
}
