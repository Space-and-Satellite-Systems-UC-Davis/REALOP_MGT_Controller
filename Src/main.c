#include <stdint.h>
#include <print_scan.h>
#include "platform_init.h"
#include "Timers/timers.h"
#include "peripherals/Intercomm/mgt_handler.h"

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

  //Length of chunks being sent in bytes between PFC and MGT
  #define CHUNK_LENGTH 8
  
  //Time between upload requests in seconds
  #define WAIT_INTERVAL 5

  while(1);
}
