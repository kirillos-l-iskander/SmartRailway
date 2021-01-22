#include "LED.h"

volatile static LED_t LED_G[1];

void LED_Init(void)
{
	GPIO_InitPin(LED_REG, LED0, GPIO_Mode_Out_PP | GPIO_Speed_50MHz);
}

void LED_Update(void)
{
	GPIO_SetPinState(LED_REG, LED0, !LED_G[0].State);
}

void LED_SetState(uint8_t LEDx, uint8_t State)
{
	switch(LEDx)
	{
		case LED0:
			LED_G[0].State = State & 0x01;
			break;
	}
}
