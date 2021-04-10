#include "Nrf.h"

typedef struct
{
	GPIO_ID_t gpioIdIrq;
	GPIO_PIN_t gpioPinIrq;
	GPIO_ID_t gpioIdCe;
	GPIO_PIN_t gpioPinCe;
	GPIO_ID_t gpioIdCsn;
	GPIO_PIN_t gpioPinCsn;
	GPIO_ID_t gpioIdCom;
	GPIO_PIN_t gpioPinCom;
	SPI_ID_t spiId;
	NRF_NODE_t node;
	uint8_t freq;
	uint8_t state;
	uint8_t safty;
	uint8_t address[ NRF_NODE_MAX ][ ADDR_WIDTH ];
	uint8_t txBuffer[ NRF_NODE_MAX ][ PAYLOAD_SIZE ];
	uint8_t rxBuffer[ NRF_NODE_MAX ][ PAYLOAD_SIZE ];
	uint8_t rTxIndex;
	uint8_t rRxIndex;
	uint8_t wTxIndex;
	uint8_t wRxIndex;
}	Nrf_t;

static Nrf_t nrf[ NRF_ID_MAX ];

void initNrfHardware( NRF_ID_t id );

void Nrf_init( NRF_ID_t id, GPIO_ID_t gpioIdCe, GPIO_PIN_t gpioPinCe,
							 GPIO_ID_t gpioIdCsn, GPIO_PIN_t gpioPinCsn, GPIO_ID_t gpioIdCom, GPIO_PIN_t gpioPinCom, SPI_ID_t spiId )
{
	nrf[ id ].gpioIdCe = gpioIdCe;
	nrf[ id ].gpioPinCe = gpioPinCe;
	nrf[ id ].gpioIdCsn = gpioIdCsn;
	nrf[ id ].gpioPinCsn = gpioPinCsn;
	nrf[ id ].gpioIdCom = gpioIdCom;
	nrf[ id ].gpioPinCom = gpioPinCom;
	nrf[ id ].spiId = spiId;
	nrf[ id ].node = NRF_NODE_0;
	nrf[ id ].freq = 0;
	nrf[ id ].state = 0;
	nrf[ id ].safty = 0;
	for( size_t i = 0; i < NRF_NODE_MAX; i++ )
	{
		for( size_t j = 0; j < ( ADDR_WIDTH - 1 ); j++ )
		{
			nrf[ id ].address[ i ][ j ] = 0xF0;
		}
		nrf[ id ].address[ i ][ 4 ] = (uint8_t) i;
	}
	Nrf_clearTxBuffer( id );
	Nrf_clearRxBuffer( id );

	initNrfHardware( id );
	Nrf_ce( id, NRF_STATE_LOW );
	Nrf_csn( id, NRF_STATE_HIGH );

	//Reset Interrupt
	Nrf_writeByte( id, CONFIG, BV( MASK_RX_DR ) | BV( MASK_TX_DS ) | BV( MASK_MAX_RT ) | BV( EN_CRC ) | BV( CRCO ) );
	//Reset Auto Acknowledge
	Nrf_writeByte( id, EN_AA, 0x00 );
	//Enable Payloads
	//Nrf_WriteByte( id, EN_RXADDR, _BV( ERX_P0 ) );
	//Set Address Size
	Nrf_writeByte( id, SETUP_AW, 0x03 );
	//Set Retries
	Nrf_writeByte( id, SETUP_RETR, ( ( DELAY & 0xF )<<ARD | ( COUNT & 0xF )<<ARC ) );
	//Set Channel to 0
	Nrf_writeByte( id, RF_CH, 0x00 );
	//Set DataRate to RF24_250KPS
	Nrf_writeByte( id, RF_SETUP, ( Nrf_readByte( id, RF_SETUP ) | BV( RF_DR_LOW ) ) & ~BV( RF_DR_HIGH ) );
	//Set PA to RF24_PA_MAX
	Nrf_writeByte( id, RF_SETUP, ( Nrf_readByte( id, RF_SETUP ) & 0xF8 ) | NRF_PA_MAX<<RF_PWR );
	//Nrf_writePage( id, TX_ADDR, NRF_Addr_G[ 0 ], ADDR_WIDTH );
	//Nrf_writePage( id, RX_ADDR_P0, NRF_Addr_G[ 0 ], ADDR_WIDTH );
	//Nrf_writeByte( id, RX_PW_P0, PAYLOAD_SIZE );
	Nrf_writeByte( id, FEATURE, 0x00 );
	Nrf_writeByte( id, DYNPD, 0x00 );

	Nrf_writeByte( id, STATUS, BV( RX_DR ) | BV( TX_DS ) | BV( MAX_RT ) );
	Nrf_flushTx( id );
	Nrf_flushRx( id );
}

