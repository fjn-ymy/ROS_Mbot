#include "usart3.h"

u8 Usart3_Receive;

/**************************************************************************
�������ܣ�����3��ʼ��
��ڲ����� bound:������
����  ֵ����
**************************************************************************/
void uart3_init(u32 bound)
{  	 
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	   //ʹ��UGPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	   //ʹ��USART3ʱ��
	//USART3_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	           //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USART3_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;      //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;   //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		   //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                           //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;                //���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;        //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure);                  //��ʼ������3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);             //�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                                 //ʹ�ܴ���3 
}

/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{	
		//USART_ClearITPendingBit(USART3,USART_IT_RXNE);  //����жϱ�־λ	���ڶ�ȡDR�Ĵ����Ժ��Ӳ������жϱ�־λ	
		static	int uart_receive=0;                       //����������ر���
		uart_receive=USART_ReceiveData(USART3); 

		if(uart_receive==0x4A)//�ٶ��л�
		{
			if(Flag_sudu==2) //���ٵ���Ĭ��ֵ��
				Flag_sudu=1; //���ٵ�
			else
				Flag_sudu=2;  
		}	
		
		if(uart_receive==0x4B)
		{
			if(Flag_useApp==0)//Ĭ�ϲ�ʹ��APP
				Flag_useApp=1;
			else
				Flag_useApp=0;
		}
		
		if(uart_receive>10)  //Ĭ��ʹ��
		{			
			if(uart_receive==0x5A)	
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ɲ��
			else if(uart_receive==0x41||uart_receive==0x42||uart_receive==0x48)	
				Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ǰ
			else if(uart_receive==0x44||uart_receive==0x45||uart_receive==0x46)	
				Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////��
			else if(uart_receive==0x43)	
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;//////////////��
			else if(uart_receive==0x47)
				Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;//////////////��
			else 
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ɲ��
		}
	}  											 
} 



