#include "IR.h"

volatile static uint8_t IR_State_G[3] = {0};

void IR_Init(void)
{
	GPIO_InitPin(IR_REG, IR0, GPIO_Mode_IN_FLOATING | GPIO_Input);
	GPIO_InitPin(IR_REG, IR1, GPIO_Mode_IN_FLOATING | GPIO_Input);
	GPIO_InitPin(IR_REG, IR2, GPIO_Mode_IN_FLOATING | GPIO_Input);
}

void IR_Update(void)
{
	IR_State_G[0] = GPIO_GetPinState(IR_REG, IR0);
	IR_State_G[1] = GPIO_GetPinState(IR_REG, IR1);
	IR_State_G[2] = GPIO_GetPinState(IR_REG, IR2);
}

uint8_t IR_GetState(uint8_t IRx)
{
	uint8_t State = 0;
	switch(IRx)
	{
		case IR0:
			State = IR_State_G[0];
			break;
		case IR1:
			State = IR_State_G[1];
			break;
		case IR2:
			State = IR_State_G[2];
			break;
	}
	return State;
}
