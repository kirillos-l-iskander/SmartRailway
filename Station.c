#include "Station.h"

typedef struct
{
	NRF_ID_t xNrfId;
	IRSNSR_ID_t eIrsnsrId;
	IRSNSR_ID_t pIrsnsrId;
	IRSNSR_ID_t wIrsnsrId;
	LCD_ID_t xLcdId;
	uint8_t state;
	uint16_t wait;
	uint8_t empty;
	uint8_t currentAddress;
	uint8_t nextAddress;
	uint8_t targetAddress;
	uint8_t buffer;
	uint8_t pad;
}	Station_t;

static Station_t station[ STATION_ID_MAX ];
static char trainEntry1[ 17 ] = "Train:        <<";
static char trainEntry2[ 17 ] = "Speed:  cm/s    ";
static char trainStop1[ 17 ] = "Train:        XX";
static char trainStop2[ 17 ] = "Speed:  cm/s    ";
static char trainLeave1[ 17 ] = "Train:        <-";
static char trainLeave2[ 17 ] = "Speed:  cm/s    ";

void Station_init( STATION_ID_t id, NRF_ID_t xNrfId, IRSNSR_ID_t eIrsnsrId, IRSNSR_ID_t pIrsnsrId, IRSNSR_ID_t wIrsnsrId, LCD_ID_t xLcdId )
{
	station[ id ].xNrfId = xNrfId;
	station[ id ].eIrsnsrId = eIrsnsrId;
	station[ id ].pIrsnsrId = pIrsnsrId;
	station[ id ].wIrsnsrId = wIrsnsrId;
	station[ id ].xLcdId = xLcdId;
	station[ id ].state = 0;
	station[ id ].wait = 0;
	station[ id ].empty = 1;
	station[ id ].currentAddress = 0;
	station[ id ].nextAddress = 0;
	station[ id ].targetAddress = 0;
	station[ id ].buffer = 0;
	if( Nrf_getNodeNumber( station[ id ].xNrfId ) == NRF_NODE_1 )
	{
		station[ id ].currentAddress = 1;
		station[ id ].nextAddress = 2;
		station[ id ].targetAddress = 3;
		station[ id ].buffer = 15;
	}else if( Nrf_getNodeNumber( station[ id ].xNrfId ) == NRF_NODE_2 )
	{
		station[ id ].currentAddress = 2;
		station[ id ].nextAddress = 1;
		station[ id ].targetAddress = 4;
		station[ id ].buffer = 15;
	}
}

void Station_startState( STATION_ID_t id )
{
	Nrf_setTxBuffer( station[ id ].xNrfId, NRF_NODE_0, 0, station[ id ].targetAddress );
	Nrf_setTxBuffer( station[ id ].xNrfId, NRF_NODE_0, 1, station[ id ].currentAddress );
	Nrf_setTxBuffer( station[ id ].xNrfId, NRF_NODE_0, 2, station[ id ].buffer );
	station[ id ].state = 2;
}

void Station_getTrainAddress( STATION_ID_t id )
{
	if( Nrf_getRxBuffer( station[ id ].xNrfId, NRF_NODE_0, 0 ) == Nrf_getNodeNumber( station[ id ].xNrfId )
		&& ( Nrf_getRxBuffer( station[ id ].xNrfId, NRF_NODE_0, 2 ) != station[ id ].buffer ) )
	{
		station[ id ].state = 2;
		station[ id ].targetAddress = Nrf_getRxBuffer( station[ id ].xNrfId, NRF_NODE_0, 2 );
		station[ id ].buffer = 15;
		Nrf_clearRxBuffer( station[ id ].xNrfId );
	}
}

void Station_detectTrainEnter( STATION_ID_t id )
{
	if( Irsnsr_getState( station[ id ].eIrsnsrId ) == IRSNSR_STATE_HIGH )
	{
		if( station[ id ].empty )
		{
			station[ id ].empty = 0;
			station[ id ].buffer = 10;
			station[ id ].state = 3;
			Lcd_setString( station[ id ].xLcdId, trainEntry1, 0, 0 );
			Lcd_setCharacter( station[ id ].xLcdId, ( ( station[ id ].targetAddress / 1 ) % 10 ) + '0', 0, 6 );
			Lcd_setString( station[ id ].xLcdId, trainEntry2, 1, 0 );
			Lcd_setCharacter( station[ id ].xLcdId, ( ( station[ id ].buffer / 10) % 10) + '0', 1, 6 );
			Lcd_setCharacter( station[ id ].xLcdId, ( ( station[ id ].buffer / 1) % 10) + '0', 1, 7 );
		}else
		{
			station[ id ].buffer = 0;
		}
	}
}

