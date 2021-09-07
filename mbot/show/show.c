#include "show.h"
#include "pid.h"
#include "stdio.h"
#include "MPU6050.h"
#include "control.h"
#include "motor.h"

extern unsigned char sendCtrlFlag;
extern unsigned char receCtrlFlag;
/**************************************************************************
函数功能：向PC发送调试数据，配合串口助手使用
入口参数：无
返回  值：无
**************************************************************************/
void pcShow(void)
{    
	//printf("V:%d.%dv:%lf:%lf:%lf:%lf:%lf\r\n",Voltage/100,Voltage%100,Accel_X,Accel_Y,Accel_Z,Gyro_X,Gyro_Z);//打印到APP上面 显示波形
	//printf("Angle:%d V:%d.%d:%d:%d\r\n",(int)Gyro_Turn,Voltage/100,Voltage%100,leftWheelEncoder,rightWheelEncoder);//打印到APP上面 显示波形
	//printf("Angle:%d V:%d.%d:%d:%d | %d:%d\r\n",(int)Gyro_Turn,Voltage/100,Voltage%100,g_Pid_Left_Adjust,g_Pid_Right_Adjust,leftSpeedNow,rightSpeedNow);//打印到APP上面 显示波形
//	printf("%d,%d,%d,%d\r\n",leftSpeedNow,rightSpeedNow,(int)yaw,sendCtrlFlag);
	printf("%d\r\n",sendCtrlFlag);
	//printf("%d,%d,%d\r\n",leftSpeedNow,Voltage,(int)yaw);
	//printf("%d,%d,\r\n",g_Pid_Left_Adjust,g_Pid_Right_Adjust);
	//printf("Angle:%d,%d,\r\n",(int)Gyro_Turn,g_Pid_Angle_Adjust);

}


void status_show(void)
{
	switch(receCtrlFlag)
	{
		case 1:
			printf("打开建图功能\r\n");break;
		case 2:
			printf("建图完成\r\n");break;
		case 3:
			printf("打开导航功能\r\n");break;
		case 4:
			printf("自动巡航\r\n");break;
		case 5:
			printf("到达A点\r\n");break;
		case 6:
			printf("到达B点\r\n");break;
		default:
			break;
	}
}
