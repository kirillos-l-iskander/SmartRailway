#ifndef INFRARED_H
#define	INFRARED_H

#include "SchedulerConfig.h"
#include "Gpio.h"

void Infrared_init( Id_t id );
uint8_t Infrared_getState( Id_t id );
void Infrared_setGpio( Id_t id, Id_t idGpio, uint8_t pin );

#endif	/* INFRARED_H */
