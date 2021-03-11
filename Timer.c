#include "Timer.h"

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

typedef TIM_TypeDef Timer_t;

static Timer_t *timer[ 3 ] = { TIM1, TIM2, TIM3 };

void Timer_initCounter( Id_t id )
{
	timer[ id ]->ARR = 65535;
	timer[ id ]->CNT = 0;
	timer[ id ]->EGR |= UG;
	timer[ id ]->SMCR |= TS_TI1 | SMS;
	timer[ id ]->CCMR1 |= IC1F | CC1S_CH1TI1;
	timer[ id ]->CCER |= CC1E;
	timer[ id ]->CR1 |= ARPE | CEN;
}

void Timer_setCounterValue( Id_t id, UBaseType_t counterValue )
{
	timer[ id ]->CNT = ( uint16_t ) counterValue;
}

UBaseType_t Timer_getCounterValue( Id_t id )
{
	return timer[ id ]->CNT;
}

void Timer_initPwm( Id_t id )
{
	timer[ id ]->PSC = 72;
	timer[ id ]->ARR = 65535;
	timer[ id ]->CCR1 = 0;
	timer[ id ]->CNT = 0;
	timer[ id ]->EGR |= UG;
	timer[ id ]->CCMR1 |= OC1M | OC1PE;
	timer[ id ]->CCER |= CC1E;
	timer[ id ]->CR1 |= ARPE | CEN;
}

void Timer_setPwmDutyCycle( Id_t id, UBaseType_t pwmDutyCycle )
{
	timer[ id ]->CCR1 = ( uint16_t ) pwmDutyCycle;
}

UBaseType_t Timer_getPwmDutyCycle( Id_t id )
{
	return timer[ id ]->CCR1;
}
