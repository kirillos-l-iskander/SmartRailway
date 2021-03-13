#include "Encoder.h"

typedef struct
{
	Id_t xGpioId;
	uint8_t xPin;
	Id_t xTimerId;
}Encoder_t;

static Encoder_t encoder[ ENCODER_NUMBER ];

void Encoder_init( Id_t id, Id_t xGpioId, uint8_t xPin, Id_t xTimerId )
{
	encoder[ id ].xGpioId = xGpioId;
	encoder[ id ].xPin = xPin;
	encoder[ id ].xTimerId = xTimerId;
	Gpio_initPin( encoder[ id ].xGpioId, encoder[ id ].xPin, INPUT );
	Timer_initCounter( encoder[ id ].xTimerId );
}

UBaseType_t Encoder_getSpeed( Id_t id )
{
	return Timer_getCounterValue( encoder[ id ].xTimerId );
}

void Encoder_reset( Id_t id )
{
	Timer_setCounterValue( encoder[ id ].xTimerId, 0 );
}
