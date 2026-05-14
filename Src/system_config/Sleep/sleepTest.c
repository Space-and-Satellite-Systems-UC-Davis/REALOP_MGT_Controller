#include "sleep.h"
#include "UART/uart.h"

/*
    Sleep test function

    loop: 
    sleeps, then, when woken up, recieves 1 character via uart, increments it,
    and sends it back to main mcu. 

    succeeds if the main MCU test mgtsleep_test passes while this is running
*/

void testFunction_Sleep() {
    // init stuff

    usart_init(USART1, 9600);
    uint8_t buf[10];
    usart_recieveBytes(USART1, buf, 2);
    while(true) {
        EXTI->PR1 = 0;
        EXTI->PR2 = 0;

        sleep_init();

        uint8_t num_in[4] = {0xFF, 0xFF, 0xFF, '\0'};
        while(!usart_recieveBufferNotEmpty(USART1));
        usart_recieveBytes(USART1, num_in, 1);

        num_in[0] += 1;
        uint8_t msg[2] = {num_in[0], '\0'};
        usart_transmitBytes(USART1, msg); // send to main MCU
    }
    
}