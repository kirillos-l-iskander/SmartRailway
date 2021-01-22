#include "NRF24L01+.h"

volatile static uint8_t NRF_State_G = 0;
volatile static uint8_t NRF_TxBuff_G[PAYLOAD_SIZE] = {0};
volatile static uint8_t NRF_RxBuff_G[PAYLOAD_SIZE] = {0};
volatile static uint8_t NRF_Addr_G[6][5] = {{0xF0, 0xF0, 0xF0, 0xF0, 0x00},
																						{0xF0, 0xF0, 0xF0, 0xF0, 0x01},
																						{0xF0, 0xF0, 0xF0, 0xF0, 0x02},
																						{0xF0, 0xF0, 0xF0, 0xF0, 0x03},
																						{0xF0, 0xF0, 0xF0, 0xF0, 0x04},
																						{0xF0, 0xF0, 0xF0, 0xF0, 0x05}};

void NRF_CSN(uint8_t State)
{
	if(State)
	{
		GPIO_SetPinState(NRF_REG, CSN, HIGH);
	}else
	{
		GPIO_SetPinState(NRF_REG, CSN, LOW);
	}
}

void NRF_CE(uint8_t State)
{
	if(State)
	{
		GPIO_SetPinState(NRF_REG, CE, HIGH);
	}else
	{
		GPIO_SetPinState(NRF_REG, CE, LOW);
	}
}

void NRF_WriteByte(volatile uint8_t Reg, volatile uint8_t Buff)
{
	NRF_CSN(LOW);
	SPI2_Transfer(W_REGISTER | (REGISTER_MASK & Reg));
	SPI2_Transfer(Buff);
	NRF_CSN(HIGH);
}

void NRF_WritePage(volatile uint8_t Reg, volatile uint8_t* Buff, volatile uint8_t Len)
{
	Buff = Buff + Len - 1;
	NRF_CSN(LOW);
	SPI2_Transfer(W_REGISTER | (REGISTER_MASK & Reg));
	while(Len--)
	{
		SPI2_Transfer(*Buff--);
	}
	NRF_CSN(HIGH);
}

uint8_t NRF_ReadByte(volatile uint8_t Reg)
{
	uint8_t Buff;
	NRF_CSN(LOW);
	SPI2_Transfer(R_REGISTER | (REGISTER_MASK & Reg));
	Buff = SPI2_Transfer(NOP);
	NRF_CSN(HIGH);
	return Buff;
}

void NRF_ReadPage(volatile uint8_t Reg, volatile uint8_t* Buff, volatile uint8_t Len)
{
	Buff = Buff + Len - 1;
	NRF_CSN(LOW);
	SPI2_Transfer(R_REGISTER | (REGISTER_MASK & Reg));
	while(Len--)
	{
		*Buff-- = SPI2_Transfer(NOP);
	}
	NRF_CSN(HIGH);
}

void NRF_FlushTx(void)
{
	NRF_CSN(LOW);
  SPI2_Transfer(FLUSH_TX);
	NRF_CSN(HIGH);
}

void NRF_FlushRx(void)
{
	NRF_CSN(LOW);
  SPI2_Transfer(FLUSH_RX);
	NRF_CSN(HIGH);
}

void NRF_StartTxMode(void)
{
	NRF_WriteByte(CONFIG, (NRF_ReadByte(CONFIG) | _BV(PWR_UP)) & ~_BV(PRIM_RX));
	//Delay_us(150);
}

void NRF_StartRxMode(void)
{
	NRF_WriteByte(CONFIG, NRF_ReadByte(CONFIG) | _BV(PWR_UP) | _BV(PRIM_RX));
	NRF_CE(HIGH);
}

void NRF_StopRxMode(void)
{
	//Delay_us(130);
	NRF_CE(LOW);
}

uint8_t NRF_CheckTxFlag(void)
{
	if(NRF_ReadByte(STATUS) & _BV(TX_DS))
	{
		NRF_WriteByte(STATUS, NRF_ReadByte(STATUS) | _BV(TX_DS));
		return TRUE;
	}
	if(NRF_ReadByte(STATUS) & _BV(MAX_RT))
	{
		NRF_WriteByte(STATUS, NRF_ReadByte(STATUS) | _BV(MAX_RT));
		SPI2_Transfer(REUSE_TX_PL);
		NRF_CE(HIGH);
		Delay_us(15);
		NRF_CE(LOW);
	}
	return FALSE;
}

uint8_t NRF_CheckRxFlag(void)
{
	if(NRF_ReadByte(STATUS) & _BV(RX_DR))
	{
		NRF_WriteByte(STATUS, NRF_ReadByte(STATUS) | _BV(RX_DR));
		return TRUE;
	}
	return FALSE;
}

