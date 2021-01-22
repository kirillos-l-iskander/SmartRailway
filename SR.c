#include "SR.h"

volatile static uint8_t SR_G = 0;
volatile static uint8_t SR_Index_G = 0;

volatile static uint8_t SR_State_G = 0;
volatile static uint8_t SR_TxBuff_G[5][5] = {{0}};
volatile static uint8_t SR_RxBuff_G[5][5] = {{0}};
volatile static uint8_t SR_Addr_G[5][5] = {	{0xF0, 0xF0, 0xF0, 0xF0, 0x00},
																						{0xF0, 0xF0, 0xF0, 0xF0, 0x01},
																						{0xF0, 0xF0, 0xF0, 0xF0, 0x02},
																						{0xF0, 0xF0, 0xF0, 0xF0, 0x03},
																						{0xF0, 0xF0, 0xF0, 0xF0, 0x04}};

void SR_Update(void)
{
	switch(SR_State_G)
	{
		case 0:

			++SR_Index_G;
			if(SR_Index_G > 4)
			{
				SR_Index_G = 1;
			}
			NRF_OpenWritingPipe(SR_Addr_G[SR_Index_G]);
			NRF_StartTxMode();
			SR_State_G = 1;
			break;
		case 1:
			NRF_Transmit(SR_TxBuff_G[SR_Index_G]);
			SR_State_G = 2;
			break;
		case 2:
			if(!NRF_CheckTxFlag())
			{
				return;
			}
			NRF_StartRxMode();
			SR_State_G = 3;
			break;
		case 3:
			if(!NRF_CheckRxFlag())
			{
				return;
			}
			NRF_Receive(SR_RxBuff_G[SR_Index_G]);
			NRF_StopRxMode();
			SR_State_G = 4;
			break;
		case 4:
			for(uint8_t i = 1; i<5; i++)
			{
				SR_TxBuff_G[SR_RxBuff_G[i][0]][0] = SR_RxBuff_G[i][0];
				SR_TxBuff_G[SR_RxBuff_G[i][0]][1] = SR_RxBuff_G[i][1];
				SR_TxBuff_G[SR_RxBuff_G[i][0]][2] = SR_RxBuff_G[i][2];
				SR_TxBuff_G[SR_RxBuff_G[i][0]][3] = SR_RxBuff_G[i][3];
				SR_TxBuff_G[SR_RxBuff_G[i][0]][4] = SR_RxBuff_G[i][4];
			}
			SR_State_G = 0;
			SR_G^=1;
			if(SR_G)
			{
				LED_SetState(LED0, HIGH);
			}else
			{
				LED_SetState(LED0, LOW);
			}
			break;
	}
}
