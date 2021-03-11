#include "Spi.h"

//CR1
#define SSM (1<<9)
#define SPE (1<<6)
#define BR (6<<3)
#define MSTR (1<<2)

//CR2
#define SSOE (1<<2)

//SR
#define BSY (1<<7)
#define TXE (1<<1)
#define RXNE (1<<0)

typedef SPI_TypeDef Spi_t;

static Spi_t *spi[ 2 ] = { SPI1, SPI2 };

void Spi_init( Id_t id )
{
	spi[ id ]->CR2 |= SSOE;
	spi[ id ]->CR1 |= SSM | SPE | BR | MSTR;
}

uint8_t Spi_transfer( Id_t id, uint8_t txBuffer )
{
	uint8_t rxBuffer = 0;
	while( !( spi[ id ]->SR & TXE ) );
	spi[ id ]->DR = txBuffer;
	while( !( spi[ id ]->SR & RXNE ) );
	rxBuffer = ( uint8_t ) spi[ id ]->DR;
	while( spi[ id ]->SR & BSY );
	return rxBuffer;
}
