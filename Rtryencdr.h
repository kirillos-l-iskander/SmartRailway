#ifndef ENCODER_H
#define	ENCODER_H

#include "Config.h"
#include "Gpio.h"
#include "Timer.h"

typedef enum
{
	RTRYENCDR_ID_1 = 0,
	RTRYENCDR_ID_MAX,
}	RTRYENCDR_ID_t;

typedef TIMER_COUNTER_t RTRYENCDR_COUNTER_t;

void Rtryencdr_init( RTRYENCDR_ID_t id, GPIO_ID_t xGpioId, GPIO_PIN_t xPin, TIMER_ID_t xTimerId, TIMER_CHANNEL_t channel );
void Rtryencdr_update( void *paramter );
RTRYENCDR_COUNTER_t Rtryencdr_getSpeed( RTRYENCDR_ID_t id );

#endif	/* ENCODER_H */
