#include "mbotLinuxUsart.h"
#include <math.h>

/**************************************************************************
ͨ�ŵķ��ͺ����ͽ��պ��������һЩ���������������������
**************************************************************************/
//���ݽ����ݴ���
unsigned char  receiveBuff[16] = {0};         
//ͨ��Э�鳣��
const unsigned char ender[2]   = {0x0d, 0x0a};
const unsigned char header[2]  = {0x55, 0xaa};
const int SPEED_INFO           = 0xa55a; 
const int GET_SPEED            = 0xaaaa;
//�������ݣ������١������١��Ƕȣ�������
union sendData
{
	float d;
	unsigned char data[4];
}leftdata,rightdata,angle;
//���ͽ���У��ֵ������
union checkSum
{
	short d;
	unsigned char data[1];
}SendCheckSum, ReceiveCheckSum;
//������Ϣ����������ͷ������
union receiveHeader
{
	int d;
	unsigned char data[2];
}receive_command;
//��������Ĺ�����
union sendCommand
{
	int d;
	unsigned char data[2];
}send_command;
//������̼���Ϣ�������ٹ�����
union odometry
{
	int odoemtry_int;
	unsigned char odometry_char[4];
}vel_left, vel_right;

/**************************************************************************
�������ܣ�ͨ�������жϷ���������ȡ��λ�����͵������ֿ����ٶȣ��ֱ���������
��ڲ������������ٿ��Ƶ�ַ���������ٿ��Ƶ�ַ
����  ֵ������������
**************************************************************************/
int receiveTo103(int *p_leftSpeedSet,int *p_rightSpeedSet)
{
	u8 USART_Receiver               = 0;          //��������
	static u8 checkSum              = 0;
	static u8  USARTBufferIndex     = 0;
	static s16 i=0,j=0,k=0;
	static u8  USARTReceiverFront   = 0;
	static u8  Start_Flag           = START;      //һ֡���ݴ��Ϳ�ʼ��־λ
	static s16 dataLength           = 0;
	const int V_XIN_FU              =400;         //�ٶ��޷������Ը����Լ��������
	USART_Receiver = USART_ReceiveData(USART1);   //@@@@@#####�����ʹ�ò���USART1���ĳ���Ӧ�ģ�����USART3
	//������Ϣͷ
	if(Start_Flag == START)
	{
		if(USART_Receiver == 0xaa)       
		{  
			if(USARTReceiverFront == 0x55)        //����ͷ��λ
			{
				Start_Flag = !START;              //�յ�����ͷ����ʼ��������
				receiveBuff[0]= 0x55;             //�洢����ͷ��Ϣ
				receiveBuff[1]= 0xaa;
				//printf("ok\n");
				USARTBufferIndex = 0;             //��������ʼ��
				checkSum = 0x00;				  //У��ͳ�ʼ��
			}
		}
		else 
		{
			USARTReceiverFront = USART_Receiver;  //����ͷ1 0x55
		}
	}
	else
    { 
		switch(USARTBufferIndex)
		{
			case 0://������Ϣ����
				if(i==0)
				{
					receiveBuff[2] = USART_Receiver;         //buf[2]
					receive_command.data[0] = receiveBuff[2];
					i++;
				}
				else if(i==1)
				{
					receiveBuff[3] = USART_Receiver;         //buf[3]
					receive_command.data[1] = receiveBuff[3];
					//�����Ϣ���ͣ�ʧ�ܵĻ�ֱ������
					if(receive_command.d != GET_SPEED)
					{
						printf("Received command error!");
						return 0;
					}
					USARTBufferIndex++;
				}
				break;
			case 1://�����������ٶ����ݵĳ���
				receiveBuff[4] = USART_Receiver;            //buf[4]	
				dataLength     = receiveBuff[4];
				USARTBufferIndex++;
				break;
			case 2://�����ٶ����ݣ�����ֵ���� 
				receiveBuff[j + 5] = USART_Receiver;        //buf[5] - buf[12]					
				j++;
				if(j >= dataLength)                         //�޸Ĵ˴������շ���һ��
				{
					j = 0;
					USARTBufferIndex++;
				}
				break;
			case 3://����У��ֵ��Ϣ
				receiveBuff[5 + dataLength] = USART_Receiver;   //buf[13]
				ReceiveCheckSum.data[0] = receiveBuff[5 + dataLength];
				checkSum = getCrc8(receiveBuff, 5 + dataLength);
				  // �����ϢУ��ֵ
				if (checkSum != ReceiveCheckSum.d)
				{
					printf("Received data check sum error!");
					return 0;
				}
				USARTBufferIndex++;
				break;
			case 4://������Ϣβ
				if(k==0)
				{
					receiveBuff[6 + dataLength] = USART_Receiver;
					k++;
				}
				else if (k==1)
				{
					receiveBuff[6 + dataLength + 1] = USART_Receiver;
					//�����Ϣβ
					if (receiveBuff[6 + dataLength] != ender[0] || receiveBuff[6 + dataLength + 1] != ender[1])
					{
						printf("Received message header error!");
						return 0;
					}
					//ͨ�����Խ����ٶȸ�ֵ����					
					 for(k = 0; k < 4; k++)
					{
						vel_left.odometry_char[k]  = receiveBuff[k + 5];
						vel_right.odometry_char[k] = receiveBuff[k + 9];
					}				
					//�ٶȸ�ֵ����
					*p_leftSpeedSet  = vel_left.odoemtry_int;
					*p_rightSpeedSet = vel_right.odoemtry_int;
					/*=========================�ٶ�������V_XIN_FU====================*/
					if(*p_leftSpeedSet>V_XIN_FU)
						*p_leftSpeedSet=V_XIN_FU;
					if(*p_leftSpeedSet<-V_XIN_FU)
						*p_leftSpeedSet=-V_XIN_FU;
					if(*p_rightSpeedSet>V_XIN_FU)
						*p_rightSpeedSet=V_XIN_FU;
					if(*p_rightSpeedSet<-V_XIN_FU)
						*p_rightSpeedSet=-V_XIN_FU;
					/*==============================================================*/
					USARTBufferIndex++;
				}
				break;
			case 5:
				//printf("ok\n");
				USARTBufferIndex   = 0;
				USARTReceiverFront = 0;
				Start_Flag         = START;
				checkSum           = 0;
				dataLength         =0;
				i = 0;
				j = 0;
				k = 0;
				break;
			 default:break;
		}		
	}
	return 0;
}
/**************************************************************************
�������ܣ����������ٺͽǶ����ݽ��д����ͨ�����ڷ��͸�Linux
��ڲ�����ʵʱ�������١�ʵʱ�������١�ʵʱ�Ƕȣ����û�нǶ�Ҳ���Բ�����
����  ֵ����
**************************************************************************/
void usartSendSpeed(float Left_V, float Right_V,float Angle)
{
	//Э�����ݻ�������
	unsigned char buf[20] = {0};
	int i, length = 0;

	// ���������������ٶ�

	leftdata.d  = Left_V;
	rightdata.d = Right_V;
	angle.d     = Angle;
	
	// ������Ϣͷ
	for(i = 0; i < 2; i++)
		buf[i] = header[i];
	
	// ������Ϣ����
	send_command.d = SPEED_INFO;
	for(i = 0; i < 2; i++)
		buf[i + 2] = send_command.data[i];
	
	// ���û������������ٶȡ��Ƕ�
	length = 12;
	buf[4] = length;
	for(i = 0; i < 4; i++)
	{
		buf[i + 5] = leftdata.data[i];
		buf[i + 9] = rightdata.data[i];
		buf[i + 13]= angle.data[i];
	}
	
	// ����У��ֵ����Ϣβ
	buf[5 + length] = getCrc8(buf, 5 + length);
	buf[6 + length] = ender[0];
	buf[6 + length + 1] = ender[1];
	
	//�����ַ�������
	USART_Send_String(buf,sizeof(buf));
}
/**************************************************************************
�������ܣ�����ָ����С���ַ����飬��usartSendSpeed��������
��ڲ����������ַ�������С
����  ֵ����
**************************************************************************/
void USART_Send_String(u8 *p,u16 sendSize)
{ 
	static int length =0;
	while(length<sendSize)
	{   
		//@@@@@#####�����ʹ�ò���USART1���ĳ���Ӧ�ģ�����USART3�������������޸�
		while( !(USART1->SR&(0x01<<7)) );//���ͻ�����Ϊ��
		USART1->DR=*p;                   
		p++;
		length++;
	}
	length =0;
}
/**************************************************************************
�������ܣ������λѭ������У�飬��usartSendSpeed��receiveTo103��������
��ڲ����������ַ�������С
����  ֵ����
**************************************************************************/
unsigned char getCrc8(unsigned char *ptr, unsigned short len)
{
	unsigned char crc;
	unsigned char i;
	crc = 0;
	while(len--)
	{
		crc ^= *ptr++;
		for(i = 0; i < 8; i++)
		{
			if(crc&0x01)
                crc=(crc>>1)^0x8C;
			else 
                crc >>= 1;
		}
	}
	return crc;
}
/**********************************END***************************************/

////�жϷ�����
//void USART1_IRQHandler()
//{
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
// 	 {
//		 USART_ClearITPendingBit(USART1,USART_IT_RXNE);//��������жϱ�־λ
//		 receiveTo103(&leftSpeedSet,&rightSpeedSet);
//	 }
//}



