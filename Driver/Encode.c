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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);//PA5 连接到中断线5
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);//PA6 连接到中断线6
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);//PA7 连接到中断线7
	
  Encode_InitStructure.EXTI_Line = EXTI_Line5 | EXTI_Line6 | EXTI_Line7;
  Encode_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断事件
  Encode_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 	//上升沿触发 
  Encode_InitStructure.EXTI_LineCmd = ENABLE;			
  EXTI_Init(&Encode_InitStructure);		
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;					//外部中断5-7
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;				//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//外部中断5-7服务程序			编码器相关处理
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR == EXTI_Line5)
	{		 
		LED=!LED;			 
		EXTI_ClearITPendingBit(EXTI_Line5);//清除LINE5上的中断标志位  	
	}
	if(EXTI->PR == EXTI_Line6)
	{			 
		LED=!LED;				 
		EXTI_ClearITPendingBit(EXTI_Line6);//清除LINE6上的中断标志位  	
	}
	if(EXTI->PR == EXTI_Line7)
	{		 
		LED=!LED;		 
		EXTI_ClearITPendingBit(EXTI_Line7);//清除LINE6上的中断标志位  
	}
}

