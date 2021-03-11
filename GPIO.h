#ifndef PORT_H
#define PORT_H

#include "SchedulerConfig.h"

#define GPIOA_ID	( 0 )
#define GPIOB_ID	( 1 )
#define GPIOC_ID	( 2 )

#define INPUT			( GPIO_Mode_IN_FLOATING | GPIO_Input )
#define OUTPUT		( GPIO_Mode_Out_PP | GPIO_Speed_50MHz )
#define OUTPUT_AF	( GPIO_Mode_AF_PP | GPIO_Speed_50MHz )
#define LOW				( 0 )
#define HIGH			( 1 )

void Gpio_initPin( Id_t id, UBaseType_t pin, UBaseType_t mode );
void Gpio_setPinState( Id_t id, UBaseType_t pin, UBaseType_t state );
UBaseType_t Gpio_getPinState( Id_t id, UBaseType_t pin );
void Gpio_setPortState( Id_t id, UBaseType_t pins, UBaseType_t state );
UBaseType_t Gpio_getPortState( Id_t id, UBaseType_t pins );

#endif /* PORT_H */