void NRF_Transmit(volatile uint8_t* Buff)
{
	uint8_t len = PAYLOAD_SIZE;
	NRF_CSN(LOW);
	SPI2_Transfer(W_TX_PAYLOAD);
	while(len--)
	{
		SPI2_Transfer(*Buff++);
	}
	NRF_CSN(HIGH);
	NRF_CE(HIGH);
	Delay_us(15);
	NRF_CE(LOW);
}

void NRF_Receive(volatile uint8_t* Buff)
{
	uint8_t len = PAYLOAD_SIZE;
	NRF_CSN(LOW);
	SPI2_Transfer(R_RX_PAYLOAD);
	while(len--)
	{
		*Buff++ = SPI2_Transfer(NOP);
	}
	NRF_CSN(HIGH);
}

void NRF_ACK(volatile uint8_t* Buff)
{
	uint8_t len = PAYLOAD_SIZE;
	NRF_CSN(LOW);
	SPI2_Transfer(W_ACK_PAYLOAD);
	while(len--)
	{
		SPI2_Transfer(*Buff++);
	}
	NRF_CSN(HIGH);
}

void NRF_SetChannel(uint8_t Channel)
{
  NRF_WriteByte(RF_CH, Channel);
}

void NRF_OpenWritingPipe(volatile uint8_t* Addr)
{
  NRF_WritePage(TX_ADDR, Addr, ADDR_WIDTH);
  NRF_WritePage(RX_ADDR_P0, Addr, ADDR_WIDTH);
}

void NRF_OpenReadingPipe1(volatile uint8_t* Addr)
{
	NRF_WritePage(RX_ADDR_P1, Addr, ADDR_WIDTH);
}

void NRF_OpenReadingPipe2(volatile uint8_t* Addr)
{
	NRF_WriteByte(RX_ADDR_P2, Addr[4]);
}

