#ifndef __PROBE_CAP_H
#define __PROBE_CAP_H

#include "GPIO.h"


#define FLASH_SAVE_ADDR  0x08060000 	//��������	128KB	����ʹ��F407VET��F407VGT


void Probe_Cap_Init(u32 arr,u16 psc);	//Probe_Cap_Init(0XFFFFFFFF,84-1);
void Probe_Cap_DeInit(void);

void StartMeasure(u8 flag);

#endif

