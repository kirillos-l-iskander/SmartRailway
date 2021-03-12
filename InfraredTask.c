#include "InfraredTask.h"

typedef struct
{
	uint8_t state;
}InfraredTask_t;

static InfraredTask_t infraredTask[ INFRARED_TASK_NUMBER ];

void InfraredTask_init( Id_t id, Id_t gpio_id, uint8_t pin )
{
	infraredTask[ id ].state = LOW;
	Infrared_init( id, gpio_id, pin );
}

uint8_t InfraredTask_getState( Id_t id )
{
	return infraredTask[ id ].state;
}

void InfraredTask_update( void *paramter )
{
	Id_t id = (Id_t) paramter;
	infraredTask[ id ].state = !Infrared_getState( id );
}
