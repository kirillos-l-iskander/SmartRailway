#include "Led.h"

typedef struct
{
	Id_t xGpioId;
	uint8_t xPin;
	uint8_t state;
}Led_t;

volatile static Led_t led[ LED_NUMBER ];

void Led_init( Id_t id, Id_t xGpioId, uint8_t xPin )
{
	led[ id ].xGpioId = xGpioId;
	led[ id ].xPin = xPin;
	led[ id ].state = LOW;
	Gpio_initPin( led[ id ].xGpioId, led[ id ].xPin, OUTPUT );
}

void Led_setState( Id_t id, uint8_t state )
{
  led[ id ].state = state;
	Gpio_setPinState( led[ id ].xGpioId, led[ id ].xPin, led[ id ].state );
}
