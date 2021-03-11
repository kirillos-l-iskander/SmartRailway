#include "InfraredTask.h"

typedef struct
{
	uint8_t state;
}InfraredTask_t;

static InfraredTask_t infraredTask[ INFRARED_TASK_NUMBER ];

void InfraredTask_init( void )
{
	size_t id = 0;
	for ( id = 0; id < INFRARED_TASK_NUMBER; id++ )
	{
		infraredTask[ id ].state = LOW;
	}
	Infrared_init();
}

uint8_t InfraredTask_getState( Id_t id )
{
	return infraredTask[ id ].state;
}

void InfraredTask_update( void *paramter )
{
	size_t id = 0;
	for ( id = 0; id < INFRARED_TASK_NUMBER; id++ )
	{
		infraredTask[ id ].state = !Infrared_getState( (Id_t) id );
	}
}
