#ifndef IR_H
#define	IR_H

#include "CONFIG.h"
#include "IO.h"
#include "PORT.h"
#include "CORE.h"
#include "SCH_CONFIG.h"
#include "SCH.h"
#include "GPIO.h"
#include "AFIO.h"

#define IR_REG (GPIOB)
#define IR0 (0)
#define IR1 (1)
#define IR2 (10)

void IR_Init(void);
void IR_Update(void);
uint8_t IR_GetState(uint8_t IRx);

#endif	/* IR_H */
