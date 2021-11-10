#ifndef __SERVO_H
#define __SERVO_H

#include "sys.h"

#define SERVO TIM1->CCR4		//使用TIM1的捕获/比较寄存器，来产生PWM的占空比

void Servo_PWM_Init(u16 arr,u16 psc);	//舵机初始化

#endif
