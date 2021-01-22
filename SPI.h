#ifndef SPI_H
#define	SPI_H

#include "CONFIG.h"
#include "IO.h"
#include "PORT.h"
#include "CORE.h"
#include "SCH_CONFIG.h"
#include "SCH.h"
#include "GPIO.h"
#include "AFIO.h"

#define SPI2_REG GPIOB
#define SPI2_MOSI 15
#define SPI2_MISO 14
#define SPI2_SCK 13
#define SPI2_NSS 12

//CR1
#define SSM (1<<9)
#define SPE (1<<6)
#define BR (6<<3)
#define MSTR (1<<2)

//CR2
#define SSOE (1<<2)

//SR
#define BSY (1<<7)
#define TXE (1<<1)
#define RXNE (1<<0)

void SPI2_Init(void);
uint8_t SPI2_Transfer(uint8_t Buff);

#endif	/* SPI_H */
