#include "stm32f10x.h"
#include "sys.h"
#include "mbotLinuxUsart.h"
#include "pid.h"	

u8 Flag_Qian     =0;
u8 Flag_Hou      =0;
u8 Flag_Left     =0;
u8 Flag_Right    =0;
u8 Flag_sudu     =2;
u8 Flag_useApp   =0;    //����ң����صı���
u8 Flag_Stop     =1;    //ֹͣ��־λ��Ĭ��ֹͣ
u8 Flag_Show     =0;    //��ʾ��־λ  ��ʾ��

int motorLeft   =0;
int motorRight  =0;    //���PWM����	

int Voltage     =0;    //��ص�ѹ������صı���
float yaw  =0;         //ת��������

u8 delayValue   =0;
u8 delayFlag    =0;    //��ʱ�͵��εȱ��� 

int main(void)
{ 
	delay_init();	    	            //=====��ʱ������ʼ��	
	uart_init(115200);	            //=====���ڳ�ʼ��Ϊ  ��ݮ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//����JTAG ���� SWD
	LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
	MY_NVIC_PriorityGroupConfig(2);	//=====�����жϷ���
	MBOT_PWM_Init(7199,0);          //=====��ʼ��PWM 10KHZ������������� �����ʼ������ӿ� 
	uart3_init(9600);               //=====����3��ʼ��
	Encoder_Init_TIM2();            //=====�������ӿ�
	Encoder_Init_TIM4();            //=====��ʼ��������2
	Adc_Init();                     //=====adc��ʼ��
	IIC_Init();                     //=====IIC��ʼ��   ��ȡMPU6050����
	MPU6050_initialize();           //=====MPU6050��ʼ��	
	DMP_Init();                     //=====��ʼ��DMP 
	PID_Init();                     //=====PID��ʼ��
	KEY_Init();
	MBOT_EXTI_Init();               //=====MPU6050 5ms��ʱ�жϳ�ʼ��

	while(1)
	{
		//��������ʱ�ã�������ע��
		pcShow();
		//����ݮ�ɷ����ٶȣ��Ƕ�,�����ٶ��Ѿ�����1000
		usartSendSpeed((float)leftSpeedNow,(float)rightSpeedNow,(float)(int)yaw);
		delayFlag=1;	
		delayValue=0;
		while(delayFlag);	        //ͨ��MPU6050��INT�ж�ʵ�ֵ�15ms��׼��ʱ	
	} 
}

