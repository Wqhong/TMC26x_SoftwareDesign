#ifndef _Driver_Control_H
#define _Driver_Control_H

#include "GPIO.h"
#include "USART.h"
#include "LED_Key.h"
#include "Driver_Init.h"

extern u8 SpeedUp, SpeedCut;
extern u8 ControlFlag;

void SpeedControl_TIM3(u8 Drivers, u16 Period);

void Driver4A_GoForward(void);
void Driver4A_GoBack(void);
void AllDriver_Stop(void);
void Driver4A_SpeedUp(void);
void Driver4A_SpeedCut(void);

void Driver2A1_Rolate(u8 Direct);
void Driver2A1_ReSet(void);
void Driver2A1_Stop(void);

void Driver2A2_PushOut(void);
void Driver2A2_ReSet(void);
void Driver2A2_Stop(void);

#endif