void Nrf_reinit( NRF_ID_t id )
{
	Nrf_ce( id, NRF_STATE_LOW );
	Nrf_csn( id, NRF_STATE_HIGH );

	//Reset Interrupt
	Nrf_writeByte( id, CONFIG, BV( MASK_RX_DR ) | BV( MASK_TX_DS ) | BV( MASK_MAX_RT ) | BV( EN_CRC ) | BV( CRCO ) );
	//Reset Auto Acknowledge
	Nrf_writeByte( id, EN_AA, 0x00 );
	//Enable Payloads
	//Nrf_WriteByte( id, EN_RXADDR, _BV( ERX_P0 ) );
	//Set Address Size
	Nrf_writeByte( id, SETUP_AW, 0x03 );
	//Set Retries
	Nrf_writeByte( id, SETUP_RETR, ( ( DELAY & 0xF )<<ARD | ( COUNT & 0xF )<<ARC ) );
	//Set Channel to 0
	Nrf_writeByte( id, RF_CH, 0x00 );
	//Set DataRate to RF24_250KPS
	Nrf_writeByte( id, RF_SETUP, ( Nrf_readByte( id, RF_SETUP ) | BV( RF_DR_LOW ) ) & ~BV( RF_DR_HIGH ) );
	//Set PA to RF24_PA_MAX
	Nrf_writeByte( id, RF_SETUP, ( Nrf_readByte( id, RF_SETUP ) & 0xF8 ) | NRF_PA_MAX<<RF_PWR );
	//Nrf_writePage( id, TX_ADDR, NRF_Addr_G[ 0 ], ADDR_WIDTH );
	//Nrf_writePage( id, RX_ADDR_P0, NRF_Addr_G[ 0 ], ADDR_WIDTH );
	//Nrf_writeByte( id, RX_PW_P0, PAYLOAD_SIZE );
	Nrf_writeByte( id, FEATURE, 0x00 );
	Nrf_writeByte( id, DYNPD, 0x00 );

	Nrf_writeByte( id, STATUS, BV( RX_DR ) | BV( TX_DS ) | BV( MAX_RT ) );
	Nrf_flushTx( id );
	Nrf_flushRx( id );
}

