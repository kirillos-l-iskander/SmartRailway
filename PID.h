#ifndef PID_H
#define PID_H

#include "SchedulerConfig.h"

typedef enum
{
	PID1_ID
}PID_t;

void Pid_init( Id_t id );
void Pid_setError( Id_t id, int32_t requiredState, int32_t currentState );
int32_t Pid_getCorrection( Id_t id );
void Pid_update( void *paramter );

#endif /* PID_H */
