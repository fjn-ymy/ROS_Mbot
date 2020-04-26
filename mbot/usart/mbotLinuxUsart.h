#ifndef __MBOTLINUXUSART__
#define __MBOTLINUXUSART__
#include <sys.h>	

#define START   0X11
#define FILTER_N 10

//��linux���ղ��������ݵ�������ַ��
extern int receiveTo103(int *p_leftSpeedSet,int *p_rightSpeedSet);   
//��װ���ݣ�����USART1_Send_String�����ݷ��͸�linux
extern void usartSendSpeed(float Left_V, float Right_V,float Angle); 
//����ָ���ַ�����ĺ���
void USART_Send_String(u8 *p,u16 sendSize);     
//�����λѭ������У�飬�õ�У��ֵ��һ���̶�����֤���ݵ���ȷ��
unsigned char getCrc8(unsigned char *ptr, unsigned short len); 

#endif
