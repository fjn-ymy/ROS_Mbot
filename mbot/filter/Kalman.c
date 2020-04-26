#include "kalman.h"
#include "stdlib.h"

Kalman_filter   kalman_V_Left;
Kalman_filter   kalman_V_Right;

void kalman_init(Kalman_filter* kalman)
{
    kalman->x_last=1;
    kalman->p_last=5;     //��������ֵ��ʼ������ν����P��ҪΪ0
    
    kalman->Q=0.0001;     //��������
    kalman->R=0.03; 	  //�۲�����,��ԭʼ�ٶ����ݵķ��������
    kalman->A=1;          //�򻯵�ģ��
    kalman->H=1;
}

double kalman_filter(Kalman_filter *kalman,double input)
{
    kalman->x_last=kalman->A*kalman->x_last;                //������һ�ε�ֵԤ�Ȿ�ε�ֵ    
    kalman->p_last=kalman->p_last+kalman->Q;                //������һ�ε�Э������㱾�ε�Э����    
    kalman->kg=kalman->p_last/(kalman->p_last+kalman->R);   //���ݱ��ε�Э�������kalman����    
    kalman->x_last=kalman->x_last+kalman->kg*(input-kalman->H*kalman->x_last);//������ε�����ֵ    
    kalman->p_last=(1-kalman->kg)*kalman->p_last;           //������ε�Э����	
	  kalman->best=kalman->x_last;    
    return kalman->best;
}

