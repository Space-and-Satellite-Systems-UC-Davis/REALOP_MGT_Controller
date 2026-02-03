#ifndef ADC_H
#define ADC_H

#include "stm32l431xx.h"
#include <globals.h>

// #define INTERNAL_VOLTAGE_REFERENCE 2.5   // NOT AVAILABLE
#define EXTERNAL_VOLTAGE_REFERENCE 2.0  // CHANGE TO ACCURATE VALUE OF VREF+ VREF-
#define MAX_12_BIT_READING 4095.0

#define RCC_CCIPR_ADCSEL_SYSCLK (3 << RCC_CCIPR_ADCSEL_Pos)
#define RCC_CCIPR_ADCSEL_PLLSAI1 (2 << RCC_CCIPR_ADCSEL_Pos)
#define RCC_CCIPR_ADCSEL_PLLSAI2 (1 << RCC_CCIPR_ADCSEL_Pos)

uint16_t adc_readChannel(ADC_TypeDef* adc, int channel);
float adc_readVoltage(uint16_t channelReading);
void adc_init();

#endif


/*
18.2 VREFBUF functional description
The internal voltage reference buffer supports two voltages (a)
, which are configured with
VRS bits in the VREFBUF_CSR register:
• VRS = 0: VREF_OUT1 around 2.048 V
• VRS = 1: VREF_OUT2 around 2.5 V

*/