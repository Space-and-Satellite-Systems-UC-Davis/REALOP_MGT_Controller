#include "watchdog.h"
#include "UART/uart.h"
#include <LED/led.h>

void testFunction_watchdog() {
    // start watchdog
    watchdog_init(420);
    int count = 0;
    while(1){
        delay_ms(1000);
        count ++;
        if(count  == 3){
            watchdog_changeIWDGTimeout(5000);
        }
        if(count < 15){
            continue;
        }
        // uh oh!
    }
}
