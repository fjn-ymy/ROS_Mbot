#include "myexti.h"
#include "delay.h"
#include "usart3.h"

/**************************************************************************
�������ܣ��ⲿ�жϳ�ʼ��
��ڲ������� PA12 �������� �ж������� ������ʽ �ж�ͨ��
����  ֵ���� 
**************************************************************************/
void MBOT_EXTI_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);          //�ⲿ�жϣ���Ҫʹ��AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);        //ʹ��GPIO�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	                 //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					     //�����趨������ʼ��GPIO
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;      //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	                         //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		 //ʹ���ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		 //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			     //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 
}



//�ⲿ�жϳ�ʼ������
void EXTI2_Init(void)
{
		GPIO_InitTypeDef GPIO2_InitStructure; //����һ�����ų�ʼ������
 	  EXTI_InitTypeDef EXTI_InitStructure; //����һ���ⲿ�жϳ�ʼ���Ľṹ��
 	  NVIC_InitTypeDef NVIC_InitStructure; //����һ���ж����ȼ���ʼ���Ľṹ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�ж���Ҫʹ��AFIOʱ��

	  //PA2��ʼ��

		GPIO2_InitStructure.GPIO_Pin=GPIO_Pin_2; //����Ϊ����5
		GPIO2_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ�ӣ���STM32��ʹ��IO��ǰ��Ҫʹ�ܶ�Ӧʱ��
		GPIO_Init(GPIOA, &GPIO2_InitStructure); //��ʼ������GPIOA5��GPIOA����GPIOA��&GPIO5_InitStructure����Ҫ��ʼ�������źź�����״̬


  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2); //�ú�����GPIO�˿����ж���ӳ������

  	EXTI_InitStructure.EXTI_Line=EXTI_Line2; //�ⲿ�ж���5
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//�ⲿ�ж���0-15Ϊ�����ж�
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //ʹ��
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			  //�����ⲿ�ж�5-9
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//��Ӧ���ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//�ж�ʹ��
  	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStrue�Ĳ�����ʼ��VIC�Ĵ���
  
}

//void EXTI2_IRQHandler(void)
//{
//  //delay_ms(10);    //����
//	 if(KEY==1)       //����
//	{	  
//			hongwaiFlag=1;
//		 Flag_useApp=1;
//		 Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
//			hongwaiCount=0;	
//		printf("���յ������ź�\r\n");
//	}
//	EXTI_ClearITPendingBit(EXTI_Line2);  //���EXTI5��·�жϱ�־
//}






