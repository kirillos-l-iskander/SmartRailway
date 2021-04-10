#ifndef ADC_H
#define ADC_H

#include "Config.h"

typedef enum
{
	ADC_ID_1 = 0,
	ADC_ID_2,
	ADC_ID_MAX,
}	ADC_ID_t;

void Adc_init( ADC_ID_t id );
UBaseType_t Adc_getState( ADC_ID_t id );

#endif /* ADC_H */
