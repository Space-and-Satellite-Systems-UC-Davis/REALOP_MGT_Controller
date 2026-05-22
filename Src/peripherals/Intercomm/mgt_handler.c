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

void init_mgtHandler(){
  EXTI->IMR1 |= 1;
	NVIC_SetPriority(EXTI0_IRQn, 15);
	NVIC_EnableIRQ(EXTI0_IRQn);
}

void turn_off(int timer_number){
  TIM_TypeDef *timer = get_timer_from_number(timer_number);
  pwm_timerOff(timer);

}

void turn_on(int timer_number){
  TIM_TypeDef *timer = get_timer_from_number(timer_number);
  pwm_timerOn(timer);
}

void handle_packet(USART_TypeDef *bus, char chunk[]) {
    int coil_number;
    int pwm;
    int percentage;
    int timer_number;
    int direction; 
    switch (chunk[0]) {
          case 'S':
            coil_number = chunk[1] - '0';
            percentage = chunk[3];
            coils_setDuty(coil_number, percentage);
            if (percentage != 100) {
              coils_enablePWM(coil_number);
            }
            break;
          case 'W':
            coil_number = chunk[1] - '0';
            direction = chunk[3];
            if (direction == 'H') {
              coils_setDir(coil_number, HIGH);
            } else {
              coils_setDir(coil_number, LOW);
            }
            break;
          case 'C':
            crc_wait(USART1);
            float payload = 42.0;
            // payload[0] = adc_readVoltage(adc_readCurrent(??, ??)) / RESISTANCE_VALUE_OHMS;
            // TODO: use adc peripheral calls to read the current
            crc_transmit(bus, (uint8_t*)&payload, sizeof(payload));
            break;
          case 'D':
            turn_off(0);
            turn_off(1);
            turn_off(2);
            break;
          case 'F': case 'T':
            timer_number = chunk[1] - '0';
            chunk[0] == 'F' ? turn_off(timer_number) : turn_on(timer_number);
            break;
          case 'K':
            // TODO: Here Be Dragons
            break;
        }    
    return;
}


void EXTI0_IRQHandler(){
	EXTI->PR1 |= 1;
	uint8_t chunk[8];
	int read_status = crc_read(USART1, chunk);
	if (read_status > 0) 
		handle_packet(USART1, chunk);
}