#include "Gpio.h"

typedef GPIO_TypeDef Gpio_t;

static Gpio_t *gpio[ 3 ] = { GPIOA, GPIOB, GPIOC };

void Gpio_initPin( Id_t id, UBaseType_t pin, UBaseType_t mode )
{
	if( pin < 8 )
	{
		gpio[ id ]->CRL = ( gpio[ id ]->CRL & ~( 0xFU << ( pin * 4 ) ) ) | ( ( mode & 0xF ) << ( pin * 4 ) );
	}else
	{
		pin = pin - 8;
		gpio[ id ]->CRH = ( gpio[ id ]->CRH & ~( 0xFU << ( pin * 4 ) ) ) | ( ( mode & 0xF ) << ( pin * 4 ) );	
	}
}

void Gpio_setPinState( Id_t id, UBaseType_t pin, UBaseType_t state )
{
	gpio[ id ]->ODR = ( gpio[ id ]->ODR & ~( 1 << pin ) ) | ( ( state & 0x1 ) << pin );
}

UBaseType_t Gpio_getPinState( Id_t id, UBaseType_t pin )
{
	return ( ( gpio[ id ]->IDR & ( 1 << pin ) ) >> pin );
}

void Gpio_setPortState( Id_t id, UBaseType_t pins, UBaseType_t state )
{
	gpio[ id ]->ODR = ( gpio[ id ]->ODR & ~pins ) | ( state & pins );
}

UBaseType_t Gpio_getPortState( Id_t id, UBaseType_t pins )
{
	return ( gpio[ id ]->IDR & pins );
}
