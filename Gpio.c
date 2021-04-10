#include "Gpio.h"

typedef GPIO_TypeDef Gpio_t;

static Gpio_t *gpio[ GPIO_ID_MAX ] = { GPIOA, GPIOB, GPIOC };

void Gpio_initPin( GPIO_ID_t id, GPIO_PIN_t pin, GPIO_MODE_t mode, GPIO_TYPE_t type )
{
	uint8_t data = ((uint8_t)mode | (uint8_t)type) & 0xF;
	if( pin < 8 )
	{
		gpio[ id ]->CRL = (gpio[ id ]->CRL & ~(unsigned)(0xF << (pin * 4))) | (unsigned)(data << (pin * 4));
	}else
	{
		pin = pin - 8;
		gpio[ id ]->CRH = (gpio[ id ]->CRH & ~(unsigned)(0xF << (pin * 4))) | (unsigned)(data << (pin * 4));	
	}
}

void Gpio_setPinState( GPIO_ID_t id, GPIO_PIN_t pin, GPIO_STATE_t state )
{
	state &= 0x1;
	gpio[ id ]->ODR = (uint16_t)(gpio[ id ]->ODR & ~(1 << pin)) | (uint16_t)(state << pin);
}

GPIO_STATE_t Gpio_getPinState( GPIO_ID_t id, GPIO_PIN_t pin )
{
	return (GPIO_STATE_t)((gpio[ id ]->IDR & (1 << pin)) >> pin);
}

void Gpio_setPortState( GPIO_ID_t id, GPIO_PIN_t pins, GPIO_STATE_t state )
{
	state &= pins;
	gpio[ id ]->ODR = (uint16_t)(gpio[ id ]->ODR & ~pins) | (uint16_t)(state);
}

GPIO_STATE_t Gpio_getPortState( GPIO_ID_t id, GPIO_PIN_t pins )
{
	return (GPIO_STATE_t)(gpio[ id ]->IDR & pins);
}
