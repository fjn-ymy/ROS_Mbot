#include "sys.h"

//====================�Լ������ͷ�ļ�===============================
#include "delay.h"
#include "led.h"
#include "myexti.h"
#include "adc.h"
#include "pwm.h"
#include "encoder.h"
#include "usart1.h"	
#include "usart3.h"
#include "ioi2c.h"
#include "mpu6050.h"

#include "show.h"					
#include "mbotLinuxUsart.h"
#include "pid.h"
#include "control.h"
#include "motor.h"

extern float q0,q1,q2,q3;	
unsigned char buf[]="A1,F1,11,22,33";
//extern unsigned char sendCtrlFlag;
char hongwaiFlag=0;		//�����־λ
int hongwaiCount=0;		//�������
/*===================================================================
�����ܣ�ROSС���ײ���루ȫ����
�����д�����ںţ�С��ѧ�ƶ�������
����    ������Դ������κ����ʣ�����˽��С�࣬һ����ظ��ġ�
=====================================================================
------------------��ע���ںţ���ø�����Ȥ�ķ���---------------------
===================================================================*/
int main(void)
{ 
	//���ͼ���
	char sendCount=0;
	
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//����JTAG ���� SWD
	
	MY_NVIC_PriorityGroupConfig(2);	//=====�����жϷ���
	
	delay_init();	    	        //=====��ʱ������ʼ��
	
	LED_Init();                     //=====LED��ʼ��    �����	
	
	usart1_init(115200);	        //=====����1��ʼ��  ��ݮ��
	usart2_init(9600);						//======����2 ���պ��ⴥ������
	usart3_init(9600);              //=====����3��ʼ��  ����

	IIC_Init();                     //=====IIC��ʼ��    ��ȡMPU6050����
	MPU6050_initialize();           //=====MPU6050��ʼ��	
	DMP_Init();                     //=====��ʼ��DMP 

	Encoder_Init_TIM2();            //=====��ʼ��������1�ӿ�
	Encoder_Init_TIM4();            //=====��ʼ��������2�ӿ�
	
	MY_ADC_Init();                  //=====adc��ʼ��    ��ص������
	
	Motor_Init(7199,0);             //=====��ʼ��PWM 10KHZ������������� �����ʼ���������ӿ�
	PID_Init();                     //=====PID��ʼ��
	
	MBOT_EXTI_Init();               //=====MPU6050 5ms��ʱ�жϳ�ʼ��
	
	
	 EXTI2_Init(); //��ʼ���ⲿ�ж�
	
	while(1)
	{
		//USART_Send_String(USART2, buf, sizeof(buf));
		//����ݮ�ɷ����ٶȣ��Ƕ�,�����ٶ��Ѿ�����1000
		if(sendCount==0)//����  14.4ms  ����һ������ 70Hz ����
		{
			//������Ҫһ������ʱ
			usartSendData(USART1,(short)leftSpeedNow,(short)rightSpeedNow,(short)yaw,(short)Voltage,(float)q1,(float)q2,(float)q3,(float)q0,sendCtrlFlag);  //1ms
			//��������ʱ�ã�������ע��
			//pcShow();                                                           //2.2ms����float����
			sendCount++;
		}
		else
		{
			sendCount++;
			if(sendCount==25)
				sendCount=0;
		}
		//��ȡ�Ƕ�		
		getAngle(&yaw,&yaw_acc_error);         
		
		//************Begin ���⴦���� Begin**************
		hongwaiCount++;
		if(hongwaiCount>=500&&hongwaiFlag==1)	//С��û�н��յ������źţ�ֹͣ�˶�
		{
		 Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;		//С��ֹͣ
			if(hongwaiCount>2000)
			{
							hongwaiFlag=2;			//�����ź�  С����ת
			}
		}
		if(hongwaiFlag==2)		//
		{
			if(hongwaiCount>=20000)																//��ת��Ȧ֮�� С��ֹͣ
			{
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
							if(hongwaiCount==20100)
							{
								Flag_useApp=0;
								hongwaiFlag=0;
							}
			}
			
			if(hongwaiCount%3000>=2000)
			{
				Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;			//С����ת
			}
			else
					Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;			//С����ת
			

		}
		//************ END ���⴦���� END**************
	} 
}

//�жϷ�����
void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
 	 {
		 USART_ClearITPendingBit(USART1,USART_IT_RXNE);//��������жϱ�־λ
		 usartReceiveOneData(USART1,&leftSpeedSet,&rightSpeedSet,&receCtrlFlag);
	 }
 
}



/**************************************************************************
�������ܣ�����2�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/


void USART2_IRQHandler()
{
//	static	int uart_receive=0;                           //������յ����ź�
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
 	 {
		 USART_ClearITPendingBit(USART2,USART_IT_RXNE);//��������жϱ�־λ

		 hongwaiFlag=1;
		 Flag_useApp=1;
		 Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
			hongwaiCount=0;		 
	 }
 
}

void EXTI2_IRQHandler(void)
{
  delay_ms(10);    //����
	 if(KEY==1)       //����
	{	  
			hongwaiFlag=1;
		 Flag_useApp=1;
		 Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
			hongwaiCount=0;	
		printf("���յ������ź�\r\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line2);  //���EXTI5��·�жϱ�־
}

/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void USART3_IRQHandler(void)
{	
	static	int uart_receive=0;                           //����������ر���
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{	
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);    //����жϱ�־λ	���ڶ�ȡDR�Ĵ����Ժ��Ӳ������жϱ�־λ	
		uart_receive=USART_ReceiveData(USART3); 
		
		if(uart_receive==0x4C)//ģʽ�л�L
		{
			sendCtrlFlag=1;
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
			PID_DIS_Init();
		}	
		if(uart_receive==0x4D)//M
		{
			sendCtrlFlag=2;
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
		}
		if(uart_receive==0x4E)//N
		{
			sendCtrlFlag=3;
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
		}
		if(uart_receive==0x4F)//O
		{
			sendCtrlFlag=4;
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
		}
		
		//////*********************
		if(uart_receive==0x50)//P
		{
			sendCtrlFlag=5;
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
			
		}
		if(uart_receive==0x51)//Q
		{
			sendCtrlFlag=6;
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
		}
		if(uart_receive==0x52)//R
		{
			sendCtrlFlag=7;
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;	
		}
		if(uart_receive==0x53)//S
		{
			sendCtrlFlag=8;
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
		}
		if(uart_receive==0x54)//T
		{
			sendCtrlFlag=9;
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
		}
		if(uart_receive==0x55)//U
		{
			sendCtrlFlag=0;
			Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
			PID_Init();
		}
///////////////************************************************
		
		if(uart_receive==0x4A)//�ٶ��л�
		{
			if(Flag_sudu==2) //���ٵ���Ĭ��ֵ��
				Flag_sudu=1;   //���ٵ�
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
