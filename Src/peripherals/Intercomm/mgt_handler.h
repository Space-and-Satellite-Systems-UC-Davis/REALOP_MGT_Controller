#ifndef _MGT_HANDLER_
#define _MGT_HANDLER_

#include "UART/crc.h"
#include "PWM/pwm_timer.h"
#include "Coils/Coils.h"

#define RESISTANCE_VALUE_OHMS 42; // TODO

void handle_packet(USART_TypeDef *bus, char chunk[]);

#endif // _MGT_HANDLER_