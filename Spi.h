#ifndef SPI_H
#define	SPI_H

#include "Config.h"

typedef enum
{
	SPI_ID_1 = 0,
	SPI_ID_2,
	SPI_ID_MAX,
}	SPI_ID_t;

void Spi_init( SPI_ID_t id );
uint8_t Spi_transfer( SPI_ID_t id, uint8_t txBuffer );

#endif	/* SPI_H */
