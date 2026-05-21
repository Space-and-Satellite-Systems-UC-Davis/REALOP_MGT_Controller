#include "watchdog.h"
#include "UART/uart.h"
#include <LED/led.h>
#include <print_scan.h>

int zero() {
    return (getSysTime() % 2 + getSysTime()) % 2;
}

void testFunction_watchdog() {
    printer_init();
    printMsg("start watchdog\r\n");
    int count = 0;
    while(1){
    	delay_ms(1000);
        count ++;
        printMsg("count: %d\r\n", count);
        if(count  == 3){
            watchdog_changeIWDGTimeout(5000);
            delay_ms(10);
        }
        if(count < 15){
            continue;
        }
        printMsg("uh-oh!\r\n");
        int x = zero();
        printMsg("voltage: %s\r\n", x);
    }
}
