#include "Encode.h"
	
	
void Encoder_Init(void)
{
	GPIO_InitTypeDef  	Encoder_InitStructure;	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	Encoder_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	Encoder_InitStructure.GPIO_Mode = GPIO_Mode_IN;						
	Encoder_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	Encoder_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				
	GPIO_Init(GPIOA, &Encoder_InitStructure);							
	
	
	EXTI_InitTypeDef   Encode_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);//PA5 ���ӵ��ж���5
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);//PA6 ���ӵ��ж���6
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);//PA7 ���ӵ��ж���7
	
  Encode_InitStructure.EXTI_Line = EXTI_Line5 | EXTI_Line6 | EXTI_Line7;
  Encode_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ж��¼�
  Encode_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 	//�����ش��� 
  Encode_InitStructure.EXTI_LineCmd = ENABLE;			
  EXTI_Init(&Encode_InitStructure);		
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;					//�ⲿ�ж�5-7
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;				//�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//�ⲿ�ж�5-7�������			��������ش���
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR == EXTI_Line5)
	{		 
		LED=!LED;			 
		EXTI_ClearITPendingBit(EXTI_Line5);//���LINE5�ϵ��жϱ�־λ  	
	}
	if(EXTI->PR == EXTI_Line6)
	{			 
		LED=!LED;				 
		EXTI_ClearITPendingBit(EXTI_Line6);//���LINE6�ϵ��жϱ�־λ  	
	}
	if(EXTI->PR == EXTI_Line7)
	{		 
		LED=!LED;		 
		EXTI_ClearITPendingBit(EXTI_Line7);//���LINE6�ϵ��жϱ�־λ  
	}
}

