#include <stdint.h>
#include <print_scan.h>
#include "platform_init.h"
#include "Timers/timers.h"
#include "peripherals/Intercomm/mgt_handler.h"

extern void testFunction_watchdog();

#define PFC_USART USART1
//Length of chunks being sent in bytes between PFC and MGT
#define CHUNK_LENGTH 8

//Time between upload requests in seconds
#define WAIT_INTERVAL 5

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
  init_platform();
  usart_init(USART1, 9600);
  coils_setup();
  adc_init();
  usart_init(PFC_USART, 9600);

  //Length of chunks being sent in bytes between PFC and MGT
  #define CHUNK_LENGTH 8
  
  //Time between upload requests in seconds
  #define WAIT_INTERVAL 5

  testFunction_watchdog();
	uint8_t chunk[CHUNK_LENGTH];
  while(1) {
	  usart_transmitBytes(PFC_USART, "HELLO!!!", 8);
    // memset(chunk, '?', CHUNK_LENGTH);
   	// int read_status = crc_read(PFC_USART, chunk);
   	// if (read_status > 0) {
   	// 	handle_packet(PFC_USART, chunk);
   	// }
  }
}
