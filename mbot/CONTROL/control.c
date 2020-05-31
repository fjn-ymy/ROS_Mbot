#include "control.h"	
#include "pid.h"

#define GYRO_Z_OFFSET 9

u8 Flag_Target;
int Voltage_Temp,Voltage_Count,Voltage_All;

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
	if(INT==0)		
	{   
		EXTI->PR=1<<12;                                                         //����жϱ�־λ   
		
		Flag_Target=!Flag_Target;
		if(delayFlag==1)
		{
			if(++delayValue==3)	 delayValue=0,delayFlag=0;                //5 //���������ṩ15ms�ľ�׼��ʱ
		}
		if(Flag_Target==1)                                                      //5ms��ȡһ�������Ǻͼ��ٶȼƵ�ֵ
		{		
			Voltage_Temp=Get_battery_volt();		                            //=====��ȡ��ص�ѹ		
			Voltage_Count++;                                                    //=====ƽ��ֵ������
			Voltage_All+=Voltage_Temp;                                          //=====��β����ۻ�
			if(Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//=====��ƽ��ֵ		
			return 0;	                                               
		}                                                                       //10ms����һ�Σ�Ϊ�˱�֤M�����ٵ�ʱ���׼�����ȶ�ȡ����������
		leftWheelEncoder += getTIMx_DetaCnt(TIM4);
		rightWheelEncoder+= getTIMx_DetaCnt(TIM2);
		
		//��¼�������ұ���������
		leftWheelEncoderNow   = leftWheelEncoder;
		rightWheelEncoderNow  = rightWheelEncoder;
			
		//10ms����    	
		leftSpeedNow          = (leftWheelEncoderNow - leftWheelEncoderLast)*1000*100*0.000120830;//
		rightSpeedNow         = (rightWheelEncoderNow - rightWheelEncoderLast)*1000*100*0.000181245;//

		//��¼�ϴα���������
		leftWheelEncoderLast  = leftWheelEncoderNow;                    //===��ȡ��������ֵ
		rightWheelEncoderLast = rightWheelEncoderNow;                   //===��ȡ��������ֵ
		
		getAngle();                                                     //===������̬	
		key();
		Led_Flash(200);                                                 //===LED��˸;����ģʽ 1s�ı�һ��ָʾ�Ƶ�״̬		
		if((int)yaw==0) 
			Led_Flash(30); 				

		App_main();
		Pid_Ctrl();
		Xianfu_Pwm();

		if(Turn_Off(Voltage)==0)                                         //===�����ѹ�쳣
		{
			Set_Pwm(motorLeft,motorRight);                                 //===��ֵ��PWM�Ĵ��� 		
		}
		else                                                             //���ﲻ�������͹�pid��Ϊ������adc��û�ɼ�����ֵ
		{
			motorLeft=0;
			motorRight=0;
			AIN1=0;                                            
			AIN2=0;
			BIN1=0;
			BIN2=0;
		}
	}       	
	 return 0;	  
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
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motorLeft,int motorRight)
{
	if(motorLeft>0) AIN2=0,			AIN1=1;
	else 	    AIN2=1,			AIN1=0;
	PWMA=myabs(motorLeft);
	if(motorRight>0)	BIN1=0,			BIN2=1;
	else        BIN1=1,			BIN2=0;
	PWMB=myabs(motorRight);	
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{	
	int Amplitude=6900;  //===PWM������7200 ������6900
	if(motorLeft<-Amplitude) motorLeft=-Amplitude;	
	if(motorLeft>Amplitude)  motorLeft=Amplitude;	
	if(motorRight<-Amplitude) motorRight=-Amplitude;	
	if(motorRight>Amplitude)  motorRight=Amplitude;		
}
/**************************************************************************
�������ܣ�key�����ٶȣ�����PID
��ڲ�������
����  ֵ����
**************************************************************************/
void key(void)
{	
	u8 tmp=0;
	tmp=click_N_Double(50); //��������С������ͣ
	if(tmp==1)
	{
		leftSpeedSet  = 500; 
		rightSpeedSet = 500; 
		
	}
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
�������ܣ���ȡ�Ƕ� �����㷨�������ǵĵ�У�����ǳ����� 
��ڲ�������
����  ֵ����
**************************************************************************/
void getAngle()
{
		Read_DMP();                      //===��ȡ���ٶȡ����ٶȡ����
		if(Yaw<-GYRO_Z_OFFSET)
			Yaw=Yaw+360;
		yaw=GYRO_Z_OFFSET+Yaw;     //===����ת��Ƕ�
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



