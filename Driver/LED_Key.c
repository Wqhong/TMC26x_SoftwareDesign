#include "LED_Key.h" 
	 

//初始化PE7为输出口.并使能这个端口的时钟		    
void LED_KEY_Init(void)
{    	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOE时钟
	
	//GPIOE2、GPIOE3初始化为按键1、按键2	
	GPIO_InitTypeDef  Key_InitStructure;
	Key_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	Key_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//普通输入模式
	Key_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	Key_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOE, &Key_InitStructure);
	
	//GPIOE7为LED	
	GPIO_InitTypeDef  LED_InitStructure;
	LED_InitStructure.GPIO_Pin = GPIO_Pin_7;
	LED_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	LED_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	LED_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	LED_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOE, &LED_InitStructure);						//初始化
	
	GPIO_SetBits(GPIOE,GPIO_Pin_7);									//GPIOE7设置高，灯灭
}


u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;		//按键按松开标志
	if(mode)key_up=1;  		//支持连按		  
	if(key_up&&(KEY0==0||KEY1==0))
	{
		Delay_ms(10);				//去抖动 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(KEY0==0&&KEY1==0)return 0xFF;
	}
	else if(KEY0==1&&KEY1==1)key_up=1; 	    
 	return 0;							// 无按键按下
}

