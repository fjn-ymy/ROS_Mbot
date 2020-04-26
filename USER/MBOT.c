#include "stm32f10x.h"
#include "sys.h"
#include "mbotLinuxUsart.h"
#include "pid.h"	

u8 Way_Angle     =1;    //��ȡ�Ƕȵ��㷨��1����Ԫ��
u8 Flag_Qian     =0;
u8 Flag_Hou      =0;
u8 Flag_Left     =0;
u8 Flag_Right    =0;
u8 Flag_sudu     =2;
u8 Flag_useApp   =0;    //����ң����صı���
u8 Flag_Stop     =1;    //ֹͣ��־λ��Ĭ��ֹͣ
u8 Flag_Show     =0;    //��ʾ��־λ  ��ʾ��

int Encoder_Left =0;
int Encoder_Right=0;    //���ұ��������������
int Moto1        =0;
int Moto2        =0;    //���PWM����	

int Voltage      =0;    //��ص�ѹ������صı���
float Gyro_Turn  =0;    //ת��������

u8 delay_value   =0;
u8 delay_flag    =0;    //��ʱ�͵��εȱ��� 

int main(void)
{ 
	delay_init();	    	        //=====��ʱ������ʼ��	
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
		//APP_Show();
		//����ݮ�ɷ����ٶȣ��Ƕ�,�����ٶ��Ѿ�����1000
		usartSendSpeed((float)leftSpeedNow,(float)rightSpeedNow,(float)(int)Gyro_Turn);
		delay_flag=1;	
		delay_value=0;
		while(delay_flag);	        //ͨ��MPU6050��INT�ж�ʵ�ֵ�15ms��׼��ʱ	
	} 
}

