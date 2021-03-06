#include "exti.h"

/**************************************************************************
函数功能：外部中断初始化
入口参数：无
返回  值：无 
**************************************************************************/
void EXTI_KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   	 
	GPIOA->CRL&=0XFF0FFFFF; 
	GPIOA->CRL|=0X00800000;//PA5上拉输入
  GPIOA->ODR|=1<<5;     //PA5上拉	
	Ex_NVIC_Config(GPIO_A,5,FTIR);		//下降沿触发
	MY_NVIC_Init(2,1,EXTI9_5_IRQn,2);  	//抢占2，子优先级1，组2
}



int EXTI9_5_IRQHandler(void) 
{    
	 if(KEY==0)		
	{   
		  EXTI->PR=1<<5;   
			Flag_EXTI=0;
	}
}






