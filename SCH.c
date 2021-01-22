#include "SCH.h"

volatile static Task_t SCH_Tasks[SCH_MAX_TASKS];
volatile static uint8_t SCH_ErrorCode_G;
volatile static uint16_t SCH_ErrorTickCount_G;
volatile static uint8_t SCH_IntFlag_G;

void SCH_Init(void)
{
	uint8_t Index;
	for(Index = 0; Index < SCH_MAX_TASKS; Index++)
	{
		SCH_Delete_Task(Index);
	}
	SCH_ErrorCode_G = 0;
	SCH_ErrorTickCount_G = 0;
	SCH_IntFlag_G = 0;
}

void SCH_Start(void)
{
	SysTick_Init(SCH_TICK_PERIOD_MS);
}

uint8_t SCH_Add_Task(void(*Task_P)(void), volatile const uint16_t Delay, volatile const uint16_t Period)
{
	uint8_t Index;
	for(Index = 0; Index < SCH_MAX_TASKS; Index++)
	{
		if(SCH_Tasks[Index].Task_P == 0)
		{
			break;
		}
	}
	if(Index < SCH_MAX_TASKS)
	{
		SCH_Tasks[Index].Task_P = Task_P;
		SCH_Tasks[Index].Delay = Delay;
		SCH_Tasks[Index].Period = Period;
		SCH_Tasks[Index].RunMe = 0;
		return Index;
	}else
	{
		SCH_ErrorCode_G = ERROR_SCH_TOO_MANY_TASKS;
		return SCH_MAX_TASKS;
	}
}

uint8_t SCH_Delete_Task(volatile const uint8_t Index)
{
	if(SCH_Tasks[Index].Task_P != 0)
	{
		SCH_Tasks[Index].Task_P = 0x0000;
		SCH_Tasks[Index].Delay = 0;
		SCH_Tasks[Index].Period = 0;
		SCH_Tasks[Index].RunMe = 0;
		return RETURN_NORMAL;
	}else
	{
		SCH_ErrorCode_G = ERROR_SCH_CANNOT_DELETE_TASK;
		return RETURN_ERROR;
	}
}

void SCH_Report_Status(void)
{
	if(SCH_ErrorTickCount_G)
	{
		SCH_ErrorTickCount_G--;
	}else if(SCH_ErrorCode_G)
	{
		SCH_ErrorTickCount_G = 5000;
		LED_SetState(LED0, LOW);
	}else
	{
		LED_SetState(LED0, HIGH);
	}
	if(SCH_IntFlag_G)
	{
		SCH_ErrorCode_G = 1;
		SCH_IntFlag_G = 0;
	}else
	{
		SCH_ErrorCode_G = 0;
	}
	while(SCH_IntFlag_G == 0);
	SCH_IntFlag_G = 0;
}

void SCH_Go_To_Sleep(void)
{

}

void SCH_Update(void)
{
	uint8_t Index;
	for(Index = 0; Index < SCH_MAX_TASKS; Index++)
	{
		if(SCH_Tasks[Index].Task_P != 0)
		{
			if(SCH_Tasks[Index].Delay == 0)
			{
				SCH_Tasks[Index].RunMe = 1;
				SCH_Tasks[Index].Delay = SCH_Tasks[Index].Period;
			}
			if(SCH_Tasks[Index].Delay != 0)
			{
				SCH_Tasks[Index].Delay--;
			}
		}
	}
	SCH_IntFlag_G = 1;
}

void SCH_Dispatch_Tasks(void)
{
	uint8_t Index;
	for(Index = 0; Index < SCH_MAX_TASKS; Index++)
	{
		if(SCH_Tasks[Index].RunMe != 0)
		{
			(*SCH_Tasks[Index].Task_P)();
			SCH_Tasks[Index].RunMe = 0;
			if(SCH_Tasks[Index].Period == 0)
			{
				SCH_Delete_Task(Index);
			}
		}
	}
	SCH_Report_Status();
	SCH_Go_To_Sleep();
}
