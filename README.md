# following_smartcar-REG

Mainly coding with Registers instead of  STM32 Library functions to build a KEIL5 project.

本项目为4轮跟随小车;

主要使用寄存器编程，掺杂少量库函数;
sys文件由正点原子提供模板，该模板是库函数模板，方便库函数编程;

该小车使用舵机转向，直流减速电机驱动;用收发异体的超声波模块进行通信，实现小车的跟随功能;并在0.96寸OLED屏（SPI）显示距离信息;