void initNrfHardware( NRF_ID_t id )
{
	Gpio_initPin( nrf[ id ].gpioIdCe, nrf[ id ].gpioPinCe, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
	Gpio_initPin( nrf[ id ].gpioIdCsn, nrf[ id ].gpioPinCsn, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
	Gpio_initPin( nrf[ id ].gpioIdCom, nrf[ id ].gpioPinCom, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_AF_PP );
	Gpio_initPin( nrf[ id ].gpioIdCom, nrf[ id ].gpioPinCom+1, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_AF_PP );
	Gpio_initPin( nrf[ id ].gpioIdCom, nrf[ id ].gpioPinCom+2, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_AF_PP );
	Gpio_initPin( nrf[ id ].gpioIdCom, nrf[ id ].gpioPinCom+3, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_AF_PP );
	Spi_init( nrf[ id ].spiId );
}

void Nrf_csn( NRF_ID_t id, NRF_STATE_t state )
{
	if( state == NRF_STATE_HIGH )
	{
		Gpio_setPinState( nrf[ id ].gpioIdCsn, nrf[ id ].gpioPinCsn, GPIO_STATE_HIGH );
	}else if( state == NRF_STATE_LOW )
	{
		Gpio_setPinState( nrf[ id ].gpioIdCsn, nrf[ id ].gpioPinCsn, GPIO_STATE_LOW );
	}else
	{
		
	}
}

void Nrf_ce( NRF_ID_t id, NRF_STATE_t state )
{
	if( state == NRF_STATE_HIGH )
	{
		Gpio_setPinState( nrf[ id ].gpioIdCe, nrf[ id ].gpioPinCe, GPIO_STATE_HIGH );
	}else	if( state == NRF_STATE_LOW )
	{
		Gpio_setPinState( nrf[ id ].gpioIdCe, nrf[ id ].gpioPinCe, GPIO_STATE_LOW );
	}else
	{
		
	}
}

void Nrf_writeByte( NRF_ID_t id, NRF_REG_t reg, uint8_t buff )
{
	Nrf_csn( id, NRF_STATE_LOW );
	Spi_transfer( nrf[ id ].spiId, W_REGISTER | ( REGISTER_MASK & reg ) );
	Spi_transfer( nrf[ id ].spiId, buff );
	Nrf_csn( id, NRF_STATE_HIGH );
}

void Nrf_writePage( NRF_ID_t id, NRF_REG_t reg, uint8_t *buff, uint8_t len )
{
	buff = buff + len - 1;
	Nrf_csn( id, NRF_STATE_LOW );
	Spi_transfer( nrf[ id ].spiId, W_REGISTER | ( REGISTER_MASK & reg ) );
	while( len-- )
	{
		Spi_transfer( nrf[ id ].spiId, *buff-- );
	}
	Nrf_csn( id, NRF_STATE_HIGH );
}

uint8_t Nrf_readByte( NRF_ID_t id, NRF_REG_t reg )
{
	uint8_t buff;
	Nrf_csn( id, NRF_STATE_LOW );
	Spi_transfer( nrf[ id ].spiId, R_REGISTER | ( REGISTER_MASK & reg ) );
	buff = Spi_transfer( nrf[ id ].spiId, NOP );
	Nrf_csn( id, NRF_STATE_HIGH );
	return buff;
}

void Nrf_readPage( NRF_ID_t id, NRF_REG_t reg, uint8_t *buff, uint8_t len )
{
	buff = buff + len - 1;
	Nrf_csn( id, NRF_STATE_LOW );
	Spi_transfer( nrf[ id ].spiId, R_REGISTER | ( REGISTER_MASK & reg ) );
	while( len-- )
	{
		*buff-- = Spi_transfer( nrf[ id ].spiId, NOP );
	}
	Nrf_csn( id, NRF_STATE_HIGH );
}

void Nrf_flushTx( NRF_ID_t id )
{
	Nrf_csn( id, NRF_STATE_LOW );
  Spi_transfer( nrf[ id ].spiId, FLUSH_TX );
	Nrf_csn( id, NRF_STATE_HIGH );
}

void Nrf_flushRx( NRF_ID_t id )
{
	Nrf_csn( id, NRF_STATE_LOW );
  Spi_transfer( nrf[ id ].spiId, FLUSH_RX );
	Nrf_csn( id, NRF_STATE_HIGH );
}

void Nrf_startTxMode( NRF_ID_t id )
{
	Nrf_writeByte( id, CONFIG, ( Nrf_readByte( id, CONFIG ) | BV( PWR_UP ) ) & ~BV( PRIM_RX ) );
	DELAY_US( 50 );
}

void Nrf_startRxMode( NRF_ID_t id )
{
	Nrf_writeByte( id, CONFIG, Nrf_readByte( id, CONFIG ) | BV( PWR_UP ) | BV( PRIM_RX ) );
	Nrf_ce( id, NRF_STATE_HIGH );
}

void Nrf_stopRxMode( NRF_ID_t id )
{
	DELAY_US( 50 );
	Nrf_ce( id, NRF_STATE_LOW );
}

uint8_t Nrf_checkTxFlag( NRF_ID_t id )
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
			Spi_transfer( nrf[ id ].spiId, REUSE_TX_PL );
			Nrf_ce( id, NRF_STATE_HIGH );
			DELAY_US( 15 );
			Nrf_ce( id, NRF_STATE_LOW );
		}
		return FALSE;
	}
}

uint8_t Nrf_checkRxFlag( NRF_ID_t id )
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

void Nrf_transmit( NRF_ID_t id, uint8_t *buffer )
{
	uint8_t length = PAYLOAD_SIZE;
	Nrf_csn( id, NRF_STATE_LOW );
	Spi_transfer( nrf[ id ].spiId, W_TX_PAYLOAD );
	while( length-- )
	{
		Spi_transfer( nrf[ id ].spiId, *buffer++ );
	}
	Nrf_csn( id, NRF_STATE_HIGH );
	Nrf_ce( id, NRF_STATE_HIGH );
	DELAY_US( 15 );
	Nrf_ce( id, NRF_STATE_LOW );
}

void Nrf_receive( NRF_ID_t id, uint8_t *buffer )
{
	uint8_t length = PAYLOAD_SIZE;
	Nrf_csn( id, NRF_STATE_LOW );
	Spi_transfer( nrf[ id ].spiId, R_RX_PAYLOAD );
	while( length-- )
	{
		*buffer++ = Spi_transfer( nrf[ id ].spiId, NOP );
	}
	Nrf_csn( id, NRF_STATE_HIGH );
}

