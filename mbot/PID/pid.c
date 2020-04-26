#include "pid.h"

s32      GYRO_Ang              = 0;

u8	     g_Pid_En  		       = 1;          //pid����
u8	     g_Pid_Angle_En  	   = 0;          //�Ƕ�PID����
s16      g_Pid_Left_Adjust     = 0;		     //�����ٶȵ�����
s16      g_Pid_Right_Adjust    = 0;		     //�����ٶȵ�����
s16      g_Pid_Angle_Adjust    = 0;		     //�Ƕȵ����������þ���һ�߼�һ�߼�

PID_Uint    pid_Task_Letf;
PID_Uint    pid_Task_Right;
PID_Uint    pid_Task_A_1_1;		

//����1000֮����ٶ�ʵʱֵ
int      leftSpeedNow  = 0; 
int      rightSpeedNow = 0; 
//����1000֮����ٶ��趨ֵ
int      leftSpeedSet  = 0; 
int      rightSpeedSet = 0; 
int      angleSet      = 0;//�Ƕ��趨ֵ

/****************************************************************************
*�������ƣ�PID_Init(void)
*�������ܣ���ʼ��PID�ṹ�����
*����ֵ	 ����
*����ֵ  ����
****************************************************************************/
void PID_Init(void)
{
//����1024ԭ�������ָ��������㣬ȫ�����������㣬����PID�����������ٶȻ����
/***********************�����ٶ�pid****************************/
	pid_Task_Letf.Kp = 1024 * 0.5;//0.4
 	pid_Task_Letf.Ki = 1024 * 0;	
	pid_Task_Letf.Kd = 1024 * 0.08; 
	pid_Task_Letf.Ur = 1024 * 4000;
	pid_Task_Letf.Umax=1024 * 400;
	pid_Task_Letf.Umin=1024 *(-200);
	reset_Uk(&pid_Task_Letf);		
/***********************�����ٶ�pid****************************/
	pid_Task_Right.Kp = 1024 * 0.35;//0.2
 	pid_Task_Right.Ki = 1024 * 0;	//��ʹ�û���
	pid_Task_Right.Kd = 1024 * 0.06; 
	pid_Task_Right.Ur = 1024 * 4000;
	pid_Task_Right.Umax=1024 *(400);
	pid_Task_Right.Umin=1024 *(-200);
	reset_Uk(&pid_Task_Right);
	
/***********************�Ƕ�pid****************************/
	pid_Task_A_1_1.Kp = 1024 * 3.0;
 	pid_Task_A_1_1.Ki = 1024 * 100000;	
	pid_Task_A_1_1.Kd = 1024 * 2.0; 
	pid_Task_A_1_1.Ur = 1024 * 45;

	reset_Uk(&pid_Task_A_1_1);
}

/***********************************************************************************************
 �� �� ����void reset_Uk(PID_Uint *p)
 ��    �ܣ���ʼ��U_kk,ekk,ekkk
 ˵    �����ڳ�ʼ��ʱ���ã��ı�PID����ʱ�п�����Ҫ����
 ��ڲ�����PID��Ԫ�Ĳ����ṹ�� ��ַ
 �� �� ֵ����
************************************************************************************************/
void reset_Uk(PID_Uint *p)
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
s32 PID_common(int set,int jiance,PID_Uint *p)
{
	int ek=0,U_k=0;

	ek=jiance - set;                                        //��ֵ
	
	U_k=p->U_kk + p->Kp*(ek - p->ekk) + p->Ki*ek + p->Kd*(ek - 2*p->ekk + p->ekkk);
	
	p->U_kk=U_k;
    p->ekkk=p->ekk;
	p->ekk=ek;
	
	if(U_k>(p->Ur))		                                    
		U_k=p->Ur;
	if(U_k<-(p->Ur))
		U_k=-(p->Ur);
	
	return U_k>>10; //
}

/***********************************************************************************
** �������� ��void Pid_Which(PID_TYPE *px, PID_TYPE *py, PID_TYPE *pa)
** �������� ��pidѡ����	      
** ��ڲ��� ���Ƕ�pid������				      
** ���ڲ��� ����						     
** ˵��     ��
***********************************************************************************/
void Pid_Which(PID_Uint *px, PID_Uint *py, PID_Uint *pa)
{
	/**********************�����ٶ�pid*************************/
	if(g_Pid_En == 1)
	{									
		g_Pid_Left_Adjust = -PID_common(leftSpeedSet, leftSpeedNow, px);		
	}	
	else
	{
		g_Pid_Left_Adjust = 0;
		reset_Uk(px);
		g_Pid_En = 2; 
	}
	/***********************�����ٶ�pid*************************/
	if(g_Pid_En == 1)
	{
		g_Pid_Right_Adjust = -PID_common(rightSpeedSet, rightSpeedNow, py);		
	}	
	else
	{
		g_Pid_Right_Adjust = 0;
		reset_Uk(py);
		g_Pid_En = 2; 
	}
	/***********************�Ƕ�pid*************************/
	if(g_Pid_Angle_En == 1)
	{
		g_Pid_Angle_Adjust = -PID_common(angleSet, (int)Gyro_Turn, pa);		
	}	
	else
	{
		g_Pid_Angle_Adjust = 0;
		reset_Uk(pa);
		g_Pid_Angle_En = 2; 
	}
}

/*******************************************************************************
 *
 * ��������Pid_Ctrl(void)
 * ����  ��Pid����
 * ����  ����
 * ���  ����
 * ˵��  ��pid���㣬����2�����ӷ��ٶ�
 *
 *******************************************************************************/
void Pid_Ctrl(void)
{
	if(g_Pid_En == 1)
	{
		Pid_Which(&pid_Task_Letf, &pid_Task_Right, &pid_Task_A_1_1); 
		Moto1 += g_Pid_Left_Adjust;
		Moto2 += g_Pid_Right_Adjust;
	}
}


