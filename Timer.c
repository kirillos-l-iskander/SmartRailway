#include "Timer.h"

typedef TIM_TypeDef Timer_t;

static Timer_t *timer[ TIMER_ID_MAX ] = { TIM1, TIM2, TIM3 };

void initChannel( TIMER_ID_t id, TIMER_CHANNEL_t channel, TIMER_EVENT_t event );

void Timer_init( TIMER_ID_t id, TIMER_MODE_t mode, TIMER_PERIOD_t period, TIMER_CHANNEL_t channel, TIMER_EVENT_t event )
{
	timer[ id ]->EGR = 0;
	timer[ id ]->CR1 = 0;
	timer[ id ]->CR2 = 0;

	if( mode == TIMER_MODE_NORMAL )
	{
		timer[ id ]->CR1 &= ~TIM_CR1_DIR;
		timer[ id ]->CR1 &= ~TIM_CR1_CMS;
	}
	else if( mode == TIMER_MODE_COUNTUP )
	{
		timer[ id ]->CR1 &= ~TIM_CR1_DIR;
		timer[ id ]->CR1 &= ~TIM_CR1_CMS;
	}
	else if( mode == TIMER_MODE_COUNTDOWN )
	{
		timer[ id ]->CR1 |= TIM_CR1_DIR;
		timer[ id ]->CR1 &= ~TIM_CR1_CMS;
	}
	else
	{
	}

	if( event == TIMER_EVENT_CLOCKIN_RISING || event == TIMER_EVENT_CLOCKIN_FALLING )
	{
		timer[ id ]->ARR = period;
		timer[ id ]->CNT = 0;
		timer[ id ]->PSC = 0;
	}else
	{
		for( size_t prescale = 1; prescale < UINT16_MAX; prescale *= 2 )
		{
			uint32_t ticks = (uint32_t)(((((SCH_CPU_F / SCH_CPU_INSTRUCTION) / prescale) / 1000) * period) - 1);
			if( ticks < UINT16_MAX )
			{
				timer[ id ]->ARR = (uint16_t)ticks;
				timer[ id ]->CNT = 0;
				timer[ id ]->PSC = (uint16_t)prescale;
				break;
			}
		}
	}
	
	initChannel( id, channel, event );
	timer[ id ]->EGR |= TIM_EGR_UG;
	timer[ id ]->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

void initChannel( TIMER_ID_t id, TIMER_CHANNEL_t channel, TIMER_EVENT_t event )
{
	timer[ id ]->CCMR1 = 0;
	timer[ id ]->CCMR2 = 0;
	timer[ id ]->CCER = 0;

	if( channel == TIMER_CHANNEL_NORMAL )
	{
	}
	else if( channel == TIMER_CHANNEL_1 )
	{
		if( event == TIMER_EVENT_CLOCKIN_RISING )
		{
			timer[ id ]->SMCR &= ~TIM_SMCR_ETP;
			timer[ id ]->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 | TIM_SMCR_SMS;
			timer[ id ]->CCMR1 |= TIM_CCMR1_IC1F | TIM_CCMR1_CC1S_0;
		}
		else if( event == TIMER_EVENT_CLOCKIN_FALLING )
		{
			timer[ id ]->SMCR |= TIM_SMCR_ETP;
			timer[ id ]->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 | TIM_SMCR_SMS;
			timer[ id ]->CCMR1 |= TIM_CCMR1_IC1F | TIM_CCMR1_CC1S_0;
		}
		else if( event == TIMER_EVENT_PWM_NONINVERT )
		{
			timer[ id ]->CCMR1 &= ~TIM_CCMR1_CC1S;
			timer[ id ]->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
		}
		else if( event == TIMER_EVENT_PWM_INVERT )
		{
			timer[ id ]->CCMR1 &= ~TIM_CCMR1_CC1S;
			timer[ id ]->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1PE;
		}
		else
		{
		}
		timer[ id ]->CCR1 = 0;
		timer[ id ]->CCER |= TIM_CCER_CC1E;
	}
	else if( channel == TIMER_CHANNEL_2 )
	{
		if( event == TIMER_EVENT_CLOCKIN_RISING )
		{
			timer[ id ]->SMCR &= ~TIM_SMCR_ETP;
			timer[ id ]->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 | TIM_SMCR_SMS;
			timer[ id ]->CCMR1 |= TIM_CCMR1_IC2F | TIM_CCMR1_CC2S_0;
		}
		else if( event == TIMER_EVENT_CLOCKIN_FALLING )
		{
			timer[ id ]->SMCR |= TIM_SMCR_ETP;
			timer[ id ]->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 | TIM_SMCR_SMS;
			timer[ id ]->CCMR1 |= TIM_CCMR1_IC2F | TIM_CCMR1_CC2S_0;
		}
		else if( event == TIMER_EVENT_PWM_NONINVERT )
		{
			timer[ id ]->CCMR1 &= ~TIM_CCMR1_CC2S;
			timer[ id ]->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
		}
		else if( event == TIMER_EVENT_PWM_INVERT )
		{
			timer[ id ]->CCMR1 &= ~TIM_CCMR1_CC2S;
			timer[ id ]->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2PE;
		}
		else
		{
		}
		timer[ id ]->CCR2 = 0;
		timer[ id ]->CCER |= TIM_CCER_CC2E;
	}
	else if( channel == TIMER_CHANNEL_3 )
	{
		if( event == TIMER_EVENT_CLOCKIN_RISING )
		{
			timer[ id ]->SMCR &= ~TIM_SMCR_ETP;
			timer[ id ]->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 | TIM_SMCR_SMS;
			timer[ id ]->CCMR2 |= TIM_CCMR2_IC3F | TIM_CCMR2_CC3S_0;
		}
		else if( event == TIMER_EVENT_CLOCKIN_FALLING )
		{
			timer[ id ]->SMCR |= TIM_SMCR_ETP;
			timer[ id ]->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 | TIM_SMCR_SMS;
			timer[ id ]->CCMR2 |= TIM_CCMR2_IC3F | TIM_CCMR2_CC3S_0;
		}
		else if( event == TIMER_EVENT_PWM_NONINVERT )
		{
			timer[ id ]->CCMR2 &= ~TIM_CCMR2_CC3S;
			timer[ id ]->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
		}
		else if( event == TIMER_EVENT_PWM_INVERT )
		{
			timer[ id ]->CCMR2 &= ~TIM_CCMR2_CC3S;
			timer[ id ]->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3PE;
		}
		else
		{
		}
		timer[ id ]->CCR3 = 0;
		timer[ id ]->CCER |= TIM_CCER_CC3E;
	}
	else if( channel == TIMER_CHANNEL_4 )
	{
		if( event == TIMER_EVENT_CLOCKIN_RISING )
		{
			timer[ id ]->SMCR &= ~TIM_SMCR_ETP;
			timer[ id ]->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 | TIM_SMCR_SMS;
			timer[ id ]->CCMR2 |= TIM_CCMR2_IC4F | TIM_CCMR2_CC4S_0;
		}
		else if( event == TIMER_EVENT_CLOCKIN_FALLING )
		{
			timer[ id ]->SMCR |= TIM_SMCR_ETP;
			timer[ id ]->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 | TIM_SMCR_SMS;
			timer[ id ]->CCMR2 |= TIM_CCMR2_IC4F | TIM_CCMR2_CC4S_0;
		}
		else if( event == TIMER_EVENT_PWM_NONINVERT )
		{
			timer[ id ]->CCMR2 &= ~TIM_CCMR2_CC4S;
			timer[ id ]->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;
		}
		else if( event == TIMER_EVENT_PWM_INVERT )
		{
			timer[ id ]->CCMR2 &= ~TIM_CCMR2_CC4S;
			timer[ id ]->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0| TIM_CCMR2_OC4PE;
		}
		else
		{
		}
		timer[ id ]->CCR4 = 0;
		timer[ id ]->CCER |= TIM_CCER_CC4E;
	}
	else
	{
	}
}

void Timer_setCounter( TIMER_ID_t id, TIMER_COUNTER_t counter )
{
	timer[ id ]->CNT = (uint16_t)counter;
}

TIMER_COUNTER_t Timer_getCounter( TIMER_ID_t id )
{
	return (TIMER_COUNTER_t)timer[ id ]->CNT;
}

void Timer_setPwmDutyCycle( TIMER_ID_t id, TIMER_CHANNEL_t channel, TIMER_PWM_t pwmDutyCycle )
{
	if( channel == TIMER_CHANNEL_NORMAL )
	{
	}
	else if( channel == TIMER_CHANNEL_1 )
	{
		timer[ id ]->CCR1 = (uint16_t)(((float)pwmDutyCycle / 100) * timer[ id ]->ARR);
	}
	else if( channel == TIMER_CHANNEL_2 )
	{
		timer[ id ]->CCR2 = (uint16_t)(((float)pwmDutyCycle / 100) * timer[ id ]->ARR);
	}
	else if( channel == TIMER_CHANNEL_3 )
	{
		timer[ id ]->CCR3 = (uint16_t)(((float)pwmDutyCycle / 100) * timer[ id ]->ARR);
	}
	else if( channel == TIMER_CHANNEL_4 )
	{
		timer[ id ]->CCR4 = (uint16_t)(((float)pwmDutyCycle / 100) * timer[ id ]->ARR);
	}
	else
	{
	}
}

TIMER_PWM_t Timer_getPwmDutyCycle( TIMER_ID_t id, TIMER_CHANNEL_t channel )
{
	if( channel == TIMER_CHANNEL_NORMAL )
	{
	}
	else if( channel == TIMER_CHANNEL_1 )
	{
		return (TIMER_PWM_t)(((float)timer[ id ]->CCR1 / timer[ id ]->ARR) * 100);
	}
	else if( channel == TIMER_CHANNEL_2 )
	{
		return (TIMER_PWM_t)(((float)timer[ id ]->CCR2 / timer[ id ]->ARR) * 100);
	}
	else if( channel == TIMER_CHANNEL_3 )
	{
		return (TIMER_PWM_t)(((float)timer[ id ]->CCR3 / timer[ id ]->ARR) * 100);
	}
	else if( channel == TIMER_CHANNEL_4 )
	{
		return (TIMER_PWM_t)(((float)timer[ id ]->CCR4 / timer[ id ]->ARR) * 100);
	}
	else
	{
	}
	return TIMER_PWM_MIN;
}
