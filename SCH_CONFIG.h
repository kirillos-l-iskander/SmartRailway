#ifndef SCH_CONFIG_H
#define SCH_CONFIG_H

#include "CONFIG.h"
#include "IO.h"
#include "PORT.h"
#include "CORE.h"
#include "SCH_CONFIG.h"
#include "SCH.h"
#include "TIM.h"
#include "GPIO.h"
#include "LED.h"

uint8_t SysTick_Init(volatile const uint32_t TicksPeriod_ms);
void SysTick_Handler(void);
void HW_Delay_ms(volatile uint32_t Delay_ms);
void HW_Delay_us(volatile uint32_t Delay_us);
void SW_Delay_ms(volatile uint32_t Delay_ms);
void SW_Delay_us(volatile uint32_t Delay_us);

#endif /* SCH_CONFIG_H */
