#ifndef LED_H
#define	LED_H

#include "CONFIG.h"
#include "IO.h"
#include "PORT.h"
#include "CORE.h"
#include "SCH_CONFIG.h"
#include "SCH.h"
#include "GPIO.h"
#include "AFIO.h"

#define LED_REG (GPIOC)
#define LED0 (13)

typedef struct
{
	uint8_t State;
}LED_t;

void LED_Init(void);
void LED_Update(void);
void LED_SetState(uint8_t LEDx, uint8_t State);

#endif	/* LED_H */
