#include "watchdog.h"
#include "UART/uart.h"
#include <LED/led.h>
#include <print_scan.h>

int zero() {
    return (getSysTime() % 2 + getSysTime()) % 2;
}

void testFunction_watchdog() {
    // start watchdog
    int count = 0;
    while(1){
    	delay_ms(1000);
        count ++;
        if(count  == 3){
            watchdog_changeIWDGTimeout(5000);
            delay_ms(10);
        }
        if(count < 15){
            continue;
        }
        // uh-oh!
        int x = zero();
        printMsg("voltage: %f\n", x);
    }
}
