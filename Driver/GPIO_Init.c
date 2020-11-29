#include "GPIO_Init.h"


void Webcam_Init(void)
{
	//GPIOE12��E13��14��ʼ������Ϊ3������ͷ�Ŀ����ź�
	GPIO_InitTypeDef  	Webcam_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	
	Webcam_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	Webcam_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				
	Webcam_InitStructure.GPIO_OType = GPIO_OType_PP;			
	Webcam_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		
	Webcam_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				
	GPIO_Init(GPIOE, &Webcam_InitStructure);						
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);	
}	


void ProbeRst_Init(void)
{
	//GPIOE15��ʼ������Ϊ̽ͷ��λ�ļ���ź�
	GPIO_InitTypeDef  	ProbeRst_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	
	ProbeRst_InitStructure.GPIO_Pin = GPIO_Pin_15;
	ProbeRst_InitStructure.GPIO_Mode = GPIO_Mode_IN;					
	ProbeRst_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		
	ProbeRst_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				
	GPIO_Init(GPIOE, &ProbeRst_InitStructure);							
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_15);	
}


void RolateRst_Init(void)
{
	//GPIOA11��ʼ������Ϊ̽ͷ��ת�Ĺ�λ�ź�
	GPIO_InitTypeDef  	RolateRst_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	RolateRst_InitStructure.GPIO_Pin = GPIO_Pin_11;
	RolateRst_InitStructure.GPIO_Mode = GPIO_Mode_IN;					
	RolateRst_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		
	RolateRst_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				
	GPIO_Init(GPIOA, &RolateRst_InitStructure);							
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);		
}


void Transmitter_Init(void)
{
	//GPIOB12��ʼ������Ϊ�����Ŷ�λ������Ŀ����ź�
	GPIO_InitTypeDef  	Transmitter_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	Transmitter_InitStructure.GPIO_Pin = GPIO_Pin_12;
	Transmitter_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			
	Transmitter_InitStructure.GPIO_OType = GPIO_OType_PP;			
	Transmitter_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		
	Transmitter_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				
	GPIO_Init(GPIOB, &Transmitter_InitStructure);							
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);	
}

void GPIO_InitAll(void)
{
	Webcam_Init();
	ProbeRst_Init();
	RolateRst_Init();
	Transmitter_Init();
}
