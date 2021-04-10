#include "Adc.h"

typedef ADC_TypeDef Adc_t;

static Adc_t *adc[ ADC_ID_MAX ] = { ADC1, ADC2 };

void Adc_init( ADC_ID_t id )
{
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;
		
	//Sampling
	adc[ id ]->SMPR1 &= ~( ADC_SMPR1_SMP15 | ADC_SMPR1_SMP14 | ADC_SMPR1_SMP13 | ADC_SMPR1_SMP12 | ADC_SMPR1_SMP11 | ADC_SMPR1_SMP10 );
	adc[ id ]->SMPR2 = ADC_SMPR2_SMP0;

	//Channels
	adc[ id ]->SQR1 &= ~ADC_SQR1_L;
	adc[ id ]->SQR2 &= ~( ADC_SQR2_SQ12 | ADC_SQR2_SQ11 | ADC_SQR2_SQ10 );
	adc[ id ]->SQR3 &= ~ADC_SQR3_SQ1;		//SQ1 = PA0

	adc[ id ]->CR1 = 0;

	adc[ id ]->CR2 |= ADC_CR2_EXTSEL | ADC_CR2_ADON;
	DELAY_MS(1);
	adc[ id ]->CR2 |= ADC_CR2_ADON;
	DELAY_MS(1);

	//Calibration
	adc[ id ]->CR2 |= ADC_CR2_RSTCAL;					//Reset calibration
	while( adc[ id ]->CR2 & ADC_CR2_RSTCAL );	//Wait until reset finished
	adc[ id ]->CR2 |= ADC_CR2_CAL;						//Strat calibration
	while( adc[ id ]->CR2 & ADC_CR2_CAL );		//Wait until calbration finished
}

UBaseType_t Adc_getState( ADC_ID_t id )
{
	//adc[ id ]->CR2 |= ADC_CR2_SWSTART;			//Start SW conversion
	adc[ id ]->CR2 |= ADC_CR2_ADON;
	while( !( adc[ id ]->SR & ADC_SR_EOC ) );		//Check EOC flag
	return adc[ id ]->DR;												//Read ADC
}
