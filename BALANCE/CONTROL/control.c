#include "control.h"		

/**************************************************************************
函数功能：小车运动数学模型
入口参数：速度和转角
返回  值：无
**************************************************************************/
void Kinematic_Analysis(float velocity,float angle)
{
		Target_A=velocity*(1+T*tan(angle)/2/L); 
		Target_B=velocity*(1-T*tan(angle)/2/L);      //后轮差速
		Servo=SERVO_INIT+angle*K;                    //舵机转向   
}
/**************************************************************************
函数功能：使用TIM1的中断功能，在TIM1发送PWM的同时，其CNT读数溢出arr时产生中断。
注：			TIM1频率36MHz，72分频，计数频率为36M/72=0.5MHz，计数周期遂为1/500 000 s
					由于Arr=10 000，产生PWM的一个脉冲，T(PWM)=1/500 000*Arr=1/50s=0.02s=20ms				
**************************************************************************/
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)																	//定时中断
	{   
		 TIM1->SR&=~(1<<0);                             //清除定时器1中断标志位	
		
					Encoder_Left=Read_Encoder(2);                                       //读取编码器的值							 //为了保证M法测速的时间基准，首先读取编码器数据
					Encoder_Right=-Read_Encoder(3);                                      //读取编码器的值					 
			
					//Angle=0;
					//Velocity = 0;
					Velocity = Incremental_P_DforV(Ultra_Distance,Target_distance);
				
					Kinematic_Analysis(Velocity,Angle);     														//小车运动学分析   

					Motor_A=Incremental_PI_A(Encoder_Left,Target_A);                   //速度闭环控制计算电机A最终PWM
					Motor_B=Incremental_PI_B(Encoder_Right,Target_B);                  //速度闭环控制计算电机B最终PWM 
				
					Direction();                                 	 //方向判断
				
				//根据方向发出电机的最终指令
				if(Flag_direction != Flag_Last_direction)		//如果需要切换方向，就立马刹车
					{	
						Set_Pwm (0,0,SERVO_INIT);				
					}
				else											 									//如果不需要换方向，就继续开									
					{
						Xianfu_Pwm();											//PWM限幅		
						Set_Pwm(Motor_A,Motor_B,Servo);		
					}
				
					Flag_Last_direction=Flag_direction;    //更新方向数据
					
	}
} 
/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
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
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=6900;    //===PWM满幅是7200 限制在6900
    if(Motor_A<-Amplitude) Motor_A=-Amplitude;	
		if(Motor_A>Amplitude)  Motor_A=Amplitude;	
	  if(Motor_B<-Amplitude) Motor_B=-Amplitude;	
		if(Motor_B>Amplitude)  Motor_B=Amplitude;		
		if(Servo<(SERVO_INIT-500))     Servo=SERVO_INIT-500;	  //舵机限幅
		if(Servo>(SERVO_INIT+500))     Servo=SERVO_INIT+500;		  //舵机限幅
}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：刹车换向
					仅在小车实验项目，直线运动时粗暴使用
无返回值
**************************************************************************/
void Direction()
{
	//方向判定
	if(Ultra_Distance > Target_distance)
	{
		Flag_direction =1;	//表示应正向运动
	}
	else
	{
		Flag_direction =0;	//表示应逆向运动
	}
}
/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
/**************************************************************************
函数功能：增量PI控制器
入口参数：与目标的实时距离，与目标的设定距离
返回  值：小车应该保持的速度v
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_DforV (int d,int Target)
{ 	
	 static int Bias,v,Last_bias;
	 Bias=Target-d;                //计算偏差
	 v+=Distance_KP*(Bias-Last_bias)+Distance_KI*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return v;                         //增量输出
}
/**************************************************************************
函数功能：增量P控制器
入口参数：与目标的实时距离，与目标的设定距离
返回  值：小车应该保持的速度v
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我的速度控制闭环系统里面，只使用P控制
pwm+=Kp[e（k）-e(k-1)]
**************************************************************************/
 int Incremental_P_DforV(int d,int Target)
 {
	int Bias,v;
	Bias	=	d-Target;
		v		=	Distance_KP*Bias;
	 return v;
 }
 /**************************************************************************
函数功能：增量PD控制器
入口参数：与目标的实时距离，与目标的设定距离
返回  值：小车应该保持的速度v
根据增量式离散PID公式 
v+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
v代表增量输出
**************************************************************************/
int Incremental_PD_DforV (int d,int Target)
{ 	
	 static int Bias,v,Last_bias,Before_Last_bias;
	 Bias=d-Target;                //计算偏差
	 v+=Distance_KP*(Bias-Last_bias)+Distance_KD*(Bias-(2*Last_bias)+Before_Last_bias) ;   //增量式PD控制器
	 Before_Last_bias = Last_bias ;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return v;                         //增量输出
}