void Station_detectTrainReach( STATION_ID_t id )
{
	if( Irsnsr_getState( station[ id ].pIrsnsrId ) == IRSNSR_STATE_HIGH )
	{
		station[ id ].buffer = 0;
		station[ id ].wait = 60000 / 100;
		station[ id ].state = 4;
		Lcd_setString( station[ id ].xLcdId, trainStop1, 0, 0 );
		Lcd_setCharacter( station[ id ].xLcdId, ( ( station[ id ].targetAddress / 1 ) % 10 ) + '0', 0, 6 );
		Lcd_setString( station[ id ].xLcdId, trainStop2, 1, 0 );
		Lcd_setCharacter( station[ id ].xLcdId, ( ( station[ id ].buffer / 10) % 10) + '0', 1, 6 );
		Lcd_setCharacter( station[ id ].xLcdId, ( ( station[ id ].buffer / 1) % 10) + '0', 1, 7 );
	}
}

void Station_serveTrain( STATION_ID_t id )
{
	if( station[ id ].wait )
	{
		station[ id ].wait--;
		if( Irsnsr_getState( station[ id ].wIrsnsrId ) == IRSNSR_STATE_HIGH )
		{
			//Emergency
			Lcd_clear( station[ id ].xLcdId );
			Lcd_setString( station[ id ].xLcdId, "Warning", 0, 0 );
			Lcd_setString( station[ id ].xLcdId, "Code:01", 1, 0 );
		}
		Lcd_setString( station[ id ].xLcdId, "XX", 0, 14 );
		Lcd_setCharacter( station[ id ].xLcdId, ( ( ( station[ id ].wait * 100 / 1000 ) / 10 ) % 10 ) + '0', 1, 14 );
		Lcd_setCharacter( station[ id ].xLcdId, ( ( ( station[ id ].wait * 100 / 1000 ) / 1 ) % 10 ) + '0', 1, 15 );
	}else
	{
		Lcd_setString( station[ id ].xLcdId, trainLeave1, 0, 0 );
		Lcd_setCharacter( station[ id ].xLcdId, ( ( station[ id ].targetAddress / 1 ) % 10 ) + '0', 0, 6 );
		Lcd_setString( station[ id ].xLcdId, trainLeave2, 1, 0 );
		Lcd_setCharacter( station[ id ].xLcdId, ( ( station[ id ].buffer / 10) % 10) + '0', 1, 6 );
		Lcd_setCharacter( station[ id ].xLcdId, ( ( station[ id ].buffer / 1) % 10) + '0', 1, 7 );
		Lcd_setCharacter( station[ id ].xLcdId, ( ( ( station[ id ].wait * 100 / 1000 ) / 10 ) % 10 ) + '0', 1, 14 );
		Lcd_setCharacter( station[ id ].xLcdId, ( ( ( station[ id ].wait * 100 / 1000 ) / 1 ) % 10 ) + '0', 1, 15 );
		//Communicate with next station
		station[ id ].buffer = station[ id ].targetAddress;
		station[ id ].targetAddress = station[ id ].nextAddress;
		station[ id ].state = 1;
	}
}

void Station_update( void *paramter )
{
	STATION_ID_t id = (STATION_ID_t) paramter;
	if( Irsnsr_getState( station[ id ].wIrsnsrId ) == IRSNSR_STATE_HIGH )
	{
		station[ id ].empty = 1;
		Lcd_clear( station[ id ].xLcdId );
	}

	switch( station[ id ].state )
	{
		case 0:
			Station_startState( id );
			break;
		case 1:
			Station_getTrainAddress( id );
			break;
		case 2:
			Station_detectTrainEnter( id );
			break;
		case 3:
			Station_detectTrainReach( id );
			break;
		case 4:
			Station_serveTrain( id );
			break;
	}
	Nrf_setTxBuffer( station[ id ].xNrfId, NRF_NODE_0, 0, station[ id ].targetAddress );
	Nrf_setTxBuffer( station[ id ].xNrfId, NRF_NODE_0, 1, station[ id ].currentAddress );
	Nrf_setTxBuffer( station[ id ].xNrfId, NRF_NODE_0, 2, station[ id ].buffer );
}
