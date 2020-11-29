#ifndef __Driver_Init_H
#define __Driver_Init_H

#include "stm32f4xx.h"

#include "Delay.h"
#include "USART.h"
#include "LED_Key.h"
#include "Driver_Control.h"

#define EN_2A1		PBout(5)
#define CSN_2A1		PBout(4)

#define EN_4A1		PDout(7)
#define CSN_4A1		PDout(6)
#define SDI1			PCout(12)
#define SDO1			PCout(11)
#define SCK1			PCout(10)

#define EN_2A2		PDout(13)
#define CSN_2A2		PDout(12)
#define EN_4A2		PDout(15)
#define CSN_4A2		PCout(6)
#define SDI2			PBout(15)
#define SDO2			PBout(14)
#define SCK2			PBout(13)

#define DIR_2A1			PDout(1)
#define STEP_2A1		PDout(2)
#define DIR_2A2			PDout(9)
#define STEP_2A2		PDout(10)

#define DIR_4A1			PDout(3)
#define STEP_4A1		PDout(4)
#define DIR_4A2			PCout(8)
#define STEP_4A2		PCout(9)

#define Driver2A1		1
#define Driver2A2		2
#define Driver4A		4


void Drivers_EnAndCsn_Init(void);		//main函数中调用
void Drivers_ENABLE(u8 D2AorD4A);		//main函数中调用


void Driver_SPI1_Init(void);
void Driver_SPI2_Init(void);

uint32_t Driver_Config1(u8 D2AorD4A, uint32_t write_data);
uint32_t Driver_Config2(u8 D2AorD4A, uint32_t write_data);

void Driver_2A1_Control_Init(void);
void Driver_2A2_Control_Init(void);
void Driver_4A1_Control_Init(void);
void Driver_4A2_Control_Init(void);


/************************
		外部调用	
*************************/
void Driver_2A1_Init(void);
void Driver_2A2_Init(void);

void Driver_4A_Init(void);

//void Driver_4A1_Init(void);
//void Driver_4A2_Init(void);

//void Driver_2A1_ENABLE(void);
//void Driver_2A2_ENABLE(void);
//void Driver_4A1_ENABLE(void);
//void Driver_4A2_ENABLE(void);

//void AllDriver_DeInit(void);


#endif 