void Nrf_writeAck( NRF_ID_t id, uint8_t *buffer )
{
	uint8_t length = PAYLOAD_SIZE;
	Nrf_csn( id, NRF_STATE_LOW );
	Spi_transfer( nrf[ id ].spiId, W_ACK_PAYLOAD );
	while( length-- )
	{
		Spi_transfer( nrf[ id ].spiId, *buffer++ );
	}
	Nrf_csn( id, NRF_STATE_HIGH );
}

void Nrf_setChannel( NRF_ID_t id, uint8_t channel )
{
  Nrf_writeByte( id, RF_CH, channel );
}

void Nrf_openWritingPipe( NRF_ID_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P0 ) );
  Nrf_writePage( id, TX_ADDR, address, ADDR_WIDTH );
  Nrf_writePage( id, RX_ADDR_P0, address, ADDR_WIDTH );
  Nrf_writeByte( id, RX_PW_P0, PAYLOAD_SIZE );
}

void Nrf_openReadingPipe0( NRF_ID_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P0 ) );
	Nrf_writePage( id, RX_ADDR_P0, address, ADDR_WIDTH );
  Nrf_writeByte( id, RX_PW_P0, PAYLOAD_SIZE );
}

void Nrf_openReadingPipe1( NRF_ID_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P1 ) );
	Nrf_writePage( id, RX_ADDR_P1, address, ADDR_WIDTH );
  Nrf_writeByte( id, RX_PW_P1, PAYLOAD_SIZE );
}

void Nrf_openReadingPipe2( NRF_ID_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P2 ) );
	Nrf_writeByte( id, RX_ADDR_P2, address[ 4 ] );
  Nrf_writeByte( id, RX_PW_P2, PAYLOAD_SIZE );
}

void Nrf_openReadingPipe3( NRF_ID_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P3 ) );
	Nrf_writeByte( id, RX_ADDR_P3, address[ 4 ] );
  Nrf_writeByte( id, RX_PW_P3, PAYLOAD_SIZE );
}

void Nrf_openReadingPipe4( NRF_ID_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P4 ) );
	Nrf_writeByte( id, RX_ADDR_P4, address[ 4 ] );
  Nrf_writeByte( id, RX_PW_P4, PAYLOAD_SIZE );
}

void Nrf_openReadingPipe5( NRF_ID_t id, uint8_t *address )
{
	Nrf_writeByte( id, EN_RXADDR, BV( ERX_P5 ) );
	Nrf_writeByte( id, RX_ADDR_P5, address[ 4 ] );
  Nrf_writeByte( id, RX_PW_P5, PAYLOAD_SIZE );
}

void Nrf_setNodeNumber( NRF_ID_t id, NRF_NODE_t node )
{
	nrf[ id ].node = node;
}

NRF_NODE_t Nrf_getNodeNumber( NRF_ID_t id )
{
	return nrf[ id ].node;
}

void Nrf_setTxBuffer( NRF_ID_t id, NRF_NODE_t node, uint8_t index, uint8_t buffer )
{
	nrf[ id ].txBuffer[ node ][ index ] = buffer;
}

uint8_t Nrf_getRxBuffer( NRF_ID_t id, NRF_NODE_t node, uint8_t index )
{
	return nrf[ id ].rxBuffer[ node ][ index ];
}

void Nrf_clearTxBuffer( NRF_ID_t id )
{
	for( size_t i = 0; i < NRF_NODE_MAX; i++ )
	{
		for( size_t j = 0; j < PAYLOAD_SIZE; j++ )
		{
			nrf[ id ].txBuffer[ i ][ j ] = 0;
		}
	}
}

void Nrf_clearRxBuffer( NRF_ID_t id )
{
	for( size_t i = 0; i < NRF_NODE_MAX; i++ )
	{
		for( size_t j = 0; j < PAYLOAD_SIZE; j++ )
		{
			nrf[ id ].rxBuffer[ i ][ j ] = 0;
		}
	}
}

