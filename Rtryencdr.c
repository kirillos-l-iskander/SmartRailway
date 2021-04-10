#include "Rtryencdr.h"

typedef struct
{
	GPIO_ID_t xGpioId;
	GPIO_PIN_t xPin;
	TIMER_ID_t xTimerId;
	TIMER_CHANNEL_t channel;
	RTRYENCDR_COUNTER_t counter;
}	Rtryencdr_t;

static Rtryencdr_t rtryencdr[ RTRYENCDR_ID_MAX ];

void initRtryencdrHardware( RTRYENCDR_ID_t id );
RTRYENCDR_COUNTER_t getRtryencdrHardwareSpeed( RTRYENCDR_ID_t id );
void resetRtryencdrHardware( RTRYENCDR_ID_t id );

void Rtryencdr_init( RTRYENCDR_ID_t id, GPIO_ID_t xGpioId, GPIO_PIN_t xPin, TIMER_ID_t xTimerId, TIMER_CHANNEL_t channel )
{
	rtryencdr[ id ].xGpioId = xGpioId;
	rtryencdr[ id ].xPin = xPin;
	rtryencdr[ id ].xTimerId = xTimerId;
	rtryencdr[ id ].channel = channel;
	rtryencdr[ id ].counter = 0;
	initRtryencdrHardware( id );
}

void Rtryencdr_update( void *paramter )
{
	RTRYENCDR_ID_t id = (RTRYENCDR_ID_t)paramter;
	rtryencdr[ id ].counter = getRtryencdrHardwareSpeed( id );
	resetRtryencdrHardware( id );
}

RTRYENCDR_COUNTER_t Rtryencdr_getSpeed( RTRYENCDR_ID_t id )
{
	return rtryencdr[ id ].counter;
}

void initRtryencdrHardware( RTRYENCDR_ID_t id )
{
	Gpio_initPin( rtryencdr[ id ].xGpioId, rtryencdr[ id ].xPin, GPIO_MODE_INPUT, GPIO_TYPE_FLOATING );
	Timer_init( rtryencdr[ id ].xTimerId, TIMER_MODE_NORMAL, TIMER_PERIOD_MAX, rtryencdr[ id ].channel, TIMER_EVENT_CLOCKIN_RISING );
}

RTRYENCDR_COUNTER_t getRtryencdrHardwareSpeed( RTRYENCDR_ID_t id )
{
	return Timer_getCounter( rtryencdr[ id ].xTimerId );
}

void resetRtryencdrHardware( RTRYENCDR_ID_t id )
{
	Timer_setCounter( rtryencdr[ id ].xTimerId, 0 );
}
