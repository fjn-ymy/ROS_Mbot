#include "control.h"	
#include "pid.h"
#include "MPU6050.h"
#include "motor.h"
#include "adc.h"
#include "encoder.h"
#include "key.h"
#include "led.h"
#include "usart3.h"

int leftWheelEncoder       = 0;
int rightWheelEncoder      = 0;

int leftWheelEncoderNow    = 0;
int rightWheelEncoderNow   = 0;
int leftWheelEncoderLast   = 0;
int rightWheelEncoderLast  = 0;


int Voltage     = 0;    //��ص�ѹ������صı���
int Voltage_Temp,Voltage_Count,Voltage_All;

float yaw  =0;         //ת��������

u8 Flag_Qian     =0;
u8 Flag_Hou      =0;
u8 Flag_Left     =0;
u8 Flag_Right    =0;
u8 Flag_sudu     =2;
u8 Flag_useApp   =0;    //����ң����صı���

/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
          5ms��ʱ�ж���MPU6050��INT���Ŵ���
          �ϸ�֤���������ݴ����ʱ��ͬ��		
		//�������ҳ������ٶȣ������ٶ�Ϊ��ֵ �������ٶ�Ϊ��ֵ���ٶ�Ϊ����1000֮����ٶ� mm/s
		//һ��ʱ���ڵı������仯ֵ*ת���ʣ�ת��Ϊֱ���ϵľ���m��*100s��10ms����һ�Σ� �õ� m/s *1000ת��Ϊint����

		һȦ����������
			��1560
			�ң�1040
		���Ӱ뾶��0.03m
		�����ܳ���2*pi*r
		һ������ľ��룺
			��0.000120830m
			�ң�0.000181245m
		�ٶȷֱ��ʣ�
			��0.0120m/s 12.0mm/s
			�ң�0.0181m/s 18.1mm/s
						
**************************************************************************/
int EXTI15_10_IRQHandler(void) 
{                                                         
	EXTI_ClearITPendingBit(EXTI_Line12);  //���LINE12��·����λ		
	
	leftWheelEncoder += getTIMx_DetaCnt(TIM4);
	rightWheelEncoder+= getTIMx_DetaCnt(TIM2);
	
	//��¼�������ұ���������
	leftWheelEncoderNow   = leftWheelEncoder;
	rightWheelEncoderNow  = rightWheelEncoder;
		
	//10ms����    	
	leftSpeedNow          = (leftWheelEncoderNow - leftWheelEncoderLast)*1000*100*0.000120830;  //
	rightSpeedNow         = (rightWheelEncoderNow - rightWheelEncoderLast)*1000*100*0.000181245;//

	//��¼�ϴα���������
	leftWheelEncoderLast  = leftWheelEncoderNow;                    //===��ȡ��������ֵ
	rightWheelEncoderLast = rightWheelEncoderNow;                   //===��ȡ��������ֵ
	
//	Voltage_Temp=Get_battery_volt();		                            //=====��ȡ��ص�ѹ		
//	Voltage_Count++;                                                    //=====ƽ��ֵ������
//	Voltage_All+=Voltage_Temp;                                          //=====��β����ۻ�
//	if(Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//=====��ƽ��ֵ	

//	key();
//	Led_Flash(200);                                                 //===LED��˸;����ģʽ 1s�ı�һ��ָʾ�Ƶ�״̬		
			
	App_main();
	Pid_Ctrl();
	Set_Pwm(motorLeft,motorRight); 
	
//	if(Turn_Off(Voltage)==0)                                         //===�����ѹ�쳣
//	{
//		Set_Pwm(motorLeft,motorRight);                                 //===��ֵ��PWM�Ĵ��� 		
//	}
//	else                                                             //���ﲻ�������͹�pid��Ϊ������adc��û�ɼ�����ֵ
//	{
//		motorLeft=0;
//		motorRight=0;
//		AIN1=0;                                            
//		AIN2=0;
//		BIN1=0;
//		BIN2=0;
//	}   	
	return 0;	  
} 






