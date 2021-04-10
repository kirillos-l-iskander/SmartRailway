#ifndef PID_H
#define PID_H

#include "Config.h"

typedef enum
{
	PID_ID_1 = 0,
	PID_ID_MAX,
}	PID_ID_t;

void Pid_init( PID_ID_t id, int32_t kp, int32_t ki, int32_t kd );
void Pid_setError( PID_ID_t id, int32_t requiredState, int32_t currentState );
int32_t Pid_getCorrection( PID_ID_t id );
void Pid_update( void *paramter );

#endif /* PID_H */
