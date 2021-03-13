#include "Station.h"

typedef struct
{
	uint8_t state;
	uint8_t currentAddress;
	uint8_t nextAddress;
	uint8_t targetAddress;
	uint8_t buffer;
	uint8_t empty;
	uint16_t wait;
}StationTask_t;

static StationTask_t stationTask[ STATION_NUMBER ];

void StationTask_init( Id_t id )
{
	stationTask[ id ].state = 0;
	stationTask[ id ].empty = 1;
	stationTask[ id ].wait = 0;
}

void StationTask_setNodeNumber( Id_t id, uint8_t nodeNumber )
{
	switch( nodeNumber )
	{
		case 1:
			stationTask[ id ].currentAddress = 1;
			stationTask[ id ].nextAddress = 2;	
			stationTask[ id ].targetAddress = 3;
			stationTask[ id ].buffer = 15;
			break;
		case 2:
			stationTask[ id ].currentAddress = 2;
			stationTask[ id ].nextAddress = 1;
			stationTask[ id ].targetAddress = 4;
			stationTask[ id ].buffer = 15;
			break;
	}
}

void StationTask_startState( Id_t id )
{
	NrfTaskSlave_setBuffer( id, 0, stationTask[ id ].targetAddress );
	NrfTaskSlave_setBuffer( id, 1, stationTask[ id ].currentAddress );
	NrfTaskSlave_setBuffer( id, 2, stationTask[ id ].buffer );
	stationTask[ id ].state = 2;
}

void StationTask_getTrainAddress( Id_t id )
{
	if( NrfTaskSlave_getBuffer( id, 0 ) && ( NrfTaskSlave_getBuffer( id, 2 ) != stationTask[ id ].buffer ) )
	{
		stationTask[ id ].state = 2;
		stationTask[ id ].targetAddress = NrfTaskSlave_getBuffer( id, 2 );
		stationTask[ id ].buffer = 15;
		NrfTaskSlave_clearRxBuffer( id );
	}
}

void StationTask_detectTrainEnter( Id_t id )
{
	if( InfraredTask_getState( id*3 ) )
	{
		if( stationTask[ id ].empty )
		{
			stationTask[ id ].empty = 0;
			stationTask[ id ].buffer = 10;
			stationTask[ id ].state = 3;
			LcdTask_setString( id, (uint8_t*) "Train:", 0, 0 );
			LcdTask_setCharacter( id, ( ( stationTask[ id ].targetAddress / 1 ) % 10 ) + '0', 0, 6 );
			LcdTask_setString( id, (uint8_t*) "<<", 0, 14 );
			LcdTask_setString( id, (uint8_t*) "Speed:", 1, 0 );
			LcdTask_setCharacter( id, ( ( stationTask[ id ].buffer / 10) % 10) + '0', 1, 6 );
			LcdTask_setCharacter( id, ( ( stationTask[ id ].buffer / 1) % 10) + '0', 1, 7 );
			LcdTask_setString( id, (uint8_t*) "cm/s", 1, 8 );
		}else
		{
			stationTask[ id ].buffer = 0;
		}
	}
}

void StationTask_detectTrainReach( Id_t id )
{
	if( InfraredTask_getState( id*3 + 1 ) )
	{
		stationTask[ id ].buffer = 0;
		stationTask[ id ].wait = 60000 / 100;
		stationTask[ id ].state = 4;
		LcdTask_setString( id, (uint8_t*) "Train:", 0, 0 );
		LcdTask_setCharacter( id, ( ( stationTask[ id ].targetAddress / 1 ) % 10 ) + '0', 0, 6 );
		LcdTask_setString( id, (uint8_t*) "XX", 0, 14 );
		LcdTask_setString( id, (uint8_t*) "Speed:", 1, 0 );
		LcdTask_setCharacter( id, ( ( stationTask[ id ].buffer / 10) % 10) + '0', 1, 6 );
		LcdTask_setCharacter( id, ( ( stationTask[ id ].buffer / 1) % 10) + '0', 1, 7 );
		LcdTask_setString( id, (uint8_t*) "cm/s", 1, 8 );
	}
}

void StationTask_serveTrain( Id_t id )
{
	if( stationTask[ id ].wait )
	{
		stationTask[ id ].wait--;
		if( InfraredTask_getState( id*3 + 2 ) )
		{
			//Emergency
			LcdTask_clear( id );
			LcdTask_setString( id, (uint8_t*) "Warning", 0, 0 );
			LcdTask_setString( id, (uint8_t*) "Code:01", 1, 0 );
		}
		LcdTask_setString( id, (uint8_t*) "XX", 0, 14 );
		LcdTask_setCharacter( id, ( ( ( stationTask[ id ].wait * 100 / 1000 ) / 10 ) % 10 ) + '0', 1, 14 );
		LcdTask_setCharacter( id, ( ( ( stationTask[ id ].wait * 100 / 1000 ) / 1 ) % 10 ) + '0', 1, 15 );
	}else
	{
		LcdTask_setString( id, (uint8_t*) "Train:", 0, 0 );
		LcdTask_setCharacter( id, ( ( stationTask[ id ].targetAddress / 1 ) % 10 ) + '0', 0, 6 );
		LcdTask_setString( id, (uint8_t*) "<-", 0, 14 );
		LcdTask_setString( id, (uint8_t*) "Speed:", 1, 0 );
		LcdTask_setCharacter( id, ( ( stationTask[ id ].buffer / 10) % 10) + '0', 1, 6 );
		LcdTask_setCharacter( id, ( ( stationTask[ id ].buffer / 1) % 10) + '0', 1, 7 );
		LcdTask_setCharacter( id, ( ( ( stationTask[ id ].wait * 100 / 1000 ) / 10 ) % 10 ) + '0', 1, 14 );
		LcdTask_setCharacter( id, ( ( ( stationTask[ id ].wait * 100 / 1000 ) / 1 ) % 10 ) + '0', 1, 15 );
		//Communicate with next station
		stationTask[ id ].buffer = stationTask[ id ].targetAddress;
		stationTask[ id ].targetAddress = stationTask[ id ].nextAddress;
		stationTask[ id ].state = 1;
	}
}

void StationTask_update( void *paramter )
{
	Id_t id = (Id_t) paramter;
	if( InfraredTask_getState( ( id*3 + 2 ) ) )
	{
		stationTask[ id ].empty = 1;
		LcdTask_clear( id );
	}

	switch( stationTask[ id ].state )
	{
		case 0:
			StationTask_startState( id );
			break;
		case 1:
			StationTask_getTrainAddress( id );
			break;
		case 2:
			StationTask_detectTrainEnter( id );
			break;
		case 3:
			StationTask_detectTrainReach( id );
			break;
		case 4:
			StationTask_serveTrain( id );
			break;
	}
	NrfTaskSlave_setBuffer( id, 0, stationTask[ id ].targetAddress );
	NrfTaskSlave_setBuffer( id, 1, stationTask[ id ].currentAddress );
	NrfTaskSlave_setBuffer( id, 2, stationTask[ id ].buffer );
}
