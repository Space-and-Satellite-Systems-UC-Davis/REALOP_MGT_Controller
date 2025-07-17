#include "UART/pcp_handler.h"

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

void handle_pcp_packet(PCPDevice *pcp, char chunk[]) {
    int coil_number;
    int pwm;
    int percentage;
    int timer_number;
    switch (chunk[0]) {
          case 'S':
            coil_number = chunk[1] - '0';
            pwm = chunk[3] - '0';
            percentage = 10 * (chunk[5] - '0') + (chunk[6] - '0');
            pwm_setDutyCycle(coil_number * 2 + pwm, percentage);
            break;
          case 'C':
            coil_number = chunk[1] - '0';
            float payload[1];
            // payload[0] = adc_readVoltage(adc_readCurrent(??, ??)) / RESISTANCE_VALUE_OHMS;
            // TODO: use adc peripheral calls to read the current
            pcp_transmit(&pcp, &payload, sizeof(float));
            break;
          case 'D':
            pwm_disableChannel(DRV0_PWM0);
            pwm_disableChannel(DRV0_PWM1);
            pwm_disableChannel(DRV1_PWM0);
            pwm_disableChannel(DRV1_PWM1);
            pwm_disableChannel(DRV2_PWM0);
            pwm_disableChannel(DRV2_PWM1);
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
    // pcp_transmit(&pcp, "", 1);
    return;
}