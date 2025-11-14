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
    usart_transmitChar(MGT_UART, 'A');
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

    // Convert coil_current to whatever format you use for uart
    int8_t buffer[] = {coil_current};
    // Send over uart
    usart_transmitBytes(MGT_UART, buffer);
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
    usart_transmitChar(MGT_UART, 'A');
    return true;
}
/// * YOU WILL BE PASSING IN THE TIMER NUMBER AS A INT, NOT THE ADDRESS.
/// @brief This function handles turns off the specified PWM timers on the MGT MCU side, thereby interrupting the PSM signals. \n
/// A char 'A' is sended to the primary flight computer (PFC) as acknowledgement.
/// @param timer the specific PWM Timer to disable
/// @return true if timer is successfully deactivated
bool mgt_mcu_timerOff(uint8_t timer_num)
{
    TIM_TypeDef* timer;
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
        break;
    }
    pwm_timerOff(timer);
    usart_transmitChar(MGT_UART, 'A');
    return true;
}

/// @brief get a packet that encapsulates the data for the magnetorquer function calls. After wards, write to the 
/// @note A packet consist of the following format:
/// `DELIMITER COMMAND ARG0 ARG1 ARG3 DELIMITER`
/// In total 6 bytes.
/// @return True if all parts of the packet is received intact and false if any part of the data is corrupted
bool mgt_receive_packet(uint8_t arg0[], uint8_t arg1[], uint8_t arg2[])
{
    uint8_t buffer[1];
    usart_receiveBytes(MGT_UART, &buffer, 1);
    if (buffer[0] != MGT_PACK_DELIMITER) return 0;

    usart_receiveBytes(MGT_UART, arg0, 1);
    if (sizeof(*arg0) != 1) return 0;

    usart_receiveBytes(MGT_UART, arg1, 1);
    if (sizeof(*arg1) != 1) return 0;

    usart_receiveBytes(MGT_UART, arg2, 1);
    if (sizeof(*arg2) != 1) return 0;

    usart_receiveBytes(MGT_UART, &buffer, 1);
    if (buffer[0] != MGT_PACK_DELIMITER) return 0;

    return 1;
}
