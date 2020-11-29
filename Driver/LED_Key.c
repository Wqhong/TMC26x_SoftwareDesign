#include "LED_Key.h" 
	 

//��ʼ��PE7Ϊ�����.��ʹ������˿ڵ�ʱ��		    
void LED_KEY_Init(void)
{    	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��
	
	//GPIOE2��GPIOE3��ʼ��Ϊ����1������2	
	GPIO_InitTypeDef  Key_InitStructure;
	Key_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	Key_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//��ͨ����ģʽ
	Key_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	Key_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOE, &Key_InitStructure);
	
	//GPIOE7ΪLED	
	GPIO_InitTypeDef  LED_InitStructure;
	LED_InitStructure.GPIO_Pin = GPIO_Pin_7;
	LED_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	LED_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	LED_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	LED_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOE, &LED_InitStructure);						//��ʼ��
	
	GPIO_SetBits(GPIOE,GPIO_Pin_7);									//GPIOE7���øߣ�����
}


u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;		//�������ɿ���־
	if(mode)key_up=1;  		//֧������		  
	if(key_up&&(KEY0==0||KEY1==0))
	{
		Delay_ms(10);				//ȥ���� 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(KEY0==0&&KEY1==0)return 0xFF;
	}
	else if(KEY0==1&&KEY1==1)key_up=1; 	    
 	return 0;							// �ް�������
}

