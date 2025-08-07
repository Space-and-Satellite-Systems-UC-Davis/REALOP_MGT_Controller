#include <stdint.h>
#include <print_scan.h>
#include "platform_init.h"
#include "Timers/timers.h"
#include "UART/pcp_handler.h"



#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
  init_platform();
  usart_init(USART1, 9600);


  //Length of chunks being sent in bytes between PFC, Radio, and Ground
  #define CHUNK_LENGTH 16
  //Time between upload requests in seconds
  #define WAIT_INTERVAL 5

	PCPDevice pcp;
	if (make_pcpdev(&pcp, USART1)) {
    usart_transmitBytes(USART1, "BAD_PCPDEV", 11);
  }

	uint8_t chunk[CHUNK_LENGTH];
  pcp_transmit(&pcp, "hello?", 7);
	uint64_t start_time = getSysTime();
    while(1) {
      delay_ms(10);
    	int read_status = pcp_read(&pcp, chunk);
    	if (read_status < 0) {
    		handle_pcp_packet(&pcp, chunk);
    	}
      pcp_retransmit(&pcp);
    }
}
