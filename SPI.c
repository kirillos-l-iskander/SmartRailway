#include "SPI.h"

void SPI2_Init(void)
{
	AFIO_Init();
	GPIO_InitPin(SPI2_REG, SPI2_MOSI, GPIO_Mode_AF_PP | GPIO_Speed_50MHz);
	GPIO_InitPin(SPI2_REG, SPI2_MISO, GPIO_Mode_IN_FLOATING | GPIO_Input);
	GPIO_InitPin(SPI2_REG, SPI2_SCK, GPIO_Mode_AF_PP | GPIO_Speed_50MHz);
	GPIO_InitPin(SPI2_REG, SPI2_NSS, GPIO_Mode_AF_PP | GPIO_Speed_50MHz);
	RCC->APB1ENR = RCC->APB1ENR | RCC_APB1Periph_SPI2;
	SPI2->CR2 = SPI2->CR2 | SSOE;
	SPI2->CR1 = SPI2->CR1 | SSM | SPE | BR | MSTR;
}

uint8_t SPI2_Transfer(uint8_t Buff)
{
	uint8_t Data;
	while(!(SPI2->SR & TXE));
	SPI2->DR = Buff;
	while(!(SPI2->SR & RXNE));
	Data = (uint8_t) SPI2->DR;
	while(SPI2->SR & BSY);
	return Data;
}
