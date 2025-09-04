#include <stdint.h>
#include <print_scan.h>
#include "platform_init.h"
#include "Timers/timers.h"
#include "UART/mgt_handler.h"



#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
  init_platform();
  usart_init(USART1, 9600);

  //Length of chunks being sent in bytes between PFC and MGT
  #define CHUNK_LENGTH 256
  
  //Time between upload requests in seconds
  #define WAIT_INTERVAL 5

	uint8_t chunk[CHUNK_LENGTH];
    while(1) {
    	int read_status = crc_read(USART1, chunk);
    	if (read_status >= 0) {
    		handle_packet(USART1, chunk);
    	}
    }
}
