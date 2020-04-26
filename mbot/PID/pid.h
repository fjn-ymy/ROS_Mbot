#ifndef __PID_H__
#define __PID_H__

#include <sys.h>	
#define PID_Uint struct pid_uint

PID_Uint
{
	s32 U_kk;    	    //��һ�ε������
	s32 ekk;		 	//��һ�ε�����ƫ��
	s32 ekkk;			//ǰһ�ε�����ƫ��
	s32 Ur;				//�޷����ֵ,���ʼ��
	s32 Kp;				//����
	s32 Ki;				//����
	s32 Kd;				//΢��
	s32 Umax;
	s32 Umin;
};

/****************************��Ӻ���***************************/

void  PID_Init(void);
void  reset_Uk(PID_Uint *p);
s32   PID_common(int set,int jiance,PID_Uint *p);
void  Pid_Ctrl(void);

/*****************************��ӱ���***************************/
extern s32       GYRO_Ang;
extern u8	     g_Pid_En;
extern u8	     g_Pid_Angle_En;             //�Ƕ�PID����
extern s16       g_Pid_Left_Adjust;		     //�����ٶȵ�����
extern s16       g_Pid_Right_Adjust;		 //�����ٶȵ�����
extern s16       g_Pid_Angle_Adjust;		 //�Ƕȵ����������þ���һ�߼�һ�߼�
extern PID_Uint  pid_Task_A_1_1;
//�ٶ�
extern int    leftSpeedNow; 
extern int    rightSpeedNow; 
//����1000֮����ٶ��趨ֵ
extern int       leftSpeedSet; 
extern int       rightSpeedSet; 
extern int       angleSet;//�Ƕ��趨ֵ
#endif //__PID_H__
