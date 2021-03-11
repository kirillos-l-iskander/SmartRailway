#include "NrfTask.h"

typedef struct
{
	uint8_t freq;
	uint8_t nodeNumber;
	uint8_t state;
	uint8_t index;
	uint8_t safty;
	uint8_t address[ NODES_NUMBER + 1 ][ ADDR_WIDTH ];
	uint8_t txBuffer[ NODES_NUMBER + 1 ][ PAYLOAD_SIZE ];
	uint8_t rxBuffer[ NODES_NUMBER + 1 ][ PAYLOAD_SIZE ];
}NrfTaskMaster_t;

typedef struct
{
	uint8_t freq;
	uint8_t nodeNumber;
	uint8_t index;
	uint8_t state;
	uint8_t safty;
	uint8_t address[ ADDR_WIDTH ];
	uint8_t txBuffer[ PAYLOAD_SIZE ];
	uint8_t rxBuffer[ PAYLOAD_SIZE ];
}NrfTaskSlave_t;

static NrfTaskMaster_t nrfTaskMaster[ NRF_TASK_NUMBER ];
static NrfTaskSlave_t nrfTaskSlave[ NRF_TASK_NUMBER ];

void NrfTaskMaster_init( void )
{
	size_t id = 0;
	Nrf_init();
	for ( id = 0; id < NRF_TASK_NUMBER; id++ )
	{
		nrfTaskMaster[ id ].freq = 0;
		nrfTaskMaster[ id ].state = 0;
		nrfTaskMaster[ id ].index = 0;
		nrfTaskMaster[ id ].safty = 0;
		for( size_t i = 0; i < ( NODES_NUMBER + 1 ); i++ )
		{
			for( size_t j = 0; j < ( ADDR_WIDTH - 1 ); j++ )
			{
				nrfTaskMaster[ id ].address[ i ][ j ] = 0xF0;
			}
		}
		nrfTaskMaster[ id ].address[ 0 ][ 4 ] = 0x00;
		nrfTaskMaster[ id ].address[ 1 ][ 4 ] = 0x01;
		nrfTaskMaster[ id ].address[ 2 ][ 4 ] = 0x02;
		nrfTaskMaster[ id ].address[ 3 ][ 4 ] = 0x03;
		nrfTaskMaster[ id ].address[ 4 ][ 4 ] = 0x04;
		NrfTaskMaster_clearTxBuffer( (Id_t) id );
		NrfTaskMaster_clearRxBuffer( (Id_t) id );

		Nrf_openWritingPipe( (Id_t) id, nrfTaskMaster[ id ].address[ nrfTaskMaster[ id ].nodeNumber ] );	
		Nrf_startTxMode( (Id_t) id );
	}
}

void NrfTaskMaster_setNodeNumber( Id_t id, uint8_t nodeNumber )
{
	nrfTaskMaster[ id ].nodeNumber = nodeNumber;
}

void NrfTaskMaster_clearTxBuffer( Id_t id )
{
	for( size_t i = 0; i < ( NODES_NUMBER + 1 ); i++ )
	{
		for( size_t j = 0; j < PAYLOAD_SIZE; j++ )
		{
			nrfTaskMaster[ id ].txBuffer[ i ][ j ] = 0;
		}
	}
}

void NrfTaskMaster_clearRxBuffer( Id_t id )
{
	for( size_t i = 0; i < ( NODES_NUMBER + 1 ); i++ )
	{
		for( size_t j = 0; j < PAYLOAD_SIZE; j++ )
		{
			nrfTaskMaster[ id ].rxBuffer[ i ][ j ] = 0;
		}
	}
}

void NrfTaskMaster_update( void *paramter )
{
	size_t id = 0;
	for ( id = 0; id < NRF_TASK_NUMBER; id++ )
	{
		switch( nrfTaskMaster[ id ].state )
		{
			case 0:
				nrfTaskMaster[ id ].index++;
				Nrf_openWritingPipe( (Id_t) id, nrfTaskMaster[ id ].address[ nrfTaskMaster[ id ].index ] );
				Nrf_startTxMode( (Id_t) id );
				nrfTaskMaster[ id ].state = 1;
				break;
			case 1:
				Nrf_transmit( (Id_t) id, nrfTaskMaster[ id ].txBuffer[ nrfTaskMaster[ id ].index ] );
				nrfTaskMaster[ (Id_t) id ].state = 2;
				break;
			case 2:
				if( Nrf_checkTxFlag( (Id_t) id ) )
				{
					Nrf_startRxMode( (Id_t) id );
					nrfTaskMaster[ id ].state = 3;
				}
				break;
			case 3:
				nrfTaskMaster[ id ].safty++;
				if( Nrf_checkRxFlag( (Id_t) id ) )
				{
					Nrf_receive( (Id_t) id, nrfTaskMaster[ id ].rxBuffer[ nrfTaskMaster[ id ].index ] );
					Nrf_stopRxMode( (Id_t) id );
					nrfTaskMaster[ id ].safty = 0;
					if( nrfTaskMaster[ id ].index == NODES_NUMBER )
					{
						nrfTaskMaster[ id ].index = 0;
						nrfTaskMaster[ id ].state = 4;
					}else
					{
						nrfTaskMaster[ id ].state = 0;
					}
				}else if( nrfTaskMaster[ id ].safty == 15 )
				{
					Nrf_stopRxMode( (Id_t) id );
					nrfTaskMaster[ id ].safty = 0;
					if( nrfTaskMaster[ id ].index == NODES_NUMBER )
					{
						nrfTaskMaster[ id ].index = 0;
						nrfTaskMaster[ id ].state = 4;
					}else
					{
						nrfTaskMaster[ id ].state = 0;
					}
				}
				break;
			case 4:
				for( size_t i = 0; i < ( NODES_NUMBER + 1 ); i++ )
				{
					nrfTaskMaster[ id ].txBuffer[ nrfTaskMaster[ id ].rxBuffer[ i ][ 0 ] ][ 0 ] = nrfTaskMaster[ id ].rxBuffer[ i ][ 0 ];
					nrfTaskMaster[ id ].txBuffer[ nrfTaskMaster[ id ].rxBuffer[ i ][ 0 ] ][ 1 ] = nrfTaskMaster[ id ].rxBuffer[ i ][ 1 ];
					nrfTaskMaster[ id ].txBuffer[ nrfTaskMaster[ id ].rxBuffer[ i ][ 0 ] ][ 2 ] = nrfTaskMaster[ id ].rxBuffer[ i ][ 2 ];
					nrfTaskMaster[ id ].txBuffer[ nrfTaskMaster[ id ].rxBuffer[ i ][ 0 ] ][ 3 ] = nrfTaskMaster[ id ].rxBuffer[ i ][ 3 ];
					nrfTaskMaster[ id ].txBuffer[ nrfTaskMaster[ id ].rxBuffer[ i ][ 0 ] ][ 4 ] = nrfTaskMaster[ id ].rxBuffer[ i ][ 4 ];
				}
				nrfTaskMaster[ id ].state = 0;

				nrfTaskMaster[ id ].freq ^= 1;
				if( nrfTaskMaster[ id ].freq )
				{
					LedTask_setState( (Id_t) id, HIGH, 0 );
				}else
				{
					LedTask_setState( (Id_t) id, LOW, 0 );
				}
				break;
		}
	}
}

