#include <stdint.h>
#include <print_scan.h>
#include "platform_init.h"
#include <ADC/adc.h>
#include "globals.h"



#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
    init_platform();
    adc_init();




    while (1) {

    int chan2 = adc_readChannel(ADC1, 2);
    int chan3 = adc_readChannel(ADC1, 3);
    int v2 = adc_readVoltage(chan2);
    int v3 = adc_readVoltage(chan3);
        
    }
}
