#include "Driver_Init.h"

//#include "IIC.h"
//#include "Oled.h"

void Drivers_EnAndCsn_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef	EN2A1_InitStructure;									//EN_2A1
	EN2A1_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	//CSN_2A1		EN_2A1
	EN2A1_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	EN2A1_InitStructure.GPIO_OType = GPIO_OType_PP;			
	EN2A1_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;		
	EN2A1_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					
	GPIO_Init(GPIOB, &EN2A1_InitStructure);

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef	EN4A_InitStructure;				
	EN4A_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 	//EN_2A2	CSN_2A2	
															| GPIO_Pin_7 | GPIO_Pin_6 		//EN_4A1	CSN_4A1
															|	GPIO_Pin_15;								//EN_4A2
	EN4A_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	EN4A_InitStructure.GPIO_OType = GPIO_OType_PP;			
	EN4A_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;		
	EN4A_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				
	GPIO_Init(GPIOD, &EN4A_InitStructure);
		
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef	CSN_InitStructure;
	CSN_InitStructure.GPIO_Pin = GPIO_Pin_6;
	CSN_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	CSN_InitStructure.GPIO_OType = GPIO_OType_PP;			
	CSN_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;		
	CSN_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				
	GPIO_Init(GPIOC, &CSN_InitStructure);	
	
	CSN_2A1 = 1;
	CSN_2A2 = 1;
	CSN_4A1 = 1;		
	CSN_4A2 = 1;
}

void Drivers_ENABLE(u8 D2AorD4A)
{
	if(D2AorD4A == 1)
	{
		EN_2A1 = 0;	
		EN_2A2 = 1;
		EN_4A1 = 1;
		EN_4A2 = 1;
	}
	else if(D2AorD4A == 2)
	{
		EN_2A1 = 1;	
		EN_2A2 = 0;
		EN_4A1 = 1;
		EN_4A2 = 1;		
	}
	else if(D2AorD4A == 4)
	{
		EN_2A1 = 1;	
		EN_2A2 = 1;
		EN_4A1 = 0;
		EN_4A2 = 0;		
	}
	else
	{
		EN_2A1 = 1;	
		EN_2A2 = 1;
		EN_4A1 = 1;
		EN_4A2 = 1;		
	}
}



void Driver_SPI1_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef	SPI_InitStructure;
	SPI_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;		//TMC_SCK TMC_SDI
	SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	SPI_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	SPI_InitStructure.GPIO_OType = GPIO_OType_PP;
	SPI_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&SPI_InitStructure);

	SPI_InitStructure.GPIO_Pin = GPIO_Pin_11;									//TMC_SDO
	SPI_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	SPI_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	SPI_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC,&SPI_InitStructure);	
	
	SCK1 = 1;																									//SCK初始化为高
}
void Driver_SPI2_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef	SPI_InitStructure;
	SPI_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;		//TMC_SCK TMC_SDI
	SPI_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	SPI_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	SPI_InitStructure.GPIO_OType = GPIO_OType_PP;
	SPI_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&SPI_InitStructure);

	SPI_InitStructure.GPIO_Pin = GPIO_Pin_14;									//TMC_SDO
	SPI_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	SPI_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	SPI_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB,&SPI_InitStructure);	
	
	SCK2 = 1;																						//SCK初始化为高
}



uint32_t Driver_Config1(u8 D2AorD4A, uint32_t write_data)
{
	unsigned char i;
	u32 data_temp,recv_data= 0;
	
	SCK1 = 1;
	Delay_us(1);
	if(D2AorD4A == 2)CSN_2A1 = 0;
	else if(D2AorD4A == 4)CSN_4A1 = 0;
	else return 1;
	__nop();
	Delay_us(1);
	for(i = 20; i > 0; i--)
	{
		recv_data = recv_data << 1;	
		data_temp = write_data & 0x80000;
		data_temp = data_temp >> 19; 
		if(data_temp == 1)
			SDI1 = 1;				//SDI
		else
			SDI1 = 0;
		SCK1 = 0;					//SCK
		Delay_us(2);
		SCK1 = 1;					//SCK
		Delay_us(1);
		if(SDO1)						//SDO
			recv_data |= 0x01;		
		write_data = write_data << 1;
		Delay_us(2);
	}	
//	OLED_ShowNum(0,2,recv_data,7);
	if(D2AorD4A == 2)			CSN_2A1 = 1;
	else if(D2AorD4A == 4)CSN_4A1 = 1;
	__nop();
	
	return recv_data;
}
uint32_t Driver_Config2(u8 D2AorD4A, uint32_t write_data)
{
	unsigned char i;
	u32 data_temp,recv_data= 0;
	
	SCK2 = 1;
	Delay_us(1);
	if(D2AorD4A == 2)CSN_2A2 = 0;
	else if(D2AorD4A == 4)CSN_4A2 = 0;
	else return 1;
	__nop();
	Delay_us(1);
	for(i = 20; i > 0; i--)
	{
		recv_data = recv_data << 1;	
		data_temp = write_data & 0x80000;
		data_temp = data_temp >> 19; 
		if(data_temp == 1)
		{
			SDI2 = 1;				//SDI
		}
		else
			SDI2 = 0;
		SCK2 = 0;					//SCK
		Delay_us(2);
		SCK2 = 1;					//SCK
		Delay_us(1);
		if(SDO2)						//SDO
			recv_data |= 0x01;		
		write_data = write_data << 1;
		Delay_us(2);
	}	
//	OLED_ShowNum(0,2,recv_data,7);
	if(D2AorD4A == 2)			CSN_2A2 = 1;
	else if(D2AorD4A == 4)CSN_4A2 = 1;
	__nop();
	
	return recv_data;	
}



