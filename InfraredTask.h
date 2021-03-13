#ifndef INFRARED_TASK_H
#define	INFRARED_TASK_H

#include "SchedulerConfig.h"
#include "Infrared.h"

void InfraredTask_init( Id_t id, Id_t xGpioId, uint8_t xPin );
uint8_t InfraredTask_getState( Id_t id );
void InfraredTask_update( void *paramter );

#endif	/* INFRARED_TASK_H */
