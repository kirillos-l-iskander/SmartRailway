#ifndef PID_H
#define PID_H

#include "CONFIG.h"
#include "IO.h"
#include "PORT.h"
#include "CORE.h"
#include "SCH_CONFIG.h"
#include "SCH.h"
#include "GPIO.h"
#include "AFIO.h"

typedef struct
{
	int32_t Error;
	int32_t PreviousError;
	int32_t Correction;
	int32_t P;
	int32_t I;
	int32_t D;
	int32_t Kp;
	int32_t Ki;
	int32_t Kd;
}PID_t;

void PID_Init(void);
void PID_Update(void);
void PID_SetError(int32_t RequiredState, int32_t CurrentState);
int32_t PID_GetCorrection(void);

#endif /* PID_H */
