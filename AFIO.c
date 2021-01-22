#include "AFIO.h"

void AFIO_Init(void)
{
	RCC->APB2ENR = RCC->APB2ENR | RCC_APB2Periph_AFIO;
}

void Disable_JTAG(void)
{
	AFIO->MAPR = AFIO->MAPR | (0x4<<24);
}
