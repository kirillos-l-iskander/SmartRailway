#ifndef INFRARED_H
#define	INFRARED_H

#include "Config.h"
#include "Gpio.h"

typedef enum
{
	IRSNSR_ID_1 = 0,
	IRSNSR_ID_2,
	IRSNSR_ID_3,
	IRSNSR_ID_MAX,
}	IRSNSR_ID_t;

typedef enum
{
	IRSNSR_STATE_LOW = 0,
	IRSNSR_STATE_HIGH,
	IRSNSR_STATE_ERROR,
}	IRSNSR_STATE_t;

void Irsnsr_init( IRSNSR_ID_t id, GPIO_ID_t gpioId, GPIO_PIN_t gpioPin );
void Irsnsr_update( void *paramter );
IRSNSR_STATE_t Irsnsr_getState( IRSNSR_ID_t id );

#endif	/* INFRARED_H */
