#ifndef PCP_HANDLER
#define PCP_HANDLER

#include "UART/pcp.h"
#include "PWM/pwm_timer.h"
// #include "ADC/adc.h"

#define RESISTANCE_VALUE_OHMS 42; // TODO

void handle_pcp_packet(PCPDevice *pcp, char chunk[]);

#endif // PCP_HANDLER