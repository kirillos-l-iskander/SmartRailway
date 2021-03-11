#ifndef SPI_H
#define	SPI_H

#include "SchedulerConfig.h"

#define SPI1_ID	( 0 )
#define SPI2_ID	( 1 )

void Spi_init( Id_t id );
uint8_t Spi_transfer( Id_t id, uint8_t txBuffer );

#endif	/* SPI_H */