void Nrf_updateMaster( void *paramter )
{
	NRF_ID_t id = (NRF_ID_t) paramter;
	switch( nrf[ id ].state )
	{
		case 0:
		{
			nrf[ id ].node++;
			if( nrf[ id ].node == NRF_NODE_MAX )
			{
				nrf[ id ].node = NRF_NODE_1;
			}
			Nrf_startTxMode( id );
			Nrf_openWritingPipe( id, nrf[ id ].address[ nrf[ id ].node ] );
			Nrf_transmit( id, nrf[ id ].txBuffer[ nrf[ id ].node ] );
			nrf[ id ].state = 1;
			break;
		}
		case 1:
		{
			nrf[ id ].safty++;
			if( Nrf_checkTxFlag( id ) )
			{
				Nrf_startRxMode( id );
				Nrf_openReadingPipe0( id, nrf[ id ].address[ nrf[ id ].node ] );
				nrf[ id ].safty = 0;
				nrf[ id ].state = 2;
			}else if( nrf[ id ].safty == MS_TO_TICKS( 15 ) / MS_TO_TICKS( 1 ) )
			{
				Nrf_reinit( id );
				nrf[ id ].safty = 0;
				nrf[ id ].state = 0;
			}
			break;
		}
		case 2:
		{
			nrf[ id ].safty++;
			if( Nrf_checkRxFlag( id ) )
			{
				Nrf_receive( id, nrf[ id ].rxBuffer[ nrf[ id ].node ] );
				Nrf_stopRxMode( id );
				nrf[ id ].safty = 0;
				nrf[ id ].state = 0;
				nrf[ id ].txBuffer[ nrf[ id ].rxBuffer[ nrf[ id ].node ][ 0 ] ][ 0 ] = nrf[ id ].rxBuffer[ nrf[ id ].node ][ 0 ];
				nrf[ id ].txBuffer[ nrf[ id ].rxBuffer[ nrf[ id ].node ][ 0 ] ][ 1 ] = nrf[ id ].rxBuffer[ nrf[ id ].node ][ 1 ];
				nrf[ id ].txBuffer[ nrf[ id ].rxBuffer[ nrf[ id ].node ][ 0 ] ][ 2 ] = nrf[ id ].rxBuffer[ nrf[ id ].node ][ 2 ];
				nrf[ id ].txBuffer[ nrf[ id ].rxBuffer[ nrf[ id ].node ][ 0 ] ][ 3 ] = nrf[ id ].rxBuffer[ nrf[ id ].node ][ 3 ];
				nrf[ id ].txBuffer[ nrf[ id ].rxBuffer[ nrf[ id ].node ][ 0 ] ][ 4 ] = nrf[ id ].rxBuffer[ nrf[ id ].node ][ 4 ];
				nrf[ id ].freq ^= 1;
				if( nrf[ id ].freq )
				{
					Led_setState( 1, LED_STATE_ON, LED_BLINK_0MS );
				}else
				{
					Led_setState( 1, LED_STATE_OFF, LED_BLINK_0MS );
				}
			}else if( nrf[ id ].safty == MS_TO_TICKS( 20 ) / MS_TO_TICKS( 1 ) )
			{
				Nrf_stopRxMode( id );
				Nrf_reinit( id );
				nrf[ id ].safty = 0;
				nrf[ id ].state = 0;
			}
			break;
		}
	}
}

void Nrf_updateSlave( void *paramter )
{
	NRF_ID_t id = (NRF_ID_t) paramter;
	switch( nrf[ id ].state )
	{
		case 0:
		{
			Nrf_startRxMode( id );
			Nrf_openReadingPipe0( id, nrf[ id ].address[ nrf[ id ].node ] );
			nrf[ id ].state = 1;
			break;
		}
		case 1:
		{
			nrf[ id ].safty++;
			if( Nrf_checkRxFlag( id ) )
			{
				Nrf_receive( id, nrf[ id ].rxBuffer[ 0 ] );
				Nrf_stopRxMode( id );
				nrf[ id ].safty = 0;
				nrf[ id ].state = 2;
			}else if( nrf[ id ].safty == ( MS_TO_TICKS( 1000 ) / MS_TO_TICKS( 5 ) ) )
			{
				Nrf_reinit( id );
				Nrf_clearRxBuffer( id );
				nrf[ id ].safty = 0;
				nrf[ id ].state = 0;
			}
			break;
		}
		case 2:
		{
			Nrf_startTxMode( id );
			Nrf_openWritingPipe( id, nrf[ id ].address[ nrf[ id ].node ] );
			Nrf_transmit( id, nrf[ id ].txBuffer[ 0 ] );
			nrf[ id ].state = 3;
			break;
		}
		case 3:
		{
			nrf[ id ].safty++;
			if( Nrf_checkTxFlag( id ) )
			{
				Nrf_startRxMode( id );
				Nrf_openReadingPipe0( id, nrf[ id ].address[ nrf[ id ].node ] );
				nrf[ id ].state = 1;
			}else if( nrf[ id ].safty == ( MS_TO_TICKS( 15 ) / MS_TO_TICKS( 5 ) ) )
			{
				Nrf_reinit( id );
				Nrf_clearRxBuffer( id );
				nrf[ id ].safty = 0;
				nrf[ id ].state = 0;
			}
			break;
		}
	}
}
