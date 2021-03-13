#include "Infrared.h"

typedef struct
{
	Id_t xGpioId;
	uint8_t xPin;
}Infrared_t;

static Infrared_t infrared[ INFRARED_NUMBER ];

void Infrared_init( Id_t id, Id_t xGpioId, uint8_t xPin )
{
	infrared[ id ].xGpioId = xGpioId;
	infrared[ id ].xPin = xPin;
	Gpio_initPin( infrared[ id ].xGpioId, infrared[ id ].xPin, INPUT );
}

uint8_t Infrared_getState( Id_t id )
{
	return ( uint8_t ) Gpio_getPinState( infrared[ id ].xGpioId, infrared[ id ].xPin );
}
