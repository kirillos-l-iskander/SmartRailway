#include "TR.h"

void TR_Update(void)
{
	int32_t CorrectionSpeed = 0;
	uint16_t RequiredSpeed = NRF_GetData(2);
	uint16_t CurrentSpeed = TIM3_GetCounterValue();
	
	RequiredSpeed = (uint8_t)((((float)RequiredSpeed/6)*32)/10);
	
	PID_SetError(RequiredSpeed, CurrentSpeed);
	PID_Update();
	CorrectionSpeed = (int32_t)TIM2_GetDutyCycle() + PID_GetCorrection();
	
	if(RequiredSpeed == 0 && CurrentSpeed == 0)
	{
		TIM2_SetDutyCycle((uint16_t)0);
	}else if((RequiredSpeed == 0) && (CurrentSpeed > 0))
	{
		TIM2_SetDutyCycle((uint16_t)0);
	}else if((RequiredSpeed > 0) && (CurrentSpeed == 0))
	{
		TIM2_SetDutyCycle((uint16_t)50000);
	}else if((CorrectionSpeed >= 0) && (CorrectionSpeed <= 65535))
	{
		TIM2_SetDutyCycle((uint16_t)CorrectionSpeed);
	}
	
	TIM3_SetCounterValue(0);
}
