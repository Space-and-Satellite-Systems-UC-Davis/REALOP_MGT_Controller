/**     GPIO pinout
 ** 	TIM CH1		GPIO A15	AF - 1
 **     mgt-drv0-pwm1 	  PA0  	TIM2_CH1 AF1
 **		mgt-drv0-pwm0	  PA1	TIM2_CH2 AF1
 **		mgt-drv1-pwm1	  PB10	TIM2_CH3 AF1
 **		mgt-drv1-pwm0	  PB11	TIM2_CH4 AF1
 **		mgt-drv2-pwm0	  PA9	TIM1_CH2 AF1
 **		mgt-drv2-pwm1	  PA8	TIM1_CH1 AF1
 */
#include "magnetorquer.h"


/// @brief Set the selected coil of the magnetorquer's pwm duty cycle.
/// @param coil_num the coil number to configure
/// @param pwm_num the pwm channel to configure
/// @param pwm_percentage percentage for pwm duty cycle
/// @return true if pwm is set correctly, false otherwise
bool mgt_setPercentCoilAck(uint8_t coil_num, uint8_t pwm_num, uint8_t pwm_percentage)
{
    if ((pwm_num != 0 && pwm_num != 1) || coil_num > 2) return false;
    switch (coil_num)
    {
    case 0:
        if(pwm_num == 0) {
            pwm_setDutyCycle(DRV0_PWM0, pwm_percentage);
        } else {
            pwm_setDutyCycle(DRV0_PWM1, pwm_percentage);
        }
        break;
    case 1:
        if(pwm_num == 0) {
            pwm_setDutyCycle(DRV1_PWM0, pwm_percentage);
        } else {
            pwm_setDutyCycle(DRV1_PWM1, pwm_percentage);
        }
        break;
    case 2:
        if(pwm_num == 0) {
            pwm_setDutyCycle(DRV2_PWM0, pwm_percentage);
        } else {
            pwm_setDutyCycle(DRV2_PWM1, pwm_percentage);
        }
        break;
    default:
        return false;
    }
    usart_transmitChar(MGT_UART, 'A');
    return true;
}
/**
 * mgttlm     PC0  
 */

bool mgt_getCoilCurrentAck(uint8_t coil_num)
{
    
    return true;
}

/// @brief This function handles turns off all the PWM timers on the MGT MCU side, thereby interrupting the PSM signals. \n
/// A char 'A' is sended to the primary flight computer (PFC) as acknowledgement.
/// @return true if shutdown is successful
bool mgt_ShutdownAck()
{
    pwm_timerOff(PWMTimerDRV0);
    pwm_timerOff(PWMTimerDRV1);
    pwm_timerOff(PWMTimerDRV2);
    usart_transmitChar(MGT_UART, 'A');
    return true;
}

/// @brief This function handles turns off the specified PWM timers on the MGT MCU side, thereby interrupting the PSM signals. \n
/// A char 'A' is sended to the primary flight computer (PFC) as acknowledgement.
/// @param timer the specific PWM Timer to disable
/// @return true if timer is successfully deactivated
bool mgt_timerOffAck(const TIM_TypeDef* timer)
{
    pwm_timerOff(timer);
    usart_transmitChar(MGT_UART, 'A');
    return true;
}