void NrfTaskSlave_init( void )
{
	size_t id = 0;
	Nrf_init();
	for ( id = 0; id < NRF_TASK_NUMBER; id++ )
	{
		nrfTaskSlave[ id ].state = 0;
		nrfTaskSlave[ id ].safty = 0;
		for( size_t i = 0; i < ( ADDR_WIDTH - 1 ); i++ )
		{
			nrfTaskSlave[ id ].address[ i ] = 0xF0;
		}
		nrfTaskSlave[ id ].address[ 4 ] = nrfTaskSlave[ id ].nodeNumber;
		NrfTaskSlave_clearTxBuffer( (Id_t) id );
		NrfTaskSlave_clearRxBuffer( (Id_t) id );
		Nrf_openWritingPipe( (Id_t) id, nrfTaskSlave[ id ].address );
	}
}

void NrfTaskSlave_setNodeNumber( Id_t id, uint8_t nodeNumber )
{
	nrfTaskSlave[ id ].nodeNumber = nodeNumber;
}

void NrfTaskSlave_setBuffer( Id_t id, uint8_t index, uint8_t buffer )
{
	nrfTaskSlave[ id ].txBuffer[ index ] = buffer;
}

uint8_t NrfTaskSlave_getBuffer( Id_t id, uint8_t index )
{
	return nrfTaskSlave[ id ].rxBuffer[ index ];
}

void NrfTaskSlave_clearTxBuffer( Id_t id )
{
	for( size_t i = 0; i < PAYLOAD_SIZE; i++ )
	{
		nrfTaskSlave[ id ].txBuffer[ i ] = 0;
	}
}

void NrfTaskSlave_clearRxBuffer( Id_t id )
{
	for( size_t i = 0; i < PAYLOAD_SIZE; i++ )
	{
		nrfTaskSlave[ id ].rxBuffer[ i ] = 0;
	}
}

void NrfTaskSlave_update( void *paramter )
{
	size_t id = 0;
	for ( id = 0; id < NRF_TASK_NUMBER; id++ )
	{
		switch( nrfTaskSlave[ id ].state )
		{
			case 0:
			{
				Nrf_startRxMode( (Id_t) id );
				nrfTaskSlave[ id ].state = 1;
				break;
			}
			case 1:
			{
				nrfTaskSlave[ id ].safty++;
				if( Nrf_checkRxFlag( (Id_t) id ) )
				{
					Nrf_receive( (Id_t) id, nrfTaskSlave[ id ].rxBuffer );
					Nrf_stopRxMode( (Id_t) id );
					Nrf_startTxMode( (Id_t) id );
					nrfTaskSlave[ id ].safty = 0;
					nrfTaskSlave[ id ].state = 2;
				}else if( nrfTaskSlave[ id ].safty == ( C_MS_TO_TICKS( 1000 ) / C_MS_TO_TICKS( 5 ) ) )
				{
					NrfTaskSlave_clearTxBuffer( (Id_t) id );
					NrfTaskSlave_clearRxBuffer( (Id_t) id );
					Nrf_init();
					nrfTaskSlave[ id ].safty = 0;
					nrfTaskSlave[ id ].state = 4;
				}
				break;
			}
				
			case 2:
			{
				Nrf_transmit( (Id_t) id, nrfTaskSlave[ id ].txBuffer );
				nrfTaskSlave[ id ].state = 3;
				break;
			}
			
			case 3:
			{
				if( Nrf_checkTxFlag( (Id_t) id ) )
				{
					Nrf_startRxMode( (Id_t) id );
					nrfTaskSlave[ id ].state = 1;
				}
				break;
			}
				
			case 4:
			{
				//NrfTaskSlave_init( (Id_t) Id );
				Nrf_openWritingPipe( (Id_t) id, nrfTaskSlave[ id ].address );
				Nrf_startRxMode( (Id_t) id );
				nrfTaskSlave[ id ].state = 1;
				break;
			}

		}
	}
}
