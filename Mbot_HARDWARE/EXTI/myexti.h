
#ifndef __MYEXTI_H__
#define __MYEXTI_H__	 

#include "stm32f10x.h"

#define KEY GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)

void MBOT_EXTI_Init(void);	//外部中断初始化	
void EXTI2_Init(void);
//void EXTI2_IRQHandler(void);

#endif

