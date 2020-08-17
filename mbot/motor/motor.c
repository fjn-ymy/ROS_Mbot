#include "motor.h"

int motorLeft   =0;
int motorRight  =0;         //�����ֵ��PWM����

//����1000֮����ٶ�ʵʱֵ
int leftSpeedNow  = 0; 
int rightSpeedNow = 0; 
//����1000֮����ٶ��趨ֵ
int leftSpeedSet  = 0; 
int rightSpeedSet = 0; 

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motorLeft,int motorRight)
{
	if(motorLeft>0)     AIN2=0, AIN1=1;
	else 	            AIN2=1,	AIN1=0;
	PWMA=myabs(motorLeft);
	if(motorRight>0)	BIN1=0,	BIN2=1;
	else                BIN1=1,	BIN2=0;
	PWMB=myabs(motorRight);	
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
u8 Turn_Off(int voltage)
{
	u8 temp=0;
	if(voltage<1110)//��ص�ѹ����11.1V�رյ��
	{	                                             
		temp=1;                                        
		AIN1=0;                                            
		AIN2=0;
		BIN1=0;
		BIN2=0;
	}
	else
		temp=0;
	return temp;			
}

/**************************************************************************
�������ܣ�APPָ���ж�
��ڲ�����
����  ֵ��
Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0
**************************************************************************/
void App_main()
{
	if(Flag_useApp==1)
	{
		if(Flag_Qian==1)
		{
			if(Flag_sudu==2)//Ĭ�ϵ���
			{
				leftSpeedSet  = 200; 
				rightSpeedSet = 200; 			
			}
			else if(Flag_sudu==1)//����
			{
				leftSpeedSet  = 350; 
				rightSpeedSet = 350; 			
			}		
		}
		else if(Flag_Hou==1)
		{

			if(Flag_sudu==2)//Ĭ�ϵ���
			{
				leftSpeedSet  = -200; 
				rightSpeedSet = -200; 			
			}
			else if(Flag_sudu==1)//����
			{
				leftSpeedSet  = -350; 
				rightSpeedSet = -350; 				
			}	
		}
		else if(Flag_Left==1)
		{

			if(Flag_sudu==2||Flag_sudu==1)//Ĭ�ϵ���
			{
				leftSpeedSet  = -100; 
				rightSpeedSet = 100;			
			}
		}
		else if(Flag_Right==1)
		{

			if(Flag_sudu==2||Flag_sudu==1)//Ĭ�ϵ���
			{
				leftSpeedSet  = 100; 
				rightSpeedSet = -100;			
			}
		}
		else
		{
			leftSpeedSet  = 0; 
			rightSpeedSet = 0; 
		}	
	}
}


/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	int temp;
	if(a<0)  
	  temp=-a;  
	else 
	  temp=a;
	return temp;
}

