#ifndef ADC_H
#define ADC_H

#include "SchedulerConfig.h"

typedef enum
{
	ADC1_ID,
	ADC2_ID
}ADC_t;

void Adc_init( Id_t id );
UBaseType_t Adc_getState( Id_t id );

#endif /* ADC_H */
