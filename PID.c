#include "PID.h"

volatile static PID_t PID_G[1];

void PID_Init(void)
{
	PID_G[0].Error = 0;
	PID_G[0].PreviousError = 0;
	PID_G[0].Correction = 0;
	PID_G[0].P = 0;
	PID_G[0].I = 0;
	PID_G[0].D = 0;
	PID_G[0].Kp = 100;
	PID_G[0].Ki = 0;
	PID_G[0].Kd = 0;
}

void PID_Update(void)
{
	PID_G[0].P = PID_G[0].Error;
	PID_G[0].I = PID_G[0].I + PID_G[0].Error;
	PID_G[0].D = PID_G[0].Error - PID_G[0].PreviousError;
	PID_G[0].Correction = (PID_G[0].Kp * PID_G[0].P) + (PID_G[0].Ki * PID_G[0].I) + (PID_G[0].Kd * PID_G[0].D);
	PID_G[0].PreviousError = PID_G[0].Error;
}

void PID_SetError(int32_t RequiredState, int32_t CurrentState)
{
	PID_G[0].Error = RequiredState - CurrentState;
	if(PID_G[0].Error >= 0)
	{
		PID_G[0].Error = PID_G[0].Error * PID_G[0].Error;
	}else
	{
		PID_G[0].Error = PID_G[0].Error * PID_G[0].Error * -1;
	}
}

int32_t PID_GetCorrection(void)
{
	return PID_G[0].Correction;
}
