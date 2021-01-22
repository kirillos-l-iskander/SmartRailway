#include "TIM.h"

void TIM2_Init(void)
{
	AFIO_Init();
	GPIO_InitPin(GPIOA, 1, GPIO_Mode_AF_PP | GPIO_Speed_50MHz);
	RCC->APB1ENR |= RCC_APB1Periph_TIM2;
	TIM2->PSC = 72;
	TIM2->ARR = 65535;
	TIM2->CCR2 = 0;
	TIM2->CNT = 0;
	TIM2->EGR |= UG;
	TIM2->CCMR1 |= OC2M | OC2PE;
	TIM2->CCER |= CC2E;
	TIM2->CR1 |= ARPE | CEN;
}

void TIM2_SetDutyCycle(uint16_t DutyCycle)
{
	TIM2->CCR2 = DutyCycle;
}

uint16_t TIM2_GetDutyCycle(void)
{
	return TIM2->CCR2;
}

void TIM3_Init(void)
{
	AFIO_Init();
	GPIO_InitPin(GPIOA, 6, GPIO_Mode_IN_FLOATING | GPIO_Input);
	RCC->APB1ENR |= RCC_APB1Periph_TIM3;
	TIM3->ARR = 65535;
	TIM3->CNT = 0;
	TIM3->EGR |= UG;
	TIM3->SMCR |= TS_TI1 | SMS;
	TIM3->CCMR1 |= IC1F | CC1S_CH1TI1;
	TIM3->CCER |= CC1E;
	TIM3->CR1 |= ARPE | CEN;
}

void TIM3_SetCounterValue(uint16_t Value)
{
	TIM3->CNT = Value;
}

uint16_t TIM3_GetCounterValue(void)
{
	return TIM3->CNT;
}
