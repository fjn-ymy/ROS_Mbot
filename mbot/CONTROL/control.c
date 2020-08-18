#include "control.h"	
#include "pid.h"
#include "motor.h"
#include "adc.h"
#include "encoder.h"
#include "led.h"

int   Voltage    =0;       //��ص�ѹ������صı���
float yaw        =0;       //ת��������

/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
          5ms��ʱ�ж���MPU6050��INT���Ŵ���		
**************************************************************************/
void EXTI15_10_IRQHandler(void) 
{                                                         
	EXTI_ClearITPendingBit(EXTI_Line12);                            //===���LINE12��·����λ		
	
	Led_Flash(200);                                                 //===LED��˸��֤��������������	
	
	Get_battery_volt_average(&Voltage,100);		                    //===��ص�ѹ��������λmv��100��ȡһ��ƽ��
	
	Get_Motor_Speed(&leftSpeedNow,&rightSpeedNow);                  //===��ȡ����������ʵ�ٶ�
	
	App_main();                                                     //===�ֻ�app����
	
	Pid_Ctrl();                                                     //===�����ٶ�PID����

	if(Turn_Off(Voltage)==0)                                        //===�����ѹ�쳣
	{
		Set_Pwm(motorLeft,motorRight);                              //===��ֵ��PWM�Ĵ��� 		
	} 	
} 








