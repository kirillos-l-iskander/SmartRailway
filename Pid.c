#include "Pid.h"

typedef struct
{
	int32_t error;
	int32_t previousError;
	int32_t kp;
	int32_t p;
	int32_t ki;
	int32_t i;
	int32_t kd;
	int32_t d;
	int32_t correction;
}	Pid_t;

static Pid_t pid[ PID_ID_MAX ];

void Pid_init( PID_ID_t id, int32_t kp, int32_t ki, int32_t kd )
{
	pid[ id ].error = 0;
	pid[ id ].previousError = 0;
	pid[ id ].kp = kp;
	pid[ id ].p = 0;
	pid[ id ].ki = ki;
	pid[ id ].i = 0;
	pid[ id ].kd = kd;
	pid[ id ].d = 0;
	pid[ id ].correction = 0;
}

void Pid_setError( PID_ID_t id, int32_t requiredState, int32_t currentState )
{
	pid[ id ].error = requiredState - currentState;
}

int32_t Pid_getCorrection( PID_ID_t id )
{
	return pid[ id ].correction;
}

void Pid_update( void *paramter )
{
	PID_ID_t id = (PID_ID_t) paramter;
	pid[ id ].p = pid[ id ].error;
	pid[ id ].i = pid[ id ].i + pid[ id ].error;
	pid[ id ].d = pid[ id ].error - pid[ id ].previousError;
	pid[ id ].previousError = pid[ id ].error;
	pid[ id ].correction += ( pid[ id ].kp * pid[ id ].p ) + ( pid[ id ].ki * pid[ id ].i ) + ( pid[ id ].kd * pid[ id ].d );
}
