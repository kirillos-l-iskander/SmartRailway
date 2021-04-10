#include "Irsnsr.h"

typedef struct
{
	GPIO_ID_t gpioId;
	GPIO_PIN_t gpioPin;
	IRSNSR_STATE_t state;
}	Irsnsr_t;

static Irsnsr_t irsnsr[ IRSNSR_ID_MAX ];

void initIrsnsrHardware( IRSNSR_ID_t id );
IRSNSR_STATE_t getIrsnsrHardwareState( IRSNSR_ID_t id );

void Irsnsr_init( IRSNSR_ID_t id, GPIO_ID_t gpioId, GPIO_PIN_t gpioPin )
{
	irsnsr[ id ].gpioId = gpioId;
	irsnsr[ id ].gpioPin = gpioPin;
	irsnsr[ id ].state = IRSNSR_STATE_LOW;
	initIrsnsrHardware( id );
}

void Irsnsr_update( void *paramter )
{
	IRSNSR_ID_t id = (IRSNSR_ID_t) paramter;
	irsnsr[ id ].state = getIrsnsrHardwareState( id );
}

IRSNSR_STATE_t Irsnsr_getState( IRSNSR_ID_t id )
{
	return irsnsr[ id ].state;
}

void initIrsnsrHardware( IRSNSR_ID_t id )
{
	Gpio_initPin( irsnsr[ id ].gpioId, irsnsr[ id ].gpioPin, GPIO_MODE_INPUT, GPIO_TYPE_FLOATING );
}

IRSNSR_STATE_t getIrsnsrHardwareState( IRSNSR_ID_t id )
{
	if( Gpio_getPinState( irsnsr[ id ].gpioId, irsnsr[ id ].gpioPin ) == GPIO_STATE_LOW )
	{
		return IRSNSR_STATE_HIGH;
	}else if( Gpio_getPinState( irsnsr[ id ].gpioId, irsnsr[ id ].gpioPin ) == GPIO_STATE_HIGH )
	{
		return IRSNSR_STATE_LOW;
	}else
	{
		return IRSNSR_STATE_ERROR;
	}
}
