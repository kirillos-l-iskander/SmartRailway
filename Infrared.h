#ifndef INFRARED_H
#define	INFRARED_H

#include "SchedulerConfig.h"
#include "Gpio.h"

typedef enum
{
	INFRARED1_ID,
	INFRARED2_ID,
	INFRARED3_ID
}INFRARED_t;

void Infrared_init( Id_t id, Id_t xGpioId, uint8_t xPin );
uint8_t Infrared_getState( Id_t id );

#endif	/* INFRARED_H */
