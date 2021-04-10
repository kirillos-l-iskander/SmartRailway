#include "Led.h"

typedef struct
{
	GPIO_ID_t gpioId;
	GPIO_PIN_t gpioPin;
	LED_STATE_t state;
	LED_STATE_t blink;
	LED_BLINK_t period;
	LED_BLINK_t delay;
}	Led_t;

static Led_t led[ LED_ID_MAX ];

void initLedHardware( LED_ID_t id );
void setLedHardwareState( LED_ID_t id, LED_STATE_t state );

void Led_init( LED_ID_t id, GPIO_ID_t gpioId, GPIO_PIN_t gpioPin )
{
	led[ id ].gpioId = gpioId;
	led[ id ].gpioPin = gpioPin;
	led[ id ].state = LED_STATE_OFF;
	led[ id ].blink = LED_STATE_OFF;
	led[ id ].period = LED_BLINK_0MS;
	led[ id ].delay = LED_BLINK_0MS;
	initLedHardware( id );
}

void Led_update( void *paramter )
{
	LED_ID_t id = (LED_ID_t) paramter;
	if( led[ id ].delay )
	{
		led[ id ].delay--;
	}else if( led[ id ].period )
	{
		led[ id ].blink ^= led[ id ].state;
		led[ id ].delay = led[ id ].period;
	}else
	{
		led[ id ].blink = 0;
	}
	setLedHardwareState( id, led[ id ].state & !led[ id ].blink );
}

void Led_setState( LED_ID_t id, LED_STATE_t state, LED_BLINK_t period )
{
	led[ id ].state = state;
	led[ id ].period = MS_TO_TICKS( period ) / MS_TO_TICKS( 1 );
}

void initLedHardware( LED_ID_t id )
{
	Gpio_initPin( led[ id ].gpioId, led[ id ].gpioPin, GPIO_MODE_OUTPUT_DHZ, GPIO_TYPE_GP_PP );
}

void setLedHardwareState( LED_ID_t id, LED_STATE_t state )
{
	if( state == LED_STATE_OFF )
	{
		Gpio_setPinState( led[ id ].gpioId, led[ id ].gpioPin, GPIO_STATE_HIGH );
	}else if( state == LED_STATE_ON )
	{
		Gpio_setPinState( led[ id ].gpioId, led[ id ].gpioPin, GPIO_STATE_LOW );
	}else
	{
	}
}
