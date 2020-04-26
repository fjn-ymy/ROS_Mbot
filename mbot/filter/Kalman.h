#ifndef __KALMAN_H__
#define __KALMAN_H__

#include "stm32f10x.h"

typedef struct KALMAN
{
    double Q;       //����������Э����
    double R;       //�۲�������Э����
    
    double x_last;  //�ϴε�����ֵ�����β���ֵ����������ֵ
    double p_last;  //�ϴ�Э�������Ԥ��Э�������Э����
    
    double kg;      //kalman����
    
    double A;       //ϵͳת�ƾ���
    double H;       //�۲�ת�ƾ���   
		
    double best;

}Kalman_filter;

extern Kalman_filter   kalman_V_Left;
extern Kalman_filter   kalman_V_Right;

void  kalman_init(Kalman_filter* kalman);
double kalman_filter(Kalman_filter *kalman,double input);

#endif
