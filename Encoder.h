#ifndef ENCODER_H
#define	ENCODER_H

#include "SchedulerConfig.h"
#include "Gpio.h"
#include "Timer.h"

void Encoder_init( void );
UBaseType_t Encoder_getSpeed( Id_t id );
void Encoder_reset( Id_t id );
void Encoder_setGpio( Id_t id, Id_t gpio_id, uint8_t pin );
void Encoder_setTimer( Id_t id, Id_t timer_id );

#endif	/* ENCODER_H */
