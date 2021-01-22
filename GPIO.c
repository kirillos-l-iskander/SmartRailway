#include "GPIO.h"

void GPIO_InitPin(GPIO_TypeDef* GPIOx, uint8_t PINx, uint8_t Mode)
{
	if(GPIOx == GPIOA)
	{
		//Enable clock for GPIOA
		RCC->APB2ENR = RCC->APB2ENR | RCC_APB2Periph_GPIOA;
	}else if(GPIOx == GPIOB)
	{
		//Enable clock for GPIOB
		RCC->APB2ENR = RCC->APB2ENR | RCC_APB2Periph_GPIOB;
	}else if(GPIOx == GPIOC)
	{
		//Enable clock for GPIOC
		RCC->APB2ENR = RCC->APB2ENR | RCC_APB2Periph_GPIOC;
	}
	if(PINx < 8)
	{
		GPIOx->CRL = (GPIOx->CRL & (unsigned)~(0xF<<(PINx*4))) | (unsigned)(Mode<<(PINx*4));
	}else
	{
		PINx = PINx - 8;
		GPIOx->CRH = (GPIOx->CRH & (unsigned)~(0xF<<(PINx*4))) | (unsigned)(Mode<<(PINx*4));	
	}
}

void GPIO_SetPinState(GPIO_TypeDef* GPIOx, uint8_t PINx, uint8_t State)
{
	GPIOx->ODR = (GPIOx->ODR & ~(1<<PINx)) | (unsigned)(State<<PINx);
}

uint8_t GPIO_GetPinState(GPIO_TypeDef* GPIOx, uint8_t PINx)
{
	return (uint8_t)((GPIOx->IDR & (1<<PINx))>>PINx);
}

void GPIO_SetPortState(GPIO_TypeDef* GPIOx, uint16_t PINx, uint16_t State)
{
	GPIOx->ODR = (GPIOx->ODR & ~PINx) | (State & PINx);
}

uint16_t GPIO_GetPortState(GPIO_TypeDef* GPIOx)
{
	return (uint16_t)GPIOx->IDR;
}
