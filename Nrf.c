#include "Nrf.h"

typedef struct
{
	Id_t irq_gpio_id;
	uint8_t pinIrq;
	Id_t ce_gpio_id;
	uint8_t pinCe;
	Id_t csn_gpio_id;
	uint8_t pinCsn;
	Id_t spi_gpio_id;
	uint8_t pinNss;
	uint8_t pinSck;
	uint8_t pinMiso;
	uint8_t pinMosi;
	Id_t spi_id;
}Nrf_t;

static Nrf_t nrf[ NRF_NUMBER ];

void Nrf_init( Id_t id )
{
	size_t pin = 0;
	Gpio_initPin( nrf[ id ].ce_gpio_id, nrf[ id ].pinCe, OUTPUT );
	Gpio_initPin( nrf[ id ].csn_gpio_id, nrf[ id ].pinCsn, OUTPUT );
	for ( pin = nrf[ id ].pinNss; pin < ( nrf[ id ].pinNss + 4 ); pin++ )
	{
		Gpio_initPin( nrf[ id ].spi_gpio_id, pin, OUTPUT_AF );
	}
	Spi_init( nrf[ id ].spi_id );

	Nrf_ce( (Id_t) id, LOW );
	Nrf_csn( (Id_t) id, HIGH );

	//Reset Interrupt
	Nrf_writeByte( (Id_t) id, CONFIG, BV( MASK_RX_DR ) | BV( MASK_TX_DS ) | BV( MASK_MAX_RT ) | BV( EN_CRC ) | BV( CRCO ) );
	//Reset Auto Acknowledge
	Nrf_writeByte( (Id_t) id, EN_AA, 0x00 );
	//Enable Payloads
	//Nrf_WriteByte( (Id_t) id, EN_RXADDR, _BV( ERX_P0 ) );
	//Set Address Size
	Nrf_writeByte( (Id_t) id, SETUP_AW, 0x03 );
	//Set Retries
	Nrf_writeByte( (Id_t) id, SETUP_RETR, ( ( DELAY & 0xF )<<ARD | ( COUNT & 0xF )<<ARC ) );
	//Set Channel to 0
	Nrf_writeByte( (Id_t) id, RF_CH, 0x00 );
	//Set DataRate to RF24_250KPS
	Nrf_writeByte( (Id_t) id, RF_SETUP, ( Nrf_readByte( (Id_t) id, RF_SETUP ) | BV( RF_DR_LOW ) ) & ~BV( RF_DR_HIGH ) );
	//Set PA to RF24_PA_MAX
	Nrf_writeByte( (Id_t) id, RF_SETUP, ( Nrf_readByte( (Id_t) id, RF_SETUP ) & 0xF8 ) | RF24_PA_MAX<<RF_PWR );
	//Nrf_writePage( (Id_t) id, TX_ADDR, NRF_Addr_G[ 0 ], ADDR_WIDTH );
	//Nrf_writePage( (Id_t) id, RX_ADDR_P0, NRF_Addr_G[ 0 ], ADDR_WIDTH );
	//Nrf_writeByte( (Id_t) id, RX_PW_P0, PAYLOAD_SIZE );
	Nrf_writeByte( (Id_t) id, FEATURE, 0x00 );
	Nrf_writeByte( (Id_t) id, DYNPD, 0x00 );

	Nrf_writeByte( (Id_t) id, STATUS, BV( RX_DR ) | BV( TX_DS ) | BV( MAX_RT ) );
	Nrf_flushTx( (Id_t) id );
	Nrf_flushRx( (Id_t) id );
}

void Nrf_setGpioCe( Id_t id, Id_t gpio_id, uint8_t pin )
{
	nrf[ id ].ce_gpio_id = gpio_id;
	nrf[ id ].pinCe = pin;
}

void Nrf_setGpioCsn( Id_t id, Id_t gpio_id, uint8_t pin )
{
	nrf[ id ].csn_gpio_id = gpio_id;
	nrf[ id ].pinCsn = pin;
}

