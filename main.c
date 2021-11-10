#include "sys.h"

/***********************************************************************************/
//�����̵ĸ����ļ�֮�䣬�д����Ĺ��ñ�����Ϊ�˷���༭
//�ز�ȡmain.c-sys.h�ķ�ʽ����main.c�ﶨ�壬��sys.h��ʹ���ⲿ����extern
//�����������ļ�������ͳһʹ�á�#include "sys.h"����������ҪƵ���޸�include xxxx 
/***********************************************************************************/
//control///////////////////////////////////////////////////
u8 Flag_direction,Flag_Last_direction;			//��������
float Velocity=0;
float Angle=0;
float Velocity_KP=12,Velocity_KI=12;
float Distance_KP=0.15,Distance_KI=1,Distance_KD=1;
int 	Target_distance = 500;		//��λmm
//motor/////////////////////////////////////////////////////
int Motor_A,Motor_B,Servo,Target_A,Target_B;  //������������� 
//ultrasonic///////////////////////////////////////////////
unsigned char   dat[3];							//��ų��������ڽ��յ��ֽ���
unsigned char   num;			 						//���������ݽ��ձ���
int             Ultra_Distance=1000;							//��������ĳ��������,�����Target_distanceһ���ĳ�ʼֵ��ʹ���ʼ��ʱ����
int							Distance=0;								//ʵʱ�������
//encoder/////////////////////////////////////////////////////
int Encoder_Left,Encoder_Right;             //���ұ��������������
//key
// volatile u8 Flag_Mode=0;									//ģʽ��־��0-���ģʽ��1-��ʻģʽ
volatile u8 Flag_EXTI=1;
volatile u8 Flag_Click=0;
volatile u8 Bottom_StatusTemp=0, Bottom_Status1=0,	Bottom_Status2=0,	Bottom_Status3=0;	//����ɨ������
/***********************************************************************************/

//С����ʻָ����TIM1���жϣ���ʱ���£�
//���Ҳ��TIM1���ƣ��ʳ�ʼ�����Servo�ͻῪʼ����С��
//��������ʼ���������ڣ����Servo��ʼ��֮ǰ����Ϊ��ʻָ���Ҫ���ݾ��뷴����
//����������ʻָ��ģ���(Ŀǰ����-Ŀ�����);

int main(void)
{
	Stm32_Clock_Init(9);            //ϵͳʱ������
	NVIC_Configuration();					 	//�ж����ȼ�����
	delay_init(72);                 //��ʱ��ʼ��
	
	JTAG_Set(JTAG_SWD_DISABLE);			//�ر�JTAG�ӿڣ���ΪҪʹ����ռ�õ�IO�ڣ����˲�����OLED��
	OLED_Init();										//��ʼ��OLED��
	
	KEY_Init();											//��ʼ������
	EXTI_KEY_Init();								//��ʼ���ⲿ�ж�(����)
	
	ultrasonic_init(115200);				//��ʼ��������������
	
	while(Flag_EXTI)
	{
		oled_show();
	}
	
	/******************************	���ϣ�ֻ���о��������ֱ�����º󣬽����ʻģʽ*******************************/
	
	Encoder_Init_TIM2();            //��ʼ����������TIM2�ı������ӿ�ģʽ�� 
	Encoder_Init_TIM3();            //��ʼ����������TIM3�ı������ӿ�ģʽ�� 	

	Motor_PWM_Init(7199,0);  				//��ʼ��PWM 10KHZ������������� 
	
	Servo_PWM_Init(9999,71);   			//��ʼ��PWM50HZ���� ���

	uart1_init(115200);	           //����1��ʼ��������PC�ϵ��Դ���
	
  while(1)	
	{
		oled_show();
		
		//printf("ultrasonic distance=%d\r\n",Ultra_Distance);		//PC�ϵ��Գ�������������
		//delay_ms(100);
	} 	
	
}
