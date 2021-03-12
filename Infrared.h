#ifndef INFRARED_H
#define	INFRARED_H

#include "SchedulerConfig.h"
#include "Gpio.h"

void Infrared_init( Id_t id, Id_t gpio_id, uint8_t pin );
uint8_t Infrared_getState( Id_t id );

#endif	/* INFRARED_H */
