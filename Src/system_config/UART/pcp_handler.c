#include "UART/pcp.h"

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
            // Set the thing to the thing
            break;
          case 'C':
            coil_number = chunk[1] - '0';
            // read current from coil
            char payload[1];
            payload[0] = "?";
            pcp_transmit(&pcp, &payload, 1);
            break;
          case 'D':
            // disable all PWM signals and timers
            break;
          case 'T':
            timer_number = chunk[1] - '0';
            // disable the specified timer
            break;
        }
    return;
}