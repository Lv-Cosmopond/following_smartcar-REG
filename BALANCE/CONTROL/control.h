#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

#define PI 3.14159265
#define ZHONGZHI 0
#define T 0.156f
#define L 0.1445f
#define K 622.8f


void Kinematic_Analysis(float velocity,float angle);
void Set_Pwm(int motor_a,int motor_b,int servo);
void Xianfu_Pwm(void);

int myabs(int a);
void Direction(void);
int Incremental_PI_A (int Encoder,int Target);
int Incremental_PI_B (int Encoder,int Target);
int Incremental_PI_DforV (int d,int Target);
int Incremental_P_DforV(int d,int Target);
int Incremental_PD_DforV (int d,int Target);

#endif
