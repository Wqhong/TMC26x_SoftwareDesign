#include "USART.h"
#include "LED_Key.h"
#include "Probe_Cap.h"
#include "CommonFunc.h"
#include "Probe_Calibrate.h"


u32	TIM5CH1_CAPTURE_VAL = 0;	//���벶��ֵ(TIM2/TIM5��32λ)	
u8  TIM5CH1_CAPTURE_STA = 0;	//���벶��״̬		    				

/****************************************************
��������Probe_Cap_Init(u32 arr,u16 psc)
������arr���Զ���װֵ		psc��ʱ��Ԥ��Ƶ��
���ܣ���ʱ��5ͨ��1���벶�����ã����ģ������������������
����ֵ����
****************************************************/
void Probe_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ��PORTAʱ��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 					//GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 			//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 			//����
	GPIO_Init(GPIOA,&GPIO_InitStructure);								//��ʼ��PA0
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //PA0����λ��ʱ��5
   
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  						//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   							//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	//��ʼ��TIM5���벶�����
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; 							
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 				
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;			  								
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;				//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	TIM5CH1_CAPTURE_VAL = 0;
	TIM5CH1_CAPTURE_STA = 0;
	
	if(Command != 0)
	{
		Command += 0x1000;
		printf("%x \r\n",Command);																		//��������ֵ
		Command = 0;
	}
}


/****************************************************
��������TIM5_IRQHandler(void)
��������
���ܣ���ʱ��5���жϷ�����
����ֵ����
****************************************************/
void TIM5_IRQHandler(void)
{ 		    	
/***************************ÿ�β���һ�ξͷ��ͳ�ȥ************************/	
	if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)		//
	{
		if(TIM5CH1_CAPTURE_STA == 0)
		{
			TIM5CH1_CAPTURE_STA |= 0x80;	//�����һ�������ز���������
			TIM_Cmd(TIM5,DISABLE); 		//�رն�ʱ��5
			TIM_SetCounter(TIM5,0);
			TIM_Cmd(TIM5,ENABLE); 		//ʹ�ܶ�ʱ��5
		}
		else
		{
			TIM5CH1_CAPTURE_VAL = TIM_GetCapture1(TIM5);
			TIM_Cmd(TIM5,DISABLE); 		//�رն�ʱ��5
			TIM_SetCounter(TIM5,0);
			TIM_Cmd(TIM5,ENABLE); 		//ʹ�ܶ�ʱ��5
		}
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); 	//����жϱ�־λ
}


/****************************************************
��������Probe_Cap_DeInit(void)
��������
���ܣ��Ͽ�����״̬
����ֵ����
****************************************************/
void Probe_Cap_DeInit(void)
{
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,DISABLE);
	TIM_Cmd(TIM5,DISABLE);
	TIM_DeInit(TIM5);
	
	TIM5CH1_CAPTURE_VAL = 0;
	TIM5CH1_CAPTURE_STA = 0;
	
	Command += 0x1000;
	printf("%x",Command);
	Command = 0;
}


void StartMeasure(u8 flag)
{
//	TIM5CH1_CAPTURE_STA = (flag == 0? 0 : Filter_N + 1);
	
	TIM_Cmd(TIM5,ENABLE); 																					//ʹ�ܶ�ʱ��5
	TIM_SetCounter(TIM5,0);
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);							//��������ж� ,����CC1IE�����ж�
	
//	if(!flag)
//		while(!(TIM5CH1_CAPTURE_STA == Filter_N + 1));
}





