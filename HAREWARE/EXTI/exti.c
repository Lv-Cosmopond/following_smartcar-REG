#include "exti.h"

/**************************************************************************
�������ܣ��ⲿ�жϳ�ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/
void EXTI_KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   	 
	GPIOA->CRL&=0XFF0FFFFF; 
	GPIOA->CRL|=0X00800000;//PA5��������
  GPIOA->ODR|=1<<5;     //PA5����	
	Ex_NVIC_Config(GPIO_A,5,FTIR);		//�½��ش���
	MY_NVIC_Init(2,1,EXTI9_5_IRQn,2);  	//��ռ2�������ȼ�1����2
}



int EXTI9_5_IRQHandler(void) 
{    
	 if(KEY==0)		
	{   
		  EXTI->PR=1<<5;   
			Flag_EXTI=0;
	}
}





