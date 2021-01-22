#include "ADC.h"

volatile static uint16_t ADC_State;

void ADC_Init(ADC_TypeDef* ADCx)
{
	AFIO_Init();
	GPIO_InitPin(GPIOA, 0, GPIO_Mode_AIN | GPIO_Input);
	//Prescaler CLK/6
	RCC->CFGR |= (0x2<<14);
  if (ADCx == ADC1)
  {
    /* Enable ADC1 reset state */
    RCC->APB2RSTR |= ADC1RST;
		/* Release ADC1 from reset state */
		RCC->APB2RSTR &= ~ADC1RST;
		//ADC Clock Enable
		RCC->APB2ENR |= ADC1EN;
  }else if (ADCx == ADC2)
  {
    /* Enable ADC1 reset state */
    RCC->APB2RSTR |= ADC2RST;
		/* Release ADC1 from reset state */
		RCC->APB2RSTR &= ~ADC2RST;
		//ADC Clock Enable
		RCC->APB2ENR |= ADC2EN;
  }
		
	//Sampling
	ADC1->SMPR1 = (0<<0);
	ADC1->SMPR2 = (7<<0);		//SMP0 = 239.5 Cycles

	//Channels
	ADCx->SQR1 = (0<<20);		//L = One Conversion
	ADCx->SQR2 = (0<<0);
	ADC1->SQR3 = (0<<0);		//SQ1 = PA0

	ADCx->CR1 = (0<<0);

	//EXTSEL = SWSTART
	//ADON = ON
	ADCx->CR2 |= (7<<17) | (1<<0);
	
	HW_Delay_ms(1);

	//ADON = ON
	ADCx->CR2 |= (1<<0);
	
	HW_Delay_ms(1);

	//Calibration
	ADC1->CR2 |= (1<<3);				//Reset calibration
	while(ADC1->CR2 & (1<<3));	//Wait until reset finished
	ADC1->CR2 |= (1<<2);				//Strat calibration
	while(ADC1->CR2 & (1<<2));	//Wait until calbration finished
}

void ADC_Update(void)
{
	if(!(ADC1->SR & (1<<1)))		//Check EOC flag
	{
		return;
	}
	ADC_State = (uint16_t)ADC1->DR;				//Read ADC
	//ADC1->CR2 |= (1<<22);				//Start SW conversion
	//ADON = ON
	ADC1->CR2 |= (1<<0);
}

uint16_t ADC_GetState(void)
{
	return ADC_State;
}
