#include "SCH_CONFIG.h"

uint8_t SysTick_Init(volatile const uint32_t TicksPeriod_ms)
{
	volatile uint32_t Ticks_N = (((CPU_F/CPU_INSTRUCTION)/1000)*TicksPeriod_ms)-1;
	if(Ticks_N < SYSTICK_MAX)
	{
		//NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS)-1);
		//NVIC_EnableIRQ(SysTick_IRQn);
		SysTick->CTRL = 0;
		SysTick->VAL = 0;
		SysTick->LOAD = Ticks_N;
		SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
		return RETURN_NORMAL;
	}else
	{
		return RETURN_ERROR;
	}
}

void SysTick_Handler(void)
{
	SCH_Update();
}

void HW_Delay_ms(volatile uint32_t Delay_ms)
{
	
}

void HW_Delay_us(volatile uint32_t Delay_us)
{
	
}

void SW_Delay_ms(volatile uint32_t Delay_ms)
{
	while(Delay_ms-- > 0)
	{
		SW_Delay_us(1000);
	}
}

void SW_Delay_us(volatile uint32_t Delay_us)
{
	volatile uint8_t Delay;
	while(Delay_us-- > 0)
	{
		Delay = 255;
		while(Delay--);
	}
}
