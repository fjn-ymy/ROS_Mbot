#include "myexti.h"
#include "delay.h"
#include "usart3.h"

/**************************************************************************
函数功能：外部中断初始化
入口参数：无 PA12 上拉输入 中断线配置 触发方式 中断通道
返回  值：无 
**************************************************************************/
void MBOT_EXTI_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);          //外部中断，需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);        //使能GPIO端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	                 //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					     //根据设定参数初始化GPIO
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;      //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	                         //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		 //使能外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		 //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			     //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
}



//外部中断初始化函数
void EXTI2_Init(void)
{
		GPIO_InitTypeDef GPIO2_InitStructure; //定义一个引脚初始化的类
 	  EXTI_InitTypeDef EXTI_InitStructure; //定义一个外部中断初始化的结构体
 	  NVIC_InitTypeDef NVIC_InitStructure; //定义一个中断优先级初始化的结构体

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断需要使能AFIO时钟

	  //PA2初始化

		GPIO2_InitStructure.GPIO_Pin=GPIO_Pin_2; //定义为引脚5
		GPIO2_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA时钟，在STM32中使用IO口前都要使能对应时钟
		GPIO_Init(GPIOA, &GPIO2_InitStructure); //初始化引脚GPIOA5。GPIOA代表GPIOA，&GPIO5_InitStructure代表要初始化的引脚号和引脚状态


  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2); //该函数将GPIO端口与中断线映射起来

  	EXTI_InitStructure.EXTI_Line=EXTI_Line2; //外部中断线5
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//外部中断线0-15为产生中断
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //使能
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			  //属于外部中断5-9
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//响应优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//中断使能
  	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStrue的参数初始化VIC寄存器
  
}

//void EXTI2_IRQHandler(void)
//{
//  //delay_ms(10);    //防抖
//	 if(KEY==1)       //防抖
//	{	  
//			hongwaiFlag=1;
//		 Flag_useApp=1;
//		 Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
//			hongwaiCount=0;	
//		printf("接收到红外信号\r\n");
//	}
//	EXTI_ClearITPendingBit(EXTI_Line2);  //清除EXTI5线路中断标志
//}






