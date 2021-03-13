#include "Lcd.h"

typedef struct
{
	Id_t rsGpioId;
	uint8_t rsPin;
	Id_t rwGpioId;
	uint8_t rwPin;
	Id_t eGpioId;
	uint8_t ePin;
	Id_t dGpioId;
	uint8_t dPin;
}Lcd_t;

static Lcd_t lcd[ LCD_NUMBER ];
static uint8_t lcdRowAddress[ 4 ] = { 0x80, 0xC0, 0x90, 0xD0 };

void Lcd_init( Id_t id, Id_t rsGpioId, uint8_t rsPin, Id_t eGpioId, uint8_t ePin, Id_t dGpioId, uint8_t dPin )
{
	size_t pin = 0;
	lcd[ id ].rsGpioId = rsGpioId;
	lcd[ id ].rsPin = rsPin;
	lcd[ id ].eGpioId = eGpioId;
	lcd[ id ].ePin = ePin;
	lcd[ id ].dGpioId = dGpioId;
	lcd[ id ].dPin = dPin;
	Gpio_initPin( lcd[ id ].rsGpioId, lcd[ id ].rsPin, OUTPUT );
	Gpio_initPin( lcd[ id ].eGpioId, lcd[ id ].ePin, OUTPUT );
	for ( pin = lcd[ id ].dPin; pin < ( lcd[ id ].dPin + 8 ); pin++ )
	{
		Gpio_initPin( lcd[ id ].dGpioId, pin, OUTPUT );
	}
	Lcd_setCommand( id, RETURN_HOME );
	DELAY_US( 100 );
	Lcd_setCommand( id, DISPLAY_2_LINES_5x7_MATRIX_8_BIT );
	DELAY_US( 100 );
	Lcd_setCommand( id, DISPLAY_ON_CURSOR_OFF );
	Lcd_setCommand( id, INCREMENT_CURSOR );
	DELAY_US( 100 );
	Lcd_clear( id );
}

void Lcd_setCommand( Id_t id, uint8_t command )
{
	Gpio_setPortState( lcd[ id ].dGpioId, ( 0xFFU << lcd[ id ].dPin ), command );
	Gpio_setPinState( lcd[ id ].rsGpioId, lcd[ id ].rsPin, LOW );
	Gpio_setPinState( lcd[ id ].eGpioId, lcd[ id ].ePin, HIGH );
	DELAY_US( 1 );
	Gpio_setPinState( lcd[ id ].eGpioId, lcd[ id ].ePin, LOW );
	DELAY_US( 50 );
}

void Lcd_setCharacter( Id_t id, uint8_t character )
{
	Gpio_setPortState( lcd[ id ].dGpioId, ( 0xFFU << lcd[ id ].dPin ), character );
	Gpio_setPinState( lcd[ id ].rsGpioId, lcd[ id ].rsPin, HIGH );
	Gpio_setPinState( lcd[ id ].eGpioId, lcd[ id ].ePin, HIGH );
	DELAY_US( 1 );
	Gpio_setPinState( lcd[ id ].eGpioId, lcd[ id ].ePin, LOW );
}

void Lcd_setString( Id_t id, uint8_t *string )
{
	size_t index = 0;
	for( index = 0; string[ index ] != 0; index++ )
	{
		Lcd_setCharacter( id, string[ index ] );
	}
}

void Lcd_setCursor( Id_t id, uint8_t row, uint8_t col )
{
	Lcd_setCommand( id, lcdRowAddress[ row ] | col );
}

void Lcd_clear( Id_t id )
{
	Lcd_setCommand( id, CLEAR_DISPLAY );
	Lcd_setCommand( id, FORCE_CURSOR_HOME );
	DELAY_US( 2000 );
}
