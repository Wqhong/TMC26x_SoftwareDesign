#include "main.h"

int main(void)
{  	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	IWDG_Init(4,500);//与分频数为64,重载值为500,溢出时间为1s
	
	Delay_init(168);  	
	USART2_Init(115200);
	SystemTim_Init(50-1,8400-1);
	
	Adc_Init();
	Encoder_Init();			
	LED_KEY_Init();
	GPIO_InitAll();
	
	Drivers_EnAndCsn_Init();
	Drivers_ENABLE(0);
	
	if(OLED == 1)
	{
		IIC_Init();
		OLED_Init();
		OLED_Clear();
		OLED_ShowString(0,0,"HardwareDesignV2");
	}
	
	FrequencyStd2Coefficient_ReadFromFlash();
	
	while(1)
	{
		if(UpdateCoefficientFlag)
		{
			FrequencyStd2Coefficient_ReadFromFlash();	//建立网络连接需要时间4-5s，不打印预校准值
			UpdateCoefficientFlag = 0;
			__set_FAULTMASK(1);
			NVIC_SystemReset();
		}
		
		USART_SendBack(0);
		
		switch(Command)
		{		
			case 0xa001:	Get_VoltageAndTemprate(); 
			
			case 0x4001:	Transmitter = 1; printf("%x",Command);	LED = !LED; Command = 0;	break;
			case 0x4002:	Transmitter = 0; printf("%x",Command);	LED = !LED; Command = 0;	break;
			
			case 0x4003:	Webcam1 = 1, Webcam2 = 0, Webcam3 = 0; printf("%x",Command);	Command = 0;	break;		
			case 0x4004:	Webcam1 = 0, Webcam2 = 1, Webcam3 = 0; printf("%x",Command);	Command = 0;	break;
			case 0x4005:	Webcam1 = 0, Webcam2 = 0, Webcam3 = 1; printf("%x",Command);	Command = 0;	break;										
			
			case 0x4006:	Probe_Cap_Init(0XFFFFFFFF,84-1);StartMeasure(1); CALI_STA = 0x4000; Command = 0; break;
			case 0x4007:	Probe_Cap_Init(0XFFFFFFFF,84-1);StartMeasure(1); CALI_STA = 0x80; Command = 0; break;											
			
			case 0x6001:	Driver4A_GoForward();	LED = !LED;	break;		
			case 0x6002:	Driver4A_GoBack();	LED = !LED;	break;									
			case 0x6003:	AllDriver_Stop();	LED = !LED;	break;			
			case 0x6004:	Driver4A_SpeedCut();	LED = !LED;	break;							
			case 0x6005:	Driver4A_SpeedUp();	LED = !LED;	break;								
			
			case 0x8001: 	Driver2A1_Rolate(0); 	LED = !LED;	break;	
			case 0x8002: 	Driver2A1_ReSet(); 	LED = !LED;	break;	//Driver2A1_Rolate(1);
			case 0x8003: 	Driver2A1_Rolate(1);	LED = !LED;	break;
			case 0x8004: 	Driver2A1_ReSet();	LED = !LED;	break;
					
			case 0x8005: 	Driver2A2_PushOut();	LED = !LED;	break;	
			case 0x8006: 	Driver2A2_ReSet();	LED = !LED;	break;	
									 
									 
			case 0x8007: 	Probe_Cap_Init(0XFFFFFFFF,84-1);	CALI_STA = 0; LED = !LED;  break;							
			case 0x8008: 	StartMeasure(1);	Get_Probe_Value(); LED = !LED; break;								
			case 0x8009: 	Probe_Cap_DeInit();	LED = 0; 	break;	
										
			
			case 0x8000:	CALI_STA |= 0x20; Command = 0; break;			//记录一次有效的校准值
			case 0x8080:	CALI_STA = 0; printf("退出校准模式\r\n"); Command = 0; break;	
			
//			case 0x8888:	CALI_STA |= 0x40; Command = 0; break;			//将按键调整过的校准值保存
//			case 0x9001:  CaliSendToUart();Command = 0; break;
			
			default:			Command = 0;	break;
		}
		
		IWDG_ReloadCounter(); 
	}
}

