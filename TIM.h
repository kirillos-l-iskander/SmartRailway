#ifndef TIM_H
#define	TIM_H

#include "CONFIG.h"
#include "IO.h"
#include "PORT.h"
#include "CORE.h"
#include "SCH_CONFIG.h"
#include "SCH.h"
#include "GPIO.h"
#include "AFIO.h"

//EGR
#define UG (1<<0)

//SMCR
#define TS_TI1 (5<<4)
#define SMS (7<<0)

//CCMR1
#define OC2M (6<<12)
#define OC2PE (1<<11)
#define CC2S_CH2TI2 (1<<8)
#define OC1M (6<<4)
#define IC1F (0xF<<4)
#define OC1PE (1<<3)
#define CC1S_CH1TI1 (1<<0)

//CCER
#define CC4E (1<<12)
#define CC3E (1<<8)
#define CC2E (1<<4)
#define CC1E (1<<0)

//CR1
#define ARPE (1<<7)
#define CEN (1<<0)

void TIM2_Init(void);
void TIM2_SetDutyCycle(uint16_t DutyCycle);
uint16_t TIM2_GetDutyCycle(void);

void TIM3_Init(void);
void TIM3_SetCounterValue(uint16_t Value);
uint16_t TIM3_GetCounterValue(void);

#endif	/* TIM_H */
