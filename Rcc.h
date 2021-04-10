#ifndef RCC_H
#define RCC_H

#include "Config.h"

typedef enum
{
	RCC_ID = 0,
	RCC_ID_MAX,
}	RCC_ID_t;

void Rcc_enableClock( RCC_ID_t id, void *paramter );

#endif /* RCC_H */