void Nrf_setGpioSpi( Id_t id, Id_t gpio_id, uint8_t pin )
{
	nrf[ id ].spi_gpio_id = gpio_id;
	nrf[ id ].pinNss = pin;
}

void Nrf_setSpi( Id_t id, Id_t spi_id )
{
	nrf[ id ].spi_id = spi_id;
}

void Nrf_csn( Id_t id, uint8_t state )
{
	if( state )
	{
		Gpio_setPinState( nrf[ id ].csn_gpio_id, nrf[ id ].pinCsn, HIGH );
	}else
	{
		Gpio_setPinState( nrf[ id ].csn_gpio_id, nrf[ id ].pinCsn, LOW );
	}
}

void Nrf_ce( Id_t id, uint8_t state )
{
	if( state )
	{
		Gpio_setPinState( nrf[ id ].ce_gpio_id, nrf[ id ].pinCe, HIGH );
	}else
	{
		Gpio_setPinState( nrf[ id ].ce_gpio_id, nrf[ id ].pinCe, LOW );
	}
}

void Nrf_writeByte( Id_t id, uint8_t reg, uint8_t buff )
{
	Nrf_csn( id, LOW );
	Spi_transfer( nrf[ id ].spi_id, W_REGISTER | ( REGISTER_MASK & reg ) );
	Spi_transfer( nrf[ id ].spi_id, buff );
	Nrf_csn( id, HIGH );
}

void Nrf_writePage( Id_t id, uint8_t reg, uint8_t *buff, uint8_t len )
{
	buff = buff + len - 1;
	Nrf_csn( id, LOW );
	Spi_transfer( nrf[ id ].spi_id, W_REGISTER | ( REGISTER_MASK & reg ) );
	while( len-- )
	{
		Spi_transfer( nrf[ id ].spi_id, *buff-- );
	}
	Nrf_csn( id, HIGH );
}

uint8_t Nrf_readByte( Id_t id, uint8_t reg )
{
	uint8_t buff;
	Nrf_csn( id, LOW );
	Spi_transfer( nrf[ id ].spi_id, R_REGISTER | ( REGISTER_MASK & reg ) );
	buff = Spi_transfer( nrf[ id ].spi_id, NOP );
	Nrf_csn( id, HIGH );
	return buff;
}

void Nrf_readPage( Id_t id, uint8_t reg, uint8_t *buff, uint8_t len )
{
	buff = buff + len - 1;
	Nrf_csn( id, LOW );
	Spi_transfer( nrf[ id ].spi_id, R_REGISTER | ( REGISTER_MASK & reg ) );
	while( len-- )
	{
		*buff-- = Spi_transfer( nrf[ id ].spi_id, NOP );
	}
	Nrf_csn( id, HIGH );
}

void Nrf_flushTx( Id_t id )
{
	Nrf_csn( id, LOW );
  Spi_transfer( nrf[ id ].spi_id, FLUSH_TX );
	Nrf_csn( id, HIGH );
}

void Nrf_flushRx( Id_t id )
{
	Nrf_csn( id, LOW );
  Spi_transfer( nrf[ id ].spi_id, FLUSH_RX );
	Nrf_csn( id, HIGH );
}

void Nrf_startTxMode( Id_t id )
{
	Nrf_writeByte( id, CONFIG, ( Nrf_readByte( id, CONFIG ) | BV( PWR_UP ) ) & ~BV( PRIM_RX ) );
	//DELAY_US( 150 );
}

void Nrf_startRxMode( Id_t id )
{
	Nrf_writeByte( id, CONFIG, Nrf_readByte( id, CONFIG ) | BV( PWR_UP ) | BV( PRIM_RX ) );
	Nrf_ce( id, HIGH );
}

void Nrf_stopRxMode( Id_t id )
{
	//DELAY_US( 130 );
	Nrf_ce( id, LOW );
}

