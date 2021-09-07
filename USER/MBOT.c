#include "sys.h"

//====================自己加入的头文件===============================
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
char hongwaiFlag=0;		//红外标志位
int hongwaiCount=0;		//红外计数
/*===================================================================
程序功能：ROS小车底层代码（全部）
程序编写：公众号：小白学移动机器人
其他    ：如果对代码有任何疑问，可以私信小编，一定会回复的。
=====================================================================
------------------关注公众号，获得更多有趣的分享---------------------
===================================================================*/
int main(void)
{ 
	//发送计数
	char sendCount=0;
	
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//禁用JTAG 启用 SWD
	
	MY_NVIC_PriorityGroupConfig(2);	//=====设置中断分组
	
	delay_init();	    	        //=====延时函数初始化
	
	LED_Init();                     //=====LED初始化    程序灯	
	
	usart1_init(115200);	        //=====串口1初始化  树莓派
	usart2_init(9600);						//======串口2 接收红外触发接收
	usart3_init(9600);              //=====串口3初始化  蓝牙

	IIC_Init();                     //=====IIC初始化    读取MPU6050数据
	MPU6050_initialize();           //=====MPU6050初始化	
	DMP_Init();                     //=====初始化DMP 

	Encoder_Init_TIM2();            //=====初始化编码器1接口
	Encoder_Init_TIM4();            //=====初始化编码器2接口
	
	MY_ADC_Init();                  //=====adc初始化    电池电量检测
	
	Motor_Init(7199,0);             //=====初始化PWM 10KHZ，用于驱动电机 如需初始化驱动器接口
	PID_Init();                     //=====PID初始化
	
	MBOT_EXTI_Init();               //=====MPU6050 5ms定时中断初始化
	
	
	 EXTI2_Init(); //初始化外部中断
	
	while(1)
	{
		//USART_Send_String(USART2, buf, sizeof(buf));
		//给树莓派发送速度，角度,这里速度已经乘以1000
		if(sendCount==0)//发送  14.4ms  发送一次数据 70Hz 左右
		{
			//发送需要一定的延时
			usartSendData(USART1,(short)leftSpeedNow,(short)rightSpeedNow,(short)yaw,(short)Voltage,(float)q1,(float)q2,(float)q3,(float)q0,sendCtrlFlag);  //1ms
			//蓝牙调试时用，不调试注释
			//pcShow();                                                           //2.2ms单个float数据
		  status_show();
			sendCount++;
		}
		else
		{
			sendCount++;
			if(sendCount==25)
				sendCount=0;
		}
		//获取角度		
		getAngle(&yaw,&yaw_acc_error);         
		
		//************Begin 红外处理部分 Begin**************
		hongwaiCount++;
		if(hongwaiCount>=500&&hongwaiFlag==1)	//小车没有接收到红外信号，停止运动
		{
		 Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;		//小车停止
			if(hongwaiCount>2000)
			{
							hongwaiFlag=2;			//红外信号  小车旋转
			}
		}
		if(hongwaiFlag==2)		//
		{
			if(hongwaiCount>=20000)																//旋转几圈之后 小车停止
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
				Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;			//小车旋转
			}
			else
					Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;			//小车旋转
			

		}
		//************ END 红外处理部分 END**************
	} 
}

//中断服务函数
void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
 	 {
		 USART_ClearITPendingBit(USART1,USART_IT_RXNE);//首先清除中断标志位
		 usartReceiveOneData(USART1,&leftSpeedSet,&rightSpeedSet,&receCtrlFlag);
	 }
 
}



/**************************************************************************
函数功能：串口2接收中断
入口参数：无
返回  值：无
**************************************************************************/


void USART2_IRQHandler()
{
//	static	int uart_receive=0;                           //红外接收到的信号
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
 	 {
		 USART_ClearITPendingBit(USART2,USART_IT_RXNE);//首先清除中断标志位

		 hongwaiFlag=1;
		 Flag_useApp=1;
		 Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
			hongwaiCount=0;		 
	 }
 
}

void EXTI2_IRQHandler(void)
{
  delay_ms(10);    //防抖
	 if(KEY==1)       //防抖
	{	  
			hongwaiFlag=1;
		 Flag_useApp=1;
		 Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;
			hongwaiCount=0;	
		printf("接收到红外信号\r\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除EXTI5线路中断标志
}

/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART3_IRQHandler(void)
{	
	static	int uart_receive=0;                           //蓝牙接收相关变量
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);    //清除中断标志位	串口读取DR寄存器以后会硬件清除中断标志位	
		uart_receive=USART_ReceiveData(USART3); 
		
		if(uart_receive==0x4C)//模式切换L
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
		
		if(uart_receive==0x4A)//速度切换
		{
			if(Flag_sudu==2) //低速挡（默认值）
				Flag_sudu=1;   //高速档
			else
				Flag_sudu=2;  
		}	
		
		if(uart_receive==0x4B)
		{
			if(Flag_useApp==0)//默认不使用APP
				Flag_useApp=1;
			else
				Flag_useApp=0;
		}
		
		if(uart_receive>10)  //默认使用
		{			
			if(uart_receive==0x5A)	
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
			else if(uart_receive==0x41||uart_receive==0x42||uart_receive==0x48)	
				Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////前
			else if(uart_receive==0x44||uart_receive==0x45||uart_receive==0x46)	
				Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////后
			else if(uart_receive==0x43)	
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;//////////////右
			else if(uart_receive==0x47)
				Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;//////////////左
			else 
				Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
		}
		
	}  											 
} 
