#include "ST.h"

#define ST_CURRENT_ADDRESS 2
#define ST_NEXT_ADDRESS 1

volatile static uint8_t ST_State_G = 0;
volatile static uint8_t ST_Addr_G = 4;
volatile static uint8_t ST_Buff_G = 0;
volatile static uint16_t ST_Empty_G = 1;
volatile static uint16_t ST_Wait_G = 0;

volatile static uint8_t IR0_State = 0;
volatile static uint8_t IR1_State = 0;
volatile static uint8_t IR2_State = 0;

void ST_StartState(void);
void ST_GetTrainAddr(void);
void ST_DetectTrainEnter(void);
void ST_WaitStationEmpty(void);
void ST_DetectTrainReach(void);
void ST_ServeTrain(void);

void ST_Update(void)
{
	/*
	LCD_SetCharacterBuffer((NRF_GetData(0)%10)+'0', 0, 0);
	LCD_SetCharacterBuffer((NRF_GetData(1)%10)+'0', 0, 1);
	LCD_SetCharacterBuffer((NRF_GetData(2)%10)+'0', 0, 2);
	return;
	*/
	
	IR0_State = !IR_GetState(IR0);
	IR1_State = !IR_GetState(IR1);
	IR2_State = !IR_GetState(IR2);
	
	if(IR2_State)
	{
		ST_Empty_G = 1;
		LCD_ClearBuffer();
	}

	switch(ST_State_G)
	{
		case 0:
			ST_StartState();
			break;
		case 1:
			ST_GetTrainAddr();
			break;
		case 2:
			ST_DetectTrainEnter();
			break;
		case 3:
			ST_WaitStationEmpty();
			break;
		case 4:
			ST_DetectTrainReach();
			break;
		case 5:
			ST_ServeTrain();
			break;
	}
}

void ST_StartState(void)
{
	ST_Buff_G = 15;
	NRF_SetData(0, ST_Addr_G);
	NRF_SetData(1, ST_CURRENT_ADDRESS);
	NRF_SetData(2, ST_Buff_G);
	ST_State_G = 2;
}

void ST_GetTrainAddr(void)
{
	if(!NRF_GetData(0))
	{
		return;
	}
	ST_Addr_G = NRF_GetData(2);
	ST_Buff_G = 15;
	NRF_SetData(0, ST_Addr_G);
	NRF_SetData(1, ST_CURRENT_ADDRESS);
	NRF_SetData(2, ST_Buff_G);
	NRF_ClearRxBuff();
	ST_State_G = 2;
}

void ST_DetectTrainEnter(void)
{
	if(!IR0_State)
	{
		return;
	}
	if(ST_Empty_G)
	{
		ST_Empty_G = 0;
		ST_Buff_G = 10;
		ST_State_G = 4;
		LCD_SetStringBuffer((uint8_t*)"Train:", 0, 0);
		LCD_SetCharacterBuffer(((ST_Addr_G/1)%10)+'0', 0, 6);
		LCD_SetStringBuffer((uint8_t*)"<<", 0, 14);
		LCD_SetStringBuffer((uint8_t*)"Speed:", 1, 0);
		LCD_SetCharacterBuffer(((ST_Buff_G/10)%10)+'0', 1, 6);
		LCD_SetCharacterBuffer(((ST_Buff_G/1)%10)+'0', 1, 7);
		LCD_SetStringBuffer((uint8_t*)"cm/s", 1, 8);
	}else
	{
		ST_Buff_G = 0;
		ST_State_G = 3;
	}
	NRF_SetData(0, ST_Addr_G);
	NRF_SetData(1, ST_CURRENT_ADDRESS);
	NRF_SetData(2, ST_Buff_G);
}

void ST_WaitStationEmpty(void)
{
	if(!ST_Empty_G)
	{
		return;
	}
	ST_State_G = 2;
}

void ST_DetectTrainReach(void)
{
	if(!IR1_State)
	{
		return;
	}
	ST_Buff_G = 0;
	NRF_SetData(0, ST_Addr_G);
	NRF_SetData(1, ST_CURRENT_ADDRESS);
	NRF_SetData(2, ST_Buff_G);
	ST_Wait_G = 60000/100;
	LCD_SetStringBuffer((uint8_t*)"Train:", 0, 0);
	LCD_SetCharacterBuffer(((ST_Addr_G/1)%10)+'0', 0, 6);
	LCD_SetStringBuffer((uint8_t*)"XX", 0, 14);
	LCD_SetStringBuffer((uint8_t*)"Speed:", 1, 0);
	LCD_SetCharacterBuffer(((ST_Buff_G/10)%10)+'0', 1, 6);
	LCD_SetCharacterBuffer(((ST_Buff_G/1)%10)+'0', 1, 7);
	LCD_SetStringBuffer((uint8_t*)"cm/s", 1, 8);
	ST_State_G = 5;
}

void ST_ServeTrain(void)
{
	if(ST_Wait_G)
	{
		ST_Wait_G--;
		if(IR2_State)
		{
			//Emergency
			LCD_ClearBuffer();
			LCD_SetStringBuffer((uint8_t*)"Warning", 0, 0);
			LCD_SetStringBuffer((uint8_t*)"Code:01", 1, 0);
		}
		LCD_SetStringBuffer((uint8_t*)"XX", 0, 14);
		LCD_SetCharacterBuffer((((ST_Wait_G*100/1000)/10)%10)+'0', 1, 14);
		LCD_SetCharacterBuffer((((ST_Wait_G*100/1000)/1)%10)+'0', 1, 15);
	}else
	{
		//Communicate with next station
		NRF_SetData(0, ST_NEXT_ADDRESS);
		NRF_SetData(1, ST_CURRENT_ADDRESS);
		NRF_SetData(2, ST_Addr_G);
		LCD_SetStringBuffer((uint8_t*)"Train:", 0, 0);
		LCD_SetCharacterBuffer(((ST_Addr_G/1)%10)+'0', 0, 6);
		LCD_SetStringBuffer((uint8_t*)"<-", 0, 14);
		LCD_SetStringBuffer((uint8_t*)"Speed:", 1, 0);
		LCD_SetCharacterBuffer(((ST_Buff_G/10)%10)+'0', 1, 6);
		LCD_SetCharacterBuffer(((ST_Buff_G/1)%10)+'0', 1, 7);
		LCD_SetStringBuffer((uint8_t*)"cm/s", 1, 8);
		LCD_SetCharacterBuffer((((ST_Wait_G*100/1000)/10)%10)+'0', 1, 14);
		LCD_SetCharacterBuffer((((ST_Wait_G*100/1000)/1)%10)+'0', 1, 15);
		ST_State_G = 1;
	}
}
