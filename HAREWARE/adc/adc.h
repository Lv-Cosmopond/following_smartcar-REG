#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

#define Battery_Ch 9

void Dly_us(void);
void Adc_Init(void);
u16 Get_Adc(u8 ch);
int Get_battery_volt(void);   


#endif 

