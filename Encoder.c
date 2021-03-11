#include "Encoder.h"

typedef struct
{
	Id_t gpio_id;
	uint8_t pin;
	Id_t timer_id;
}Encoder_t;

static Encoder_t encoder[ ENCODER_NUMBER ];

void Encoder_init( void )
{
	size_t id = 0;
	for ( id = 0; id < ENCODER_NUMBER; id++ )
	{
		Gpio_initPin( encoder[ id ].gpio_id, encoder[ id ].pin, INPUT );
		Timer_initCounter( encoder[ id ].timer_id );
	}
}

UBaseType_t Encoder_getSpeed( Id_t id )
{
	return Timer_getCounterValue( encoder[ id ].timer_id );
}

void Encoder_reset( Id_t id )
{
	Timer_setCounterValue( encoder[ id ].timer_id, 0 );
}

void Encoder_setGpio( Id_t id, Id_t gpio_id, uint8_t pin )
{
	encoder[ id ].gpio_id = gpio_id;
	encoder[ id ].pin = pin;
}

void Encoder_setTimer( Id_t id, Id_t timer_id )
{
	encoder[ id ].timer_id = timer_id;
}
