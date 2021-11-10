#include "sys.h"

/***********************************************************************************/
//本工程的各个文件之间，有大量的共用变量。为了方便编辑
//特采取main.c-sys.h的方式：在main.c里定义，在sys.h里使用外部声明extern
//这样，其他文件，可以统一使用【#include "sys.h"】，而不需要频繁修改include xxxx 
/***********************************************************************************/
//control///////////////////////////////////////////////////
u8 Flag_direction,Flag_Last_direction;			//方向数据
float Velocity=0;
float Angle=0;
float Velocity_KP=12,Velocity_KI=12;
float Distance_KP=0.15,Distance_KI=1,Distance_KD=1;
int 	Target_distance = 500;		//单位mm
//motor/////////////////////////////////////////////////////
int Motor_A,Motor_B,Servo,Target_A,Target_B;  //电机舵机控制相关 
//ultrasonic///////////////////////////////////////////////
unsigned char   dat[3];							//存放超声波串口接收的字节数
unsigned char   num;			 						//超声波数据接收变量
int             Ultra_Distance=1000;							//计算出来的超声波测距,给予和Target_distance一样的初始值，使其初始化时，别动
int							Distance=0;								//实时地面距离
//encoder/////////////////////////////////////////////////////
int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数
//key
// volatile u8 Flag_Mode=0;									//模式标志，0-测距模式，1-驾驶模式
volatile u8 Flag_EXTI=1;
volatile u8 Flag_Click=0;
volatile u8 Bottom_StatusTemp=0, Bottom_Status1=0,	Bottom_Status2=0,	Bottom_Status3=0;	//按键扫描序列
/***********************************************************************************/

//小车驾驶指令由TIM1的中断，定时更新；
//舵机也由TIM1控制，故初始化舵机Servo就会开始驱动小车
//超声波初始化，必须在，舵机Servo初始化之前，因为驾驶指令，需要根据距离反馈。
//而反馈给驾驶指令的，是(目前距离-目标距离);

int main(void)
{
	Stm32_Clock_Init(9);            //系统时钟设置
	NVIC_Configuration();					 	//中断优先级分组
	delay_init(72);                 //延时初始化
	
	JTAG_Set(JTAG_SWD_DISABLE);			//关闭JTAG接口，因为要使用其占用的IO口，关了才能用OLED屏
	OLED_Init();										//初始化OLED屏
	
	KEY_Init();											//初始化按键
	EXTI_KEY_Init();								//初始化外部中断(按键)
	
	ultrasonic_init(115200);				//初始化超声波接收器
	
	while(Flag_EXTI)
	{
		oled_show();
	}
	
	/******************************	以上，只进行距离测量；直到按下后，进入驾驶模式*******************************/
	
	Encoder_Init_TIM2();            //初始化编码器（TIM2的编码器接口模式） 
	Encoder_Init_TIM3();            //初始化编码器（TIM3的编码器接口模式） 	

	Motor_PWM_Init(7199,0);  				//初始化PWM 10KHZ，用于驱动电机 
	
	Servo_PWM_Init(9999,71);   			//初始化PWM50HZ驱动 舵机

	uart1_init(115200);	           //串口1初始化，用于PC上调试串口
	
  while(1)	
	{
		oled_show();
		
		//printf("ultrasonic distance=%d\r\n",Ultra_Distance);		//PC上调试超声波串口所用
		//delay_ms(100);
	} 	
	
}
