#include "sleep.h"
#include "platform_init.h"


bool is_DBP_not_set() { return (PWR->CR1 & PWR_CR1_DBP) == 0; }
void testFunction_Sleep(int sleepMode) {
    // init stuff
    init_platform();
    usart_init(USART3, 9600);

  // enable LSE clock
    backup_domain_controlEnable();
  	wait_with_timeout(is_DBP_not_set, DEFAULT_TIMEOUT_MS);
  	RCC->BDCR |= RCC_BDCR_LSEON;
  	backup_domain_controlDisable();

    rtc_config(LSE, true);
    
    // enable gpio for testing
    // gpio_mode(GPIOB, 1, GPIO_Input, 0, 0x3, GPIO_PullupPulldown);
    gpio_mode(GPIOB, 0, GPIO_Output, 0, 0x3, GPIO_PullupPulldown); 
    for(int i = 0; i < 3; i++) {
        gpio_low(GPIOB, 0);
        delay_ms(500);
        gpio_high(GPIOB, 0);
        delay_ms(500);
    }
    EXTI->PR1 = 0;
    EXTI->PR2 = 0;
    switch(sleepMode) {
        case SLEEP:
            sleep_init();
            break;
        case LPSLEEP:
            lowPowerSleep_init();
            break;
        case STOP0:
            stop0_init();
            break;
        default:
            break;
    }
    for(int i = 0; i < 20; i++) {
        gpio_low(GPIOB, 0);
        delay_ms(250);
        gpio_high(GPIOB, 0);
        delay_ms(250);
    }
}