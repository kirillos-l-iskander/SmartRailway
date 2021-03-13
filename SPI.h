#ifndef SPI_H
#define	SPI_H

#include "SchedulerConfig.h"

typedef enum
{
	SPI1_ID,
	SPI2_ID
}SPI_t;

void Spi_init( Id_t id );
uint8_t Spi_transfer( Id_t id, uint8_t txBuffer );

#endif	/* SPI_H */
