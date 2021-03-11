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
}Pid_t;

static Pid_t pid[ PID_NUMBER ];

void Pid_init( Id_t id )
{
	pid[ id ].error = 0;
	pid[ id ].previousError = 0;
	pid[ id ].kp = 1;
	pid[ id ].p = 0;
	pid[ id ].ki = 0;
	pid[ id ].i = 0;
	pid[ id ].kd = 0;
	pid[ id ].d = 0;
	pid[ id ].correction = 0;
}

void Pid_setError( Id_t id, int32_t requiredState, int32_t currentState )
{
	pid[ id ].error = requiredState - currentState;
}

int32_t Pid_getCorrection( Id_t id )
{
	return pid[ id ].correction;
}

void Pid_update( void *paramter )
{
	Id_t id = (Id_t) paramter;
	pid[ id ].p = pid[ id ].error;
	pid[ id ].i = pid[ id ].i + pid[ id ].error;
	pid[ id ].d = pid[ id ].error - pid[ id ].previousError;
	pid[ id ].correction = ( pid[ id ].kp * pid[ id ].p ) + ( pid[ id ].ki * pid[ id ].i ) + ( pid[ id ].kd * pid[ id ].d );
	pid[ id ].previousError = pid[ id ].error;
}
