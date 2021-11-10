#include "show.h"

unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
float Vol;
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
void oled_show(void)
{
	//第0行、第1行
	//显示：	Target Distance
	//					1000		mm
		 OLED_ShowString(00,00,"Target Distance");
		 OLED_ShowNumber(40,10,Target_distance,4,12);
		 OLED_ShowString(80,10,"mm");
	
	//第2行、第3行
	//显示：	Now Distance	
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
	
	//第4行、第5行                                                                                                                   
	//显示：Mode:		Measure/Drive
		OLED_ShowString(00,50,"Mode:");
		if(Flag_EXTI == 1)		OLED_ShowString(50,50,"Measure");
		else 									OLED_ShowString(50,50,"Driving");
		
		//=============刷新=======================//
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
