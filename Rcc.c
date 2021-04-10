#include "Rcc.h"

typedef RCC_TypeDef Rcc_t;

static Rcc_t *rcc[ RCC_ID_MAX ] = { RCC };

void Rcc_enableClock( RCC_ID_t id, void *paramter )
{
	if( paramter == AFIO )
	{
		rcc[ id ]->APB2ENR |= RCC_APB2Periph_AFIO;
	}else if( paramter == GPIOA )
	{
		//Enable clock for GPIOA
		rcc[ id ]->APB2ENR |= RCC_APB2Periph_GPIOA;
	}else if( paramter == GPIOB )
	{
		//Enable clock for GPIOB
		rcc[ id ]->APB2ENR |= RCC_APB2Periph_GPIOB;
	}else if( paramter == GPIOC )
	{
		//Enable clock for GPIOC
		rcc[ id ]->APB2ENR |= RCC_APB2Periph_GPIOC;
	}else if( paramter == ADC1 )
	{
		rcc[ id ]->APB2ENR |= RCC_APB2Periph_ADC1;
	}else if( paramter == ADC2 )
	{
		rcc[ id ]->APB2ENR |= RCC_APB2Periph_ADC2;
	}else if( paramter == TIM1 )
	{
		rcc[ id ]->APB2ENR |= RCC_APB2Periph_TIM1;
	}else if( paramter == SPI1 )
	{
		rcc[ id ]->APB2ENR |= RCC_APB2Periph_SPI1;
	}else if( paramter == TIM2 )
	{
		rcc[ id ]->APB1ENR |= RCC_APB1Periph_TIM2;
	}else if( paramter == TIM3 )
	{
		rcc[ id ]->APB1ENR |= RCC_APB1Periph_TIM3;
	}else if( paramter == SPI2 )
	{
		rcc[ id ]->APB1ENR |= RCC_APB1Periph_SPI2;
	}
}
