#ifndef __SERVO_H
#define __SERVO_H

#include "sys.h"

#define SERVO TIM1->CCR4		//ʹ��TIM1�Ĳ���/�ȽϼĴ�����������PWM��ռ�ձ�

void Servo_PWM_Init(u16 arr,u16 psc);	//�����ʼ��

#endif
