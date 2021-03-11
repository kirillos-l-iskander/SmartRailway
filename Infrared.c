#include "Infrared.h"

typedef struct
{
	Id_t idGpio;
	uint8_t pin;
}Infrared_t;

static Infrared_t infrared[ INFRARED_NUMBER ];

void Infrared_init( Id_t id )
{
	Gpio_initPin( infrared[ id ].idGpio, infrared[ id ].pin, INPUT );
}

uint8_t Infrared_getState( Id_t id )
{
	return ( uint8_t ) Gpio_getPinState( infrared[ id ].idGpio, infrared[ id ].pin );
}

void Infrared_setGpio( Id_t id, Id_t idGpio, uint8_t pin )
{
	infrared[ id ].idGpio = idGpio;
	infrared[ id ].pin = pin;
}
