#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "SchedulerConfig.h"

void Scheduler_init( void );
UBaseType_t Scheduler_addTask( void (*task)( void *paramter ), void *paramter, UBaseType_t delay, UBaseType_t period );
UBaseType_t Scheduler_deleteTask( UBaseType_t index );
void Scheduler_start( void );
void Scheduler_update( void );
void Scheduler_dispatchTasks( void );
void Scheduler_reportStatus( void );
void Scheduler_goToSleep( void );

#endif /* SCHEDULER_H */
