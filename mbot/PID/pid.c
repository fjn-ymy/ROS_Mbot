#include "pid.h"
#include "motor.h"

struct pid_uint pid_Task_Letf;
struct pid_uint pid_Task_Right;

/****************************************************************************
*�������ƣ�PID_Init(void)
*�������ܣ���ʼ��PID�ṹ�����
****************************************************************************/

void PID_Init(void)
{
//����1024ԭ�������ָ��������㣬ȫ�����������㣬����PID�����������ٶȻ����
/***********************�����ٶ�pid****************************/
	pid_Task_Letf.Kp = 1024 * 0.5;//0.4
 	pid_Task_Letf.Ki = 1024 * 0;	
	pid_Task_Letf.Kd = 1024 * 0.08; 
	pid_Task_Letf.Ur = 1024 * 4000;
	pid_Task_Letf.Adjust = 0;
	pid_Task_Letf.En     = 1;
	reset_Uk(&pid_Task_Letf);		
/***********************�����ٶ�pid****************************/
	pid_Task_Right.Kp = 1024 * 0.35;//0.2
 	pid_Task_Right.Ki = 1024 * 0;	//��ʹ�û���
	pid_Task_Right.Kd = 1024 * 0.06; 
	pid_Task_Right.Ur = 1024 * 4000;
	pid_Task_Right.Adjust = 0;
	pid_Task_Right.En     = 1;
	reset_Uk(&pid_Task_Right);
}

/***********************************************************************************************
 �� �� ����void reset_Uk(PID_Uint *p)
 ��    �ܣ���ʼ��U_kk,ekk,ekkk
 ˵    �����ڳ�ʼ��ʱ���ã��ı�PID����ʱ�п�����Ҫ����
 ��ڲ�����PID��Ԫ�Ĳ����ṹ�� ��ַ
************************************************************************************************/

void reset_Uk(struct pid_uint *p)
{
	p->U_kk=0;
	p->ekk=0;
	p->ekkk=0;
}

/***********************************************************************************************
 �� �� ����s32 PID_commen(int set,int jiance,PID_Uint *p)
 ��    �ܣ�PID���㺯��
 ˵    ���������ⵥ��PID�Ŀ�����
 ��ڲ���������ֵ��ʵ��ֵ��PID��Ԫ�ṹ��
 �� �� ֵ��PID������
************************************************************************************************/

s32 PID_common(int set,int jiance,struct pid_uint *p)
{
	int ek=0,U_k=0;

	ek=jiance - set;                                                               
	
	U_k=p->U_kk + p->Kp*(ek - p->ekk) + p->Ki*ek + p->Kd*(ek - 2*p->ekk + p->ekkk);
	
	p->U_kk=U_k;
    p->ekkk=p->ekk;
	p->ekk=ek;
	
	if(U_k>(p->Ur))		                                    
		U_k=p->Ur;
	if(U_k<-(p->Ur))
		U_k=-(p->Ur);
	
	return U_k>>10; 
}

/***********************************************************************************
** �������� ��void Pid_Which(int leftSet,int rightSet,int leftNow,int rightNow, struct pid_uint *pl, struct pid_uint *pr)
** �������� ��pidѡ����	      
***********************************************************************************/

void Pid_Which(int leftSet,int rightSet,int leftNow,int rightNow, struct pid_uint *pl, struct pid_uint *pr)
{
	/**********************�����ٶ�pid*************************/
	if(pl->En == 1)
	{									
		pl->Adjust = -PID_common(leftSet, leftNow, pl);		
	}	
	else
	{
		pl->Adjust = 0;
		reset_Uk(pl);
		pl->En = 2; 
	}
	/***********************�����ٶ�pid*************************/
	if(pr->En == 1)
	{
		pr->Adjust = -PID_common(rightSet, rightNow, pr);		
	}	
	else
	{
		pr->Adjust = 0;
		reset_Uk(pr);
		pr->En = 2; 
	}
}

/*******************************************************************************
 * ��������Pid_Ctrl(void)
 * ����  ��Pid����
 *******************************************************************************/

void Pid_Ctrl(void)
{
	Pid_Which(leftSpeedSet,rightSpeedSet,leftSpeedNow,rightSpeedNow,&pid_Task_Letf, &pid_Task_Right); 
	motorLeft += pid_Task_Letf.Adjust;
	motorRight += pid_Task_Right.Adjust;
}

