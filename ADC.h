#ifndef ADC_H
#define ADC_H

#include "SchedulerConfig.h"

#define ADC1_ID	( 0 )
#define ADC2_ID	( 1 )

void Adc_init( Id_t Id );
UBaseType_t Adc_getState( Id_t id );

#endif /* ADC_H */
