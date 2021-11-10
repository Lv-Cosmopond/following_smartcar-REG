#include "servo.h"

/************************************************
函数名称：舵机初始化函数
函数功能：产生作用于舵机，使其偏转的PWM
函数思路：使用TIM1的捕获/比较寄存器，通道CH4，来产生PWM
入口参数：	arr	TIM1的自动重装值
						psc	TIM1的预分频数
无返回值
************************************************/
void Servo_PWM_Init(u16 arr,u16 psc)
{
	//一、使能 TIM1 时钟、GPIO口时钟
	RCC->APB2ENR |= 1<<11;	
	RCC->APB2ENR |=1<<2;        //PORTA时钟使能 
	
	//二、配置对应引脚PA11(TIM1_CH4)的复用输出功能
	GPIOA->CRH&=0XFFFF0FFF;    //PORTA11模式清0
	GPIOA->CRH|=0X0000B000;    //PORTA11复用输出（复用功能之一，TIM1_CH4）
	
	//三、设定重装载值、是否分频
	TIM1->ARR = arr;				//设置 TIM1 的重装载值
	TIM1->PSC = psc;				//设置 TIM1 的预分频，0=不分频
	
	//四、设置PWM模式
	TIM1->CCMR2 |= 6<<12;		//CH4 [6:4]=110，PWM模式1 ；向上计数，TIMx_CNT < TIMx_CCR1时通道1为有效电平
	TIM1->CCMR2 |= 1<<11;		//CH4 预装载使能：启动影子寄存器
	
	//五、输出使能
	TIM1->CCER  |= 1<<12;		//CH4 输出使能
	
	//六、主动重装载预装载允许位(ARPE) 及 定时器 使能
	TIM1->BDTR  |= 1<<15;		//开启OC信号输出到对应引脚 TIM1要有这句才输出PWM
	TIM1->CR1 = 0x0080;				//7th=1;TIM_ARR被装入缓冲器，ARPE使能
	TIM1->DIER |= 1<<0;			//允许中断的更新
	TIM1->CCR4 = 1500;			//死区时间设置
	TIM1->CR1 |= 0x01;			//使能定时器1
	
	MY_NVIC_Init(1,1,TIM1_UP_IRQn,2);	//主优先级1，响应优先级1，TIM1中断，中断分组2
}