void Driver_2A1_Control_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef	Step_Dir;
	Step_Dir.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	Step_Dir.GPIO_Speed = GPIO_Speed_50MHz;
	Step_Dir.GPIO_Mode = GPIO_Mode_OUT;
	Step_Dir.GPIO_OType = GPIO_OType_PP;
	Step_Dir.GPIO_PuPd = GPIO_PuPd_UP;							//核心板只能上拉
	GPIO_Init(GPIOD,&Step_Dir);
	
	STEP_2A1 = 1;
	DIR_2A1 = 1;
}
void Driver_2A2_Control_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef	Step_Dir;
	Step_Dir.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	Step_Dir.GPIO_Speed = GPIO_Speed_50MHz;
	Step_Dir.GPIO_Mode = GPIO_Mode_OUT;
	Step_Dir.GPIO_OType = GPIO_OType_PP;
	Step_Dir.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&Step_Dir);
	
	STEP_2A2 = 1;
	DIR_2A2 = 1;
}
void Driver_4A1_Control_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef	Step_Dir;
	Step_Dir.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	Step_Dir.GPIO_Speed = GPIO_Speed_50MHz;
	Step_Dir.GPIO_Mode = GPIO_Mode_OUT;
	Step_Dir.GPIO_OType = GPIO_OType_PP;
	Step_Dir.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&Step_Dir);
	
	STEP_4A1 = 1;
	DIR_4A1 = 1;
}
void Driver_4A2_Control_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef	Step_Dir;
	Step_Dir.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	Step_Dir.GPIO_Speed = GPIO_Speed_50MHz;
	Step_Dir.GPIO_Mode = GPIO_Mode_OUT;
	Step_Dir.GPIO_OType = GPIO_OType_PP;
	Step_Dir.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&Step_Dir);
	
	STEP_4A2 = 1;
	DIR_4A2 = 1;
}
	


void Driver_2A1_Init(void)
{
	Drivers_ENABLE(Driver2A1);
	Driver_SPI1_Init();
	Driver_2A1_Control_Init();

	Driver_Config1(2,0x00004);				//DRVCTRL
	Driver_Config1(2,0x901B4);				//CHOPCONF
	Driver_Config1(2,0xA8202);				//SMARTEN
	Driver_Config1(2,0xD0008);				//SGCSCONF		0xD001F 电流加倍
	Driver_Config1(2,0xE0050);				//CRVCONF			0xE0050	电流减半
}
void Driver_2A2_Init(void)
{
	Drivers_ENABLE(Driver2A2);
	Driver_SPI2_Init();
	Driver_2A2_Control_Init();

	Driver_Config2(2,0x00004);				//DRVCTRL
	Driver_Config2(2,0x901B4);				//CHOPCONF
	Driver_Config2(2,0xA8202);				//SMARTEN
	Driver_Config2(2,0xD0008);				//SGCSCONF		0xD001F 电流加倍
	Driver_Config2(2,0xE0050);				//CRVCONF			0xE0050	电流减半	
}
void Driver_4A_Init(void)
{
	Drivers_ENABLE(Driver4A);
	
	Driver_SPI1_Init();
	Driver_4A1_Control_Init();
	Driver_Config1(4,0x00004);				//DRVCTRL
	Driver_Config1(4,0x901B4);				//CHOPCONF
	Driver_Config1(4,0xA8202);				//SMARTEN
	Driver_Config1(4,0xD0010);				//SGCSCONF		0xD001F 电流加倍
	Driver_Config1(4,0xE0050);				//CRVCONF			0xE0010	0xE0050(电流减半)
	
	Driver_SPI2_Init();
	Driver_4A2_Control_Init();
	Driver_Config2(4,0x00004);				//DRVCTRL
	Driver_Config2(4,0x901B4);				//CHOPCONF
	Driver_Config2(4,0xA8202);				//SMARTEN
	Driver_Config2(4,0xD0010);				//SGCSCONF		0xD001F 电流加倍
	Driver_Config2(4,0xE0050);				//CRVCONF			0xE0010	0xE0050(电流减半)
}


