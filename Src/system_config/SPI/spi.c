/*
 * spi.c
 *
 *	- August 17, 2024
 *		Author	: Darsh
 *		Log		: Copied from IntelliSat
 */

#include "spi.h"

/**
 * Configures GPIO for the SPI-2 Peripheral
 */
void spi3_gpioInit() {

}


void spi2_gpioInit() {

}

void spi1_gpioInit() {

}

/**************************** SPI INITIALIZATIONS ****************************/

void spi_disable(SPI_TypeDef *spi, GPIO_TypeDef *cs_port, int cs_pin) {
	while(spi->SR & SPI_SR_FTLVL);	// Wait till there is no data to transmit
	while(spi->SR & SPI_SR_BSY);	// Wait till last data frame is processed
	spi_stopCommunication(cs_port, cs_pin);
	spi->CR1 &= ~SPI_CR1_SPE;		// Disable SPI2

	uint8_t temp;
	while(spi->SR & SPI_SR_FRLVL){
		// Wait till all data is received
		temp = SPI2->DR;
	}
}

void spi1_config() {
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;	// GPIO
	spi1_gpioInit();

	spi_disable(SPI1, SPI1_CS);

	SPI1->CR1 = 0;
	SPI1->CR2 = 0;
	// CR1
	// CR2

	spi_enable(SPI1);

}

void spi2_config() {
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;	// Clock
	spi2_gpioInit();						// GPIO

	spi_disable(SPI2, SPI2_CS);

	SPI2->CR1 = 0;
	SPI2->CR2 = 0;
	// CR1
	// CR2

	spi_enable(SPI2);
}

void spi3_config() {
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;	// Clock
    spi3_gpioInit();						// GPIO

	spi_disable(SPI3, SPI3_CS);

	SPI3->CR1 = 0;
	SPI3->CR2 = 0;
	// CR1
	// CR2

	spi_enable(SPI3);
}

void spi_config(SPI_TypeDef *spi) {
	switch ((uint32_t)spi) {
		case (uint32_t)SPI1:
			spi1_config();
			break;
		case (uint32_t)SPI2:
			spi2_config();
			break;
		case (uint32_t)SPI3:
			spi3_config();
			break;
	}
}

/***************************** SPI COMMUNICATION *****************************/
void spi_startCommunication(GPIO_TypeDef *cs_port, int cs_pin) {

	gpio_low(cs_port, cs_pin);
}void spi_stopCommunication(GPIO_TypeDef *cs_port, int cs_pin) {
	gpio_high(cs_port, cs_pin);
}

bool spi_transmitReceive(SPI_TypeDef* spi, uint8_t* transmission, uint8_t *reception, uint16_t size, bool dma) {
	while (size-- >= 1) {
		// wait for TXFIFO to be empty
		while(!(spi->SR & SPI_SR_TXE));	// TXE = TX Empty
		if (transmission == NULL) {
			// send a dummy byte to trigger the clock pulses
			*((volatile uint8_t*) &(spi->DR)) = SPI_DUMMY_BYTE;
		} else {
			// fill TXFIFO with the instruction
			*((volatile uint8_t*) &(spi->DR)) = *transmission;
			transmission++;
		}
		while(!(spi->SR & SPI_SR_TXE));

		// read the reception line until it's empty
		while (spi->SR & SPI_SR_RXNE) {	// RXNE = RX Not Empty
			if (reception == NULL) {
				spi->DR;
			} else {
				*reception = spi->DR;
				reception++;
			}
		}
	}
	return true;
}
