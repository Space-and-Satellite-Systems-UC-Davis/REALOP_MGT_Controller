#include <stdint.h>
#include <print_scan.h>
#include "platform_init.h"
#include "MGT/magnetorquer.h"
#include "UART/uart.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
  init_platform();

  while (1)
  {
    char command = '0';
    uint8_t coil_num = -1;
    uint8_t timer_num = -1;

    uint8_t size = (uint8_t) usart_receiveBytes(MGT_UART, command, 1);
    if (size != sizeof(command)) continue;

    switch (command)
    {
    case MGT_GET_CURRENT_CMD:
      size = (uint8_t) usart_receiveBytes(MGT_UART, coil_num, 1);
      if (size != sizeof(coil_num)) continue;
      mgt_mcu_getCoilCurrent(coil_num);
      continue;
    case MGT_SET_PWM_PERCENT_CMD:
      // TODO Get the inputs and parse with correct formats
      mgt_mcu_setPWMCoil();
      continue;
    case MGT_SHUTDOWN_CMD:
      mgt_mcu_shutdown();
      continue;
    case MGT_TIMER_OFF_CMD:
      size = (uint8_t) usart_receiveBytes(MGT_UART, timer_num, 1);
      if (size != sizeof(timer_num)) continue;
      mgt_mcu_timerOff(timer_num);
    default:
      continue;
    }
  }
}
