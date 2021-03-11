#ifndef STATION_TASK_H
#define	STATION_TASK_H

#include "SchedulerConfig.h"
#include "NrfTask.h"
#include "InfraredTask.h"
#include "LcdTask.h"

void StationTask_init( void );
void StationTask_setNodeNumber( Id_t id, uint8_t nodeNumber );
void StationTask_startState( Id_t id );
void StationTask_getTrainAddress( Id_t id );
void StationTask_detectTrainEnter( Id_t id );
void StationTask_detectTrainReach( Id_t id );
void StationTask_serveTrain( Id_t id );
void StationTask_update( void *paramter );

#endif	/* STATION_TASK_H */
