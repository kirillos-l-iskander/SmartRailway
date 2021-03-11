#include "Adc.h"

#define ADC1RST ( (unsigned ) 1 << 9 )
#define ADC2RST ( (unsigned) 1 << 10 )

#define ADC1EN ( (unsigned) 1 << 9 )
#define ADC2EN ( (unsigned) 1 << 10 )

typedef ADC_TypeDef Adc_t;

static Adc_t *adc[ 2 ] = { ADC1, ADC2 };

void Adc_init( Id_t id )
{
	//Prescaler CLK/6
	RCC->CFGR |= ( 0x2 << 14 );
		
	//Sampling
	adc[ id ]->SMPR1 = ( 0 << 0 );
	adc[ id ]->SMPR2 = ( 7 << 0 );		//SMP0 = 239.5 Cycles

	//Channels
	adc[ id ]->SQR1 = ( 0 << 20 );		//L = One Conversion
	adc[ id ]->SQR2 = ( 0 << 0 );
	adc[ id ]->SQR3 = ( 0 << 0 );		//SQ1 = PA0

	adc[ id ]->CR1 = (0<<0);

	//EXTSEL = SWSTART
	//ADON = ON
	adc[ id ]->CR2 |= ( 7 << 17 ) | ( 1 << 0 );
	
	DELAY_MS(1);

	//ADON = ON
	adc[ id ]->CR2 |= ( 1 << 0 );
	
	DELAY_MS(1);

	//Calibration
	adc[ id ]->CR2 |= ( 1 << 3 );					//Reset calibration
	while( adc[ id ]->CR2 & ( 1 << 3 ) );	//Wait until reset finished
	adc[ id ]->CR2 |= ( 1 << 2 );					//Strat calibration
	while( adc[ id ]->CR2 & ( 1 << 2 ) );	//Wait until calbration finished
}

UBaseType_t Adc_getState( Id_t id )
{
	//pxAdcHandle->CR2 |= ( 1 << 22 );					//Start SW conversion
	//ADON = ON
	adc[ id ]->CR2 |= ( 1 << 0 );
	while( !( adc[ id ]->SR & ( 1 << 1 ) ) );		//Check EOC flag
	return adc[ id ]->DR;												//Read ADC
}
