#ifndef ADC_H
#define ADC_H

#include "CONFIG.h"
#include "IO.h"
#include "PORT.h"
#include "CORE.h"
#include "SCH_CONFIG.h"
#include "SCH.h"
#include "GPIO.h"
#include "AFIO.h"

#define ADC1RST ((unsigned)1<<9)
#define ADC2RST ((unsigned)1<<10)

#define ADC1EN ((unsigned)1<<9)
#define ADC2EN ((unsigned)1<<10)

void ADC_Init(ADC_TypeDef * ADCx);
void ADC_Update(void);
uint16_t ADC_GetState(void);

#endif /* ADC_H */
