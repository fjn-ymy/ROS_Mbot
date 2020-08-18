#ifndef __PID_H__
#define __PID_H__

#include "stm32f10x.h"

struct pid_uint
{
	s32 U_kk;    	    //��һ�ε������
	s32 ekk;		 	//��һ�ε�����ƫ��
	s32 ekkk;			//ǰһ�ε�����ƫ��
	s32 Ur;				//�޷����ֵ,���ʼ��
	s32 Kp;				//����
	s32 Ki;				//����
	s32 Kd;				//΢��
	
	u8  En;             //����
	s16 Adjust;         //������	
};

/****************************��Ӻ���***************************/

void  PID_Init(void);
void  reset_Uk(struct pid_uint *p);
s32   PID_common(int set,int jiance,struct pid_uint *p);
void  Pid_Ctrl(void);

#endif //__PID_H__