//void Driver_4A1_Init(void)
//{
//	Driver_4A1_ENABLE();
//	Driver_SPI1_Init();
//	Driver_4A1_Control_Init();

//	Driver_Config1(4,0x00004);				//DRVCTRL
//	Driver_Config1(4,0x901B4);				//CHOPCONF
//	Driver_Config1(4,0xA8202);				//SMARTEN
//	Driver_Config1(4,0xD0010);				//SGCSCONF		0xD001F 电流加倍
//	Driver_Config1(4,0xE0050);				//CRVCONF			0xE0010	0xE0050(电流减半)
//}
//void Driver_4A2_Init(void)
//{
//	Driver_4A2_ENABLE();
//	Driver_SPI2_Init();
//	Driver_4A2_Control_Init();

//	Driver_Config2(4,0x00004);				//DRVCTRL
//	Driver_Config2(4,0x901B4);				//CHOPCONF
//	Driver_Config2(4,0xA8202);				//SMARTEN
//	Driver_Config2(4,0xD0010);				//SGCSCONF		0xD001F 电流加倍
//	Driver_Config2(4,0xE0050);				//CRVCONF			0xE0010	0xE0050(电流减半)
//}



//void Driver_2A1_ENABLE(void)
//{
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//	GPIO_InitTypeDef	EN_InitStructure;										//EN_2A1
//	EN_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
//	EN_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	EN_InitStructure.GPIO_OType = GPIO_OType_PP;			
//	EN_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;		
//	EN_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;						//核心板只能上拉
//	GPIO_Init(GPIOB, &EN_InitStructure);	
//	
//	EN_2A1 = 0;	
//	EN_2A2 = 1;
//	EN_4A1 = 1;
//	EN_4A2 = 1;
//	CSN_2A1 = 1;	
//}
//void Driver_2A2_ENABLE(void)
//{
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//	GPIO_InitTypeDef	EN_InitStructure;				//EN
//	EN_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12;
//	EN_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	EN_InitStructure.GPIO_OType = GPIO_OType_PP;			
//	EN_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;		
//	EN_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;				
//	GPIO_Init(GPIOD, &EN_InitStructure);

//	EN_2A1 = 1;	
//	EN_2A2 = 0;
//	EN_4A1 = 1;
//	EN_4A2 = 1;
//	CSN_2A2 = 1;		
//}
//void Driver_4A1_ENABLE(void)
//{
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//	GPIO_InitTypeDef	EN_InitStructure;									//EN_4A1
//	EN_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
//	EN_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	EN_InitStructure.GPIO_OType = GPIO_OType_PP;			
//	EN_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;		
//	EN_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;				
//	GPIO_Init(GPIOD, &EN_InitStructure);

//	EN_2A1 = 1;	
//	EN_2A2 = 1;
//	EN_4A1 = 0;
//	CSN_4A1 = 1;				
//}
//void Driver_4A2_ENABLE(void)
//{
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//	GPIO_InitTypeDef	EN_InitStructure;				//EN_4A2
//	EN_InitStructure.GPIO_Pin = GPIO_Pin_15;
//	EN_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	EN_InitStructure.GPIO_OType = GPIO_OType_PP;			
//	EN_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;		
//	EN_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;				
//	GPIO_Init(GPIOD, &EN_InitStructure);	

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	GPIO_InitTypeDef	CSN_InitStructure;
//	EN_InitStructure.GPIO_Pin = GPIO_Pin_6;
//	EN_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	EN_InitStructure.GPIO_OType = GPIO_OType_PP;			
//	EN_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;		
//	EN_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;				
//	GPIO_Init(GPIOC, &CSN_InitStructure);

//	EN_2A1 = 1;	
//	EN_2A2 = 1;
//	EN_4A2 = 0;	
//	CSN_4A2 = 1;	
//}

//void AllDriver_DeInit(void)
//{
//	EN_2A1 = 1;
//	EN_2A2 = 1;
//	EN_4A1 = 1;
//	EN_4A2 = 1;
//}

