#ifndef SCH_H
#define SCH_H

#include "CONFIG.h"
#include "IO.h"
#include "PORT.h"
#include "CORE.h"
#include "SCH_CONFIG.h"
#include "SCH.h"
#include "TIM.h"
#include "GPIO.h"
#include "LED.h"

typedef struct
{
	void (*Task_P)(void);
	uint32_t Delay;
	uint32_t Period;
	uint32_t RunMe;
}Task_t;

void SCH_Init(void);
void SCH_Start(void);
uint8_t SCH_Add_Task(void (*Task_P)(void), volatile const uint16_t Delay, volatile const uint16_t Period);
uint8_t SCH_Delete_Task(volatile const uint8_t Index);
void SCH_Report_Status(void);
void SCH_Go_To_Sleep(void);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);

#endif /* SCH_H */
