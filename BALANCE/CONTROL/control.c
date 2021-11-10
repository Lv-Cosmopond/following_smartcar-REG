#include "control.h"		

/**************************************************************************
�������ܣ�С���˶���ѧģ��
��ڲ������ٶȺ�ת��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis(float velocity,float angle)
{
		Target_A=velocity*(1+T*tan(angle)/2/L); 
		Target_B=velocity*(1-T*tan(angle)/2/L);      //���ֲ���
		Servo=SERVO_INIT+angle*K;                    //���ת��   
}
/**************************************************************************
�������ܣ�ʹ��TIM1���жϹ��ܣ���TIM1����PWM��ͬʱ����CNT�������arrʱ�����жϡ�
ע��			TIM1Ƶ��36MHz��72��Ƶ������Ƶ��Ϊ36M/72=0.5MHz������������Ϊ1/500 000 s
					����Arr=10 000������PWM��һ�����壬T(PWM)=1/500 000*Arr=1/50s=0.02s=20ms				
**************************************************************************/
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)																	//��ʱ�ж�
	{   
		 TIM1->SR&=~(1<<0);                             //�����ʱ��1�жϱ�־λ	
		
					Encoder_Left=Read_Encoder(2);                                       //��ȡ��������ֵ							 //Ϊ�˱�֤M�����ٵ�ʱ���׼�����ȶ�ȡ����������
					Encoder_Right=-Read_Encoder(3);                                      //��ȡ��������ֵ					 
			
					//Angle=0;
					//Velocity = 0;
					Velocity = Incremental_P_DforV(Ultra_Distance,Target_distance);
				
					Kinematic_Analysis(Velocity,Angle);     														//С���˶�ѧ����   

					Motor_A=Incremental_PI_A(Encoder_Left,Target_A);                   //�ٶȱջ����Ƽ�����A����PWM
					Motor_B=Incremental_PI_B(Encoder_Right,Target_B);                  //�ٶȱջ����Ƽ�����B����PWM 
				
					Direction();                                 	 //�����ж�
				
				//���ݷ��򷢳����������ָ��
				if(Flag_direction != Flag_Last_direction)		//�����Ҫ�л����򣬾�����ɲ��
					{	
						Set_Pwm (0,0,SERVO_INIT);				
					}
				else											 									//�������Ҫ�����򣬾ͼ�����									
					{
						Xianfu_Pwm();											//PWM�޷�		
						Set_Pwm(Motor_A,Motor_B,Servo);		
					}
				
					Flag_Last_direction=Flag_direction;    //���·�������
					
	}
} 
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int servo)
{
    	if(motor_a<0)			PWMA1=7200,					PWMA2=7200+motor_a;
			else 	            PWMA1=7200-motor_a,	PWMA2=7200;
		
		  if(motor_b<0)			PWMB1=7200,					PWMB2=7200+motor_b;
			else 	            PWMB1=7200-motor_b,	PWMB2=7200;
     SERVO=servo;	
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=6900;    //===PWM������7200 ������6900
    if(Motor_A<-Amplitude) Motor_A=-Amplitude;	
		if(Motor_A>Amplitude)  Motor_A=Amplitude;	
	  if(Motor_B<-Amplitude) Motor_B=-Amplitude;	
		if(Motor_B>Amplitude)  Motor_B=Amplitude;		
		if(Servo<(SERVO_INIT-500))     Servo=SERVO_INIT-500;	  //����޷�
		if(Servo>(SERVO_INIT+500))     Servo=SERVO_INIT+500;		  //����޷�
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�ɲ������
					����С��ʵ����Ŀ��ֱ���˶�ʱ�ֱ�ʹ��
�޷���ֵ
**************************************************************************/
void Direction()
{
	//�����ж�
	if(Ultra_Distance > Target_distance)
	{
		Flag_direction =1;	//��ʾӦ�����˶�
	}
	else
	{
		Flag_direction =0;	//��ʾӦ�����˶�
	}
}
/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
/**************************************************************************
�������ܣ�����PI������
��ڲ�������Ŀ���ʵʱ���룬��Ŀ����趨����
����  ֵ��С��Ӧ�ñ��ֵ��ٶ�v
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_DforV (int d,int Target)
{ 	
	 static int Bias,v,Last_bias;
	 Bias=Target-d;                //����ƫ��
	 v+=Distance_KP*(Bias-Last_bias)+Distance_KI*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return v;                         //�������
}
/**************************************************************************
�������ܣ�����P������
��ڲ�������Ŀ���ʵʱ���룬��Ŀ����趨����
����  ֵ��С��Ӧ�ñ��ֵ��ٶ�v
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
���ҵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��P����
pwm+=Kp[e��k��-e(k-1)]
**************************************************************************/
 int Incremental_P_DforV(int d,int Target)
 {
	int Bias,v;
	Bias	=	d-Target;
		v		=	Distance_KP*Bias;
	 return v;
 }
 /**************************************************************************
�������ܣ�����PD������
��ڲ�������Ŀ���ʵʱ���룬��Ŀ����趨����
����  ֵ��С��Ӧ�ñ��ֵ��ٶ�v
��������ʽ��ɢPID��ʽ 
v+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
v�����������
**************************************************************************/
int Incremental_PD_DforV (int d,int Target)
{ 	
	 static int Bias,v,Last_bias,Before_Last_bias;
	 Bias=d-Target;                //����ƫ��
	 v+=Distance_KP*(Bias-Last_bias)+Distance_KD*(Bias-(2*Last_bias)+Before_Last_bias) ;   //����ʽPD������
	 Before_Last_bias = Last_bias ;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return v;                         //�������
}


