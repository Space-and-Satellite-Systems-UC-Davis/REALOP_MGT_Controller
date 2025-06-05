#include <stdint.h>
#include <print_scan.h>
#include "platform_init.h"
//#include "print_scan.h"
#include "UART/pcp.h"
#include "UART/uart.h"
#include "Timers/timers.h"
#include "UART/pcp_handler.c"



#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
  init_platform();
  usart_init(USART1, 9600);

  //Length of chunks being sent in bytes between PFC, Radio, and Ground
  #define CHUNK_LENGTH 8
  //Time between upload requests in seconds
  #define WAIT_INTERVAL 5

	PCPDevice pcp;
	make_pcpdev(&pcp, USART1);

	uint8_t chunk[CHUNK_LENGTH];

	uint64_t start_time = getSysTime();
    while(1) {
    	nop(1);
    	int read_status = pcp_read(&pcp, chunk);
    	if (read_status != -1) {
    		handle_pcp_packet(&pcp, chunk);
    	}
    }
}
