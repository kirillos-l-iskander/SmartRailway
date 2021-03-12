#include "Infrared.h"

typedef struct
{
	Id_t gpio_id;
	uint8_t pin;
}Infrared_t;

static Infrared_t infrared[ INFRARED_NUMBER ];

void Infrared_init( Id_t id, Id_t gpio_id, uint8_t pin )
{
	infrared[ id ].gpio_id = gpio_id;
	infrared[ id ].pin = pin;
	Gpio_initPin( infrared[ id ].gpio_id, infrared[ id ].pin, INPUT );
}

uint8_t Infrared_getState( Id_t id )
{
	return ( uint8_t ) Gpio_getPinState( infrared[ id ].gpio_id, infrared[ id ].pin );
}