uint8_t Nrf_checkTxFlag( Id_t id )
{
	if( Nrf_readByte( id, STATUS ) & BV( TX_DS ) )
	{
		Nrf_writeByte( id, STATUS, Nrf_readByte( id, STATUS ) | BV( TX_DS ) );
		return TRUE;
	}else
	{
		if( Nrf_readByte( id, STATUS ) & BV( MAX_RT ) )
		{
			Nrf_writeByte( id, STATUS, Nrf_readByte( id, STATUS ) | BV( MAX_RT ) );
			Spi_transfer( nrf[ id ].spi_id, REUSE_TX_PL );
			Nrf_ce( id, HIGH );
			DELAY_US( 15 );
			Nrf_ce( id, LOW );
		}
		return FALSE;
	}
}

uint8_t Nrf_checkRxFlag( Id_t id )
{
	if( Nrf_readByte( id, STATUS ) & BV( RX_DR ) )
	{
		Nrf_writeByte( id, STATUS, Nrf_readByte( id, STATUS ) | BV( RX_DR ) );
		return TRUE;
	}else
	{
		return FALSE;
	}
}

void Nrf_transmit( Id_t id, uint8_t *buffer )
{
	uint8_t length = PAYLOAD_SIZE;
	Nrf_csn( id, LOW );
	Spi_transfer( nrf[ id ].spi_id, W_TX_PAYLOAD );
	while( length-- )
	{
		Spi_transfer( nrf[ id ].spi_id, *buffer++ );
	}
	Nrf_csn( id, HIGH );
	Nrf_ce( id, HIGH );
	DELAY_US( 15 );
	Nrf_ce( id, LOW );
}

void Nrf_receive( Id_t id, uint8_t *buffer )
{
	uint8_t length = PAYLOAD_SIZE;
	Nrf_csn( id, LOW );
	Spi_transfer( nrf[ id ].spi_id, R_RX_PAYLOAD );
	while( length-- )
	{
		*buffer++ = Spi_transfer( nrf[ id ].spi_id, NOP );
	}
	Nrf_csn( id, HIGH );
}

void Nrf_ack( Id_t id, uint8_t *buffer )
{
	uint8_t length = PAYLOAD_SIZE;
	Nrf_csn( id, LOW );
	Spi_transfer( nrf[ id ].spi_id, W_ACK_PAYLOAD );
	while( length-- )
	{
		Spi_transfer( nrf[ id ].spi_id, *buffer++ );
	}
	Nrf_csn( id, HIGH );
}

void Nrf_setChannel( Id_t id, uint8_t channel )
{
  Nrf_writeByte( id, RF_CH, channel );
}

void Nrf_openWritingPipe( Id_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P0 ) );
  Nrf_writePage( id, TX_ADDR, address, ADDR_WIDTH );
  Nrf_writePage( id, RX_ADDR_P0, address, ADDR_WIDTH );
  Nrf_writeByte( id, RX_PW_P0, PAYLOAD_SIZE );
}

void Nrf_openReadingPipe1( Id_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P1 ) );
	Nrf_writePage( id, RX_ADDR_P1, address, ADDR_WIDTH );
  Nrf_writeByte( id, RX_PW_P1, PAYLOAD_SIZE );
}

void Nrf_openReadingPipe2( Id_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P2 ) );
	Nrf_writeByte( id, RX_ADDR_P2, address[ 4 ] );
  Nrf_writeByte( id, RX_PW_P2, PAYLOAD_SIZE );
}

void Nrf_openReadingPipe3( Id_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P3 ) );
	Nrf_writeByte( id, RX_ADDR_P3, address[ 4 ] );
  Nrf_writeByte( id, RX_PW_P3, PAYLOAD_SIZE );
}

void Nrf_openReadingPipe4( Id_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P4 ) );
	Nrf_writeByte( id, RX_ADDR_P4, address[ 4 ] );
  Nrf_writeByte( id, RX_PW_P4, PAYLOAD_SIZE );
}

void Nrf_openReadingPipe5( Id_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P5 ) );
	Nrf_writeByte( id, RX_ADDR_P5, address[ 4 ] );
  Nrf_writeByte( id, RX_PW_P5, PAYLOAD_SIZE );
}
