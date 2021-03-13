#ifndef RCC_H
#define RCC_H

#include "SchedulerConfig.h"

typedef enum
{
	RCC_ID
}RCC_t;

void Rcc_enableClock( Id_t id, void *paramter );

#endif /* RCC_H */
