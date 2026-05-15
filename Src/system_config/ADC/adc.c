#include "adc.h"


/** Private helper functions */

static bool adc1_isCalibrationDone() { return (ADC1->CR & ADC_CR_ADCAL) != 0; }

static bool adc_isEOCDownADC1() { return !(ADC1->ISR & ADC_ISR_EOC); }
static bool adc_isEOSDownADC1() { return !(ADC1->ISR & ADC_ISR_EOS); }

static bool adc_isADRDYNotResetADC1() { return (ADC1->ISR & ADC_ISR_ADRDY) == 0; }

static void adc_enable(ADC_TypeDef* adc){
	adc->ISR |= ADC_ISR_ADRDY; // Set before enabling ADC (clears ADC ready bit)
	adc->CR |= ADC_CR_ADEN; //Enables ADC

    switch((int) adc) {
        case (int) ADC1:
            wait_with_timeout(adc_isADRDYNotResetADC1, DEFAULT_TIMEOUT_MS); //Waits until ADRDY is reset
            break;
    }
}

static void ADC1_gpio_init() {
    /**
     * Drv0 PC1 ADC1_IN2
     * Drv1 PC2 ADC1_IN3
     * Drv2 PC3 ADC1_IN4
     */

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;


    GPIOC->MODER |= GPIO_MODER_MODE1 | GPIO_MODER_MODE2 | GPIO_MODER_MODE3;
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD1_Msk | GPIO_PUPDR_PUPD2_Msk | GPIO_PUPDR_PUPD3_Msk);

}

static uint8_t adc_calibrateADC(ADC_TypeDef* adc) {
    adc->CR &= ~ADC_CR_DEEPPWD; //makes sure ADC isn't in deep power down mode
	adc->CR |= ADC_CR_ADVREGEN; //enables ADC voltage regulator

	nop(10000);
    adc->CR &= ~ADC_CR_ADEN; //Disable ADC
	adc->CR &= ~ADC_CR_ADCALDIF; //Single-ended input
    adc->CR |= ADC_CR_ADCAL; //Enable Calibration


    // TODO: Fix switch case :(
    //Wait for Calibration to Finish
    switch ((int)adc) {
        case (int)ADC1:
            wait_with_timeout(adc1_isCalibrationDone, DEFAULT_TIMEOUT_MS);
            break;
        default:
            break;
    }

    return adc->CALFACT | ADC_CALFACT_CALFACT_S;
}

static void adcx_initCommon(ADC_TypeDef* adc) {
    adc_calibrateADC(adc);
    // adc->CR |= ADC_CR_ADSTP // Stop any ongoing regular conversions
    // wait_with_timeout(adc_isConversionOngoingADC1, DEFAULT_TIMEOUT_MS); //Waits until ADRDY is reset

    adc->SQR1 &= ~( ADC_SQR1_L ); //Sets number of channels in the sequence of 1
	adc->SQR1 &= ~(ADC_SQR1_SQ1); //Resets the sequence
}

static void ADC1_init() {
    ADC1_gpio_init();
    adcx_initCommon(ADC1);

    ADC1->SMPR1 = 0; 

    //Set sample rate to slowest rate (640.5) per conversion 
    ADC1->SMPR1 |=  ADC_SMPR1_SMP2  | ADC_SMPR1_SMP3 | ADC_SMPR1_SMP4;
    adc_enable(ADC1);

}

/** Private helper functions */

/** Public Functions */

void adc_init() {
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    
    RCC->CCIPR &= ~RCC_CCIPR_ADCSEL; //enables peripheral clock
	RCC->CCIPR |= RCC_CCIPR_ADCSEL_SYSCLK; //Sets ADC clock to system clock

    ADC1_init();
    // ADC2_init();
    // ADC3_init();
    ADC1_COMMON->CCR |= ADC_CCR_VREFEN;

    // Internal voltage reference buffer not available
    // VREFBUF->CSR |= VREFBUF_CSR_ENVR; //Enables internal reference buffer
	// VREFBUF->CSR &= ~(VREFBUF_CSR_HIZ); //Set to internal voltage reference mode (this is default high)

    VREFBUF->CSR &= ~(VREFBUF_CSR_ENVR);    // Disable internal reference buffer
    VREFBUF->CSR |= VREFBUF_CSR_HIZ;        // Set to External voltage reference mode (VREF+ pin input mode)


	// while(!(VREFBUF->CSR & VREFBUF_CSR_VRR)); //Waits until voltage reference value reaches expected output
	// VREFBUF->CSR |= VREFBUF_CSR_VRS; //Sets internal reference buffer to around 2.5V


}

/**
 * @param adc   ADC1 global
 * @param channel   2,3, or 4 according to the pin being read
 * Drv0 PC1 ADC1_IN2 - Channel 2;
 * Drv1 PC2 ADC1_IN3 - Channel 3;
 * Drv2 PC3 ADC1_IN4 - Channel 4
 */
uint16_t adc_readChannel(ADC_TypeDef* adc, int channel) {
   adc->CR &= ~ADC_CR_ADSTART;
   adc->SQR1 |= channel << ADC_SQR1_SQ1_Pos; //Set the channel in sequence to be converted
    
    adc->CR  |=  ( ADC_CR_ADSTART ); //Start conversion

	// Wait for the 'End Of Conversion' flag.
    switch((int)adc) {
        case (int) ADC1:
            wait_with_timeout(adc_isEOCDownADC1, DEFAULT_TIMEOUT_MS);    
            break;
    }
	// Read the converted value (this also clears the EOC flag).
	uint16_t adc_val = adc->DR;

	// Wait for the 'End Of Sequence' flag and clear it.
	switch((int)adc) {
        case (int) ADC1:
            wait_with_timeout(adc_isEOSDownADC1, DEFAULT_TIMEOUT_MS);    
            break;
    }
	adc->ISR |=  ( ADC_ISR_EOS );

    adc->SQR1 &= ~(channel << ADC_SQR1_SQ1_Pos); //Remove the channel from the sequence

	// Return the ADC value.
	return adc_val;
}

/**
 * @param channelReading 12 bit reading from ADC data register output from adc_readChannel()   
 * 
 * @return reading in Voltage
 */
float adc_readVoltage(uint16_t channelReading) {
    return (channelReading / MAX_12_BIT_READING) * EXTERNAL_VOLTAGE_REFERENCE;
}

/** Public Functions */
