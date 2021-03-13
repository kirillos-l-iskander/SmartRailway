#ifndef ENCODER_H
#define	ENCODER_H

#include "SchedulerConfig.h"
#include "Gpio.h"
#include "Timer.h"

typedef enum
{
	ENCODER1_ID
}ENCODER_t;

void Encoder_init( Id_t id, Id_t xGpioId, uint8_t xPin, Id_t cTimerId );
UBaseType_t Encoder_getSpeed( Id_t id );
void Encoder_reset( Id_t id );

#endif	/* ENCODER_H */
