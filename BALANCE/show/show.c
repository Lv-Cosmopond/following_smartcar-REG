#include "show.h"

unsigned char i;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
float Vol;
/**************************************************************************
�������ܣ�OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
void oled_show(void)
{
	//��0�С���1��
	//��ʾ��	Target Distance
	//					1000		mm
		 OLED_ShowString(00,00,"Target Distance");
		 OLED_ShowNumber(40,10,Target_distance,4,12);
		 OLED_ShowString(80,10,"mm");
	
	//��2�С���3��
	//��ʾ��	Now Distance	
	//					???		mm
	OLED_ShowString(00,20,"Now Distance");
	if(Ultra_Distance==Target_distance)		
	{
		OLED_ShowNumber(40,30,0,4,12);
	}
	else	
	{
		OLED_ShowNumber(40,30,Ultra_Distance,4,12);
		OLED_ShowString(80,30,"mm");
	}	
	
	//��4�С���5��                                                                                                                   
	//��ʾ��Mode:		Measure/Drive
		OLED_ShowString(00,50,"Mode:");
		if(Flag_EXTI == 1)		OLED_ShowString(50,50,"Measure");
		else 									OLED_ShowString(50,50,"Driving");
		
		//=============ˢ��=======================//
		OLED_Refresh_Gram();	
	}


void OLED_DrawPoint_Shu(u8 x,u8 y,u8 t)
{ 
	 u8 i=0;
  OLED_DrawPoint(x,y,t);
	OLED_DrawPoint(x,y,t);
	  for(i = 0;i<8; i++)
  {
      OLED_DrawPoint(x,y+i,t);
  }
}