void NRF_InitSR(void)
{
	SPI2_Init();
	GPIO_InitPin(NRF_REG, CSN, GPIO_Mode_Out_PP | GPIO_Speed_50MHz);
	GPIO_InitPin(NRF_REG, CE, GPIO_Mode_Out_PP | GPIO_Speed_50MHz);
	NRF_CSN(HIGH);
	NRF_CE(LOW);

	//Reset Interrupt
	NRF_WriteByte(CONFIG, _BV(MASK_RX_DR) | _BV(MASK_TX_DS) | _BV(MASK_MAX_RT) | _BV(EN_CRC) | _BV(CRCO));
  //Reset Auto Acknowledge
	NRF_WriteByte(EN_AA, 0x01);
	//Enable Payloads
	NRF_WriteByte(EN_RXADDR, _BV(ERX_P0));
	//Set Address Size
	NRF_WriteByte(SETUP_AW, 0x03);
	//Set Retries
	NRF_WriteByte(SETUP_RETR, ((DELAY & 0xF)<<ARD | (COUNT & 0xF)<<ARC));
	//Set Channel to 0
  NRF_WriteByte(RF_CH, 0x00);
	//Set DataRate to RF24_250KPS
	NRF_WriteByte(RF_SETUP, (NRF_ReadByte(RF_SETUP) | _BV(RF_DR_LOW)) & ~_BV(RF_DR_HIGH));
	//Set PA to RF24_PA_MAX
	NRF_WriteByte(RF_SETUP, (NRF_ReadByte(RF_SETUP) & 0xF8) | RF24_PA_MAX<<RF_PWR);
  NRF_WritePage(TX_ADDR, NRF_Addr_G[0], ADDR_WIDTH);
  NRF_WritePage(RX_ADDR_P0, NRF_Addr_G[0], ADDR_WIDTH);
  NRF_WriteByte(RX_PW_P0, PAYLOAD_SIZE);
	NRF_WriteByte(FEATURE, 0x00);
  NRF_WriteByte(DYNPD, 0x00);

  NRF_WriteByte(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
  NRF_FlushTx();
  NRF_FlushRx();
}

void NRF_InitST1(void)
{
	SPI2_Init();
	GPIO_InitPin(NRF_REG, CSN, GPIO_Mode_Out_PP | GPIO_Speed_50MHz);
	GPIO_InitPin(NRF_REG, CE, GPIO_Mode_Out_PP | GPIO_Speed_50MHz);
	NRF_CSN(HIGH);
	NRF_CE(LOW);

	//Reset Interrupt
	NRF_WriteByte(CONFIG, _BV(MASK_RX_DR) | _BV(MASK_TX_DS) | _BV(MASK_MAX_RT) | _BV(EN_CRC) | _BV(CRCO));
  //Reset Auto Acknowledge
	NRF_WriteByte(EN_AA, 0x00);
	//Enable Payloads
	NRF_WriteByte(EN_RXADDR, _BV(ERX_P0));
	//Set Address Size
	NRF_WriteByte(SETUP_AW, 0x03);
	//Set Retries
	NRF_WriteByte(SETUP_RETR, ((DELAY & 0xF)<<ARD | (COUNT & 0xF)<<ARC));
	//Set Channel to 0
  NRF_WriteByte(RF_CH, 0x00);
	//Set DataRate to RF24_250KPS
	NRF_WriteByte(RF_SETUP, (NRF_ReadByte(RF_SETUP) | _BV(RF_DR_LOW)) & ~_BV(RF_DR_HIGH));
	//Set PA to RF24_PA_MAX
	NRF_WriteByte(RF_SETUP, (NRF_ReadByte(RF_SETUP) & 0xF8) | RF24_PA_MAX<<RF_PWR);
  NRF_WritePage(TX_ADDR, NRF_Addr_G[1], ADDR_WIDTH);
  NRF_WritePage(RX_ADDR_P0, NRF_Addr_G[1], ADDR_WIDTH);
  NRF_WriteByte(RX_PW_P0, PAYLOAD_SIZE);
	NRF_WriteByte(FEATURE, 0x00);
  NRF_WriteByte(DYNPD, 0x00);

  NRF_WriteByte(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
  NRF_FlushTx();
  NRF_FlushRx();
}

void NRF_InitST2(void)
{
	SPI2_Init();
	GPIO_InitPin(NRF_REG, CSN, GPIO_Mode_Out_PP | GPIO_Speed_50MHz);
	GPIO_InitPin(NRF_REG, CE, GPIO_Mode_Out_PP | GPIO_Speed_50MHz);
	NRF_CSN(HIGH);
	NRF_CE(LOW);

	//Reset Interrupt
	NRF_WriteByte(CONFIG, _BV(MASK_RX_DR) | _BV(MASK_TX_DS) | _BV(MASK_MAX_RT) | _BV(EN_CRC) | _BV(CRCO));
  //Reset Auto Acknowledge
	NRF_WriteByte(EN_AA, 0x00);
	//Enable Payloads
	NRF_WriteByte(EN_RXADDR, _BV(ERX_P0));
	//Set Address Size
	NRF_WriteByte(SETUP_AW, 0x03);
	//Set Retries
	NRF_WriteByte(SETUP_RETR, ((DELAY & 0xF)<<ARD | (COUNT & 0xF)<<ARC));
	//Set Channel to 0
  NRF_WriteByte(RF_CH, 0x00);
	//Set DataRate to RF24_250KPS
	NRF_WriteByte(RF_SETUP, (NRF_ReadByte(RF_SETUP) | _BV(RF_DR_LOW)) & ~_BV(RF_DR_HIGH));
	//Set PA to RF24_PA_MAX
	NRF_WriteByte(RF_SETUP, (NRF_ReadByte(RF_SETUP) & 0xF8) | RF24_PA_MAX<<RF_PWR);
  NRF_WritePage(TX_ADDR, NRF_Addr_G[2], ADDR_WIDTH);
  NRF_WritePage(RX_ADDR_P0, NRF_Addr_G[2], ADDR_WIDTH);
  NRF_WriteByte(RX_PW_P0, PAYLOAD_SIZE);
	NRF_WriteByte(FEATURE, 0x00);
  NRF_WriteByte(DYNPD, 0x00);

  NRF_WriteByte(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
  NRF_FlushTx();
  NRF_FlushRx();
}

void NRF_InitTR1(void)
{
	SPI2_Init();
	GPIO_InitPin(NRF_REG, CSN, GPIO_Mode_Out_PP | GPIO_Speed_50MHz);
	GPIO_InitPin(NRF_REG, CE, GPIO_Mode_Out_PP | GPIO_Speed_50MHz);
	NRF_CSN(HIGH);
	NRF_CE(LOW);

	//Reset Interrupt
	NRF_WriteByte(CONFIG, _BV(MASK_RX_DR) | _BV(MASK_TX_DS) | _BV(MASK_MAX_RT) | _BV(EN_CRC) | _BV(CRCO));
  //Reset Auto Acknowledge
	NRF_WriteByte(EN_AA, 0x00);
	//Enable Payloads
	NRF_WriteByte(EN_RXADDR, _BV(ERX_P0));
	//Set Address Size
	NRF_WriteByte(SETUP_AW, 0x03);
	//Set Retries
	NRF_WriteByte(SETUP_RETR, ((DELAY & 0xF)<<ARD | (COUNT & 0xF)<<ARC));
	//Set Channel to 0
  NRF_WriteByte(RF_CH, 0x00);
	//Set DataRate to RF24_250KPS
	NRF_WriteByte(RF_SETUP, (NRF_ReadByte(RF_SETUP) | _BV(RF_DR_LOW)) & ~_BV(RF_DR_HIGH));
	//Set PA to RF24_PA_MAX
	NRF_WriteByte(RF_SETUP, (NRF_ReadByte(RF_SETUP) & 0xF8) | RF24_PA_MAX<<RF_PWR);
  NRF_WritePage(TX_ADDR, NRF_Addr_G[3], ADDR_WIDTH);
  NRF_WritePage(RX_ADDR_P0, NRF_Addr_G[3], ADDR_WIDTH);
  NRF_WriteByte(RX_PW_P0, PAYLOAD_SIZE);
	NRF_WriteByte(FEATURE, 0x00);
  NRF_WriteByte(DYNPD, 0x00);

  NRF_WriteByte(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
  NRF_FlushTx();
  NRF_FlushRx();
}

void NRF_InitTR2(void)
{
	SPI2_Init();
	GPIO_InitPin(NRF_REG, CSN, GPIO_Mode_Out_PP | GPIO_Speed_50MHz);
	GPIO_InitPin(NRF_REG, CE, GPIO_Mode_Out_PP | GPIO_Speed_50MHz);
	NRF_CSN(HIGH);
	NRF_CE(LOW);

	//Reset Interrupt
	NRF_WriteByte(CONFIG, _BV(MASK_RX_DR) | _BV(MASK_TX_DS) | _BV(MASK_MAX_RT) | _BV(EN_CRC) | _BV(CRCO));
  //Reset Auto Acknowledge
	NRF_WriteByte(EN_AA, 0x00);
	//Enable Payloads
	NRF_WriteByte(EN_RXADDR, _BV(ERX_P0));
	//Set Address Size
	NRF_WriteByte(SETUP_AW, 0x03);
	//Set Retries
	NRF_WriteByte(SETUP_RETR, ((DELAY & 0xF)<<ARD | (COUNT & 0xF)<<ARC));
	//Set Channel to 0
  NRF_WriteByte(RF_CH, 0x00);
	//Set DataRate to RF24_250KPS
	NRF_WriteByte(RF_SETUP, (NRF_ReadByte(RF_SETUP) | _BV(RF_DR_LOW)) & ~_BV(RF_DR_HIGH));
	//Set PA to RF24_PA_MAX
	NRF_WriteByte(RF_SETUP, (NRF_ReadByte(RF_SETUP) & 0xF8) | RF24_PA_MAX<<RF_PWR);
  NRF_WritePage(TX_ADDR, NRF_Addr_G[4], ADDR_WIDTH);
  NRF_WritePage(RX_ADDR_P0, NRF_Addr_G[4], ADDR_WIDTH);
  NRF_WriteByte(RX_PW_P0, PAYLOAD_SIZE);
	NRF_WriteByte(FEATURE, 0x00);
  NRF_WriteByte(DYNPD, 0x00);

  NRF_WriteByte(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
  NRF_FlushTx();
  NRF_FlushRx();
}

void NRF_Update(void)
{
	switch(NRF_State_G)
	{
		case 0:
			NRF_StartRxMode();
			NRF_State_G = 1;
			break;
		case 1:
			if(NRF_CheckRxFlag())
			{
				NRF_Receive(NRF_RxBuff_G);
				NRF_State_G = 2;
			}
			break;
		case 2:
			NRF_StopRxMode();
			NRF_StartTxMode();
			NRF_State_G = 3;
			break;
		case 3:
			NRF_Transmit(NRF_TxBuff_G);
			NRF_State_G = 4;
			break;
		case 4:
			if(NRF_CheckTxFlag())
			{
				NRF_State_G = 0;
			}
			break;
	}
}

void NRF_SetData(volatile uint8_t Index, volatile uint8_t Buff)
{
	NRF_TxBuff_G[Index] = Buff;
}

uint8_t NRF_GetData(volatile uint8_t Index)
{
	return NRF_RxBuff_G[Index];
}

void NRF_ClearTxBuff(void)
{
	uint8_t Index;
	for(Index=0; Index<PAYLOAD_SIZE; Index++)
	{
		NRF_TxBuff_G[Index] = 0;
	}
}

void NRF_ClearRxBuff(void)
{
	uint8_t Index;
	for(Index=0; Index<PAYLOAD_SIZE; Index++)
	{
		NRF_RxBuff_G[Index] = 0;
	}
}
