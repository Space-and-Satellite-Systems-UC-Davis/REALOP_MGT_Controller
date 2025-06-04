/*
 * print_scan.c
 *
 * 	- August 17, 2024
 * 		Author : Darsh
 * 		Log    : Copied from IntelliSat, modified for the Radio Controller
 */


#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "print_scan.h"
#include <UART/uart.h>

#define ConsoleUART 	USART1
#define UART_BAUDRATE	9600

void printer_init() {
	usart_init(ConsoleUART, UART_BAUDRATE);
}

int printMsg(const char *message, ...) {
	char buff[128];

	va_list args;
	va_start(args, message);
	vsprintf(buff,message,args);

	usart_transmitStr(ConsoleUART, buff);
}
