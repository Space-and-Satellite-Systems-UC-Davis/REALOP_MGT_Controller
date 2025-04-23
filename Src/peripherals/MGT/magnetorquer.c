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
bool mgt_mcu_setPWMCoil(uint8_t coil_num, uint8_t pwm_num, uint8_t pwm_percentage)
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
    usart_transmitChar(MGT_USART, 'A');
    return true;
}
/**
 * mgttlm     PC0  
 */

int8_t mgt_getCoilCurrent(uint8_t coil_num) {
    if(coil_num == 0) {
        return 0;
    } else if (coil_num == 1) {
        return 1;
    } else {
        return 2;
    }
}

bool mgt_mcu_getCoilCurrent(uint8_t coil_num)
{
    // TODO replace and impliment dummy funciton
    int8_t coil_current = mgt_getCoilCurrent(coil_num);

    // Send over uart
    usart_transmitChar(MGT_USART, (char) coil_current);
    // wait for ack
    return true;
}

/// @brief This function handles turns off all the PWM timers on the MGT MCU side, thereby interrupting the PSM signals. \n
/// A char 'A' is sended to the primary flight computer (PFC) as acknowledgement.
/// @return true if shutdown is successful
bool mgt_mcu_shutdown()
{
    pwm_timerOff(PWMTimerDRV0);
    pwm_timerOff(PWMTimerDRV1);
    pwm_timerOff(PWMTimerDRV2);
    usart_transmitChar(MGT_USART, 'A');
    return true;
}
/// * YOU WILL BE PASSING IN THE TIMER NUMBER AS A INT, NOT THE ADDRESS.
/// @brief This function handles turns off the specified PWM timers on the MGT MCU side, thereby interrupting the PSM signals. \n
/// A char 'A' is sended to the primary flight computer (PFC) as acknowledgement.
/// @param timer the specific PWM Timer to disable
/// @return true if timer is successfully deactivated
bool mgt_mcu_timerOff(uint8_t timer_num)
{
    TIM_TypeDef* timer = NULL;
    switch (timer_num)
    {
    case 0:
        timer = PWMTimerDRV0;
        break;
    case 1:
        timer = PWMTimerDRV1;
        break;
    case 2:
        timer = PWMTimerDRV2;
        break;
    default:
        return false;
        break;
    }
    pwm_timerOff(timer);
    usart_transmitChar(MGT_USART, 'A');
    return true;
}
