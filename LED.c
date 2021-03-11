#include "Led.h"

typedef struct
{
	Id_t gpio_id;
	uint8_t pin;
	uint8_t state;
}Led_t;

volatile static Led_t led[ LED_NUMBER ];

void Led_init( void )
{
	size_t id = 0;
	for ( id = 0; id < LED_NUMBER; id++ )
	{
		led[ id ].state = LOW;
		Gpio_initPin( led[ id ].gpio_id, led[ id ].pin, OUTPUT );
	}
}

void Led_setState( uint8_t id, uint8_t state )
{
  led[ id ].state = state;
	Gpio_setPinState( led[ id ].gpio_id, led[ id ].pin, led[ id ].state );
}

void Led_setGpio( Id_t id, Id_t gpio_id, uint8_t pin )
{
	led[ id ].gpio_id = gpio_id;
	led[ id ].pin = pin;
}
