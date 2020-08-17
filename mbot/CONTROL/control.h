#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "stm32f10x.h"
#include "pid.h"

#define PI 3.14159265

extern int leftWheelEncoder;
extern int rightWheelEncoder;
       
extern int leftWheelEncoderNow;
extern int rightWheelEncoderNow;
extern int leftWheelEncoderLast;
extern int rightWheelEncoderLast;

extern int Voltage; //��ص�ѹ������صı���

extern float yaw;         //ת��������

extern u8 Flag_Qian;
extern u8 Flag_Hou;
extern u8 Flag_Left;
extern u8 Flag_Right;
extern u8 Flag_sudu;
extern u8 Flag_useApp;    //����ң����صı���


void App_main(void);

#endif
