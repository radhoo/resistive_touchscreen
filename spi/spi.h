#pragma once

#include "../config.h"
#include <avr/io.h>



#if ATMEGA8
	#define SPI_DDR DDRB
	#define SPI_MOSI PB3
	#define SPI_MISO PB4
	#define SPI_SCK PB5
#elif ATMEGA128
	#define SPI_DDR DDRB
	#define SPI_PORT PORTB
	#define SPI_SCK PB1
	#define SPI_MOSI PB2
	//#define SPI_MISO PB3
#endif

/**
 * set spi speed and settings
 */
void spi_init(void);

/**
 * send spi data
 */
void spi_send(uint8_t data);
