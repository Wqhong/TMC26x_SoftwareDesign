#ifndef __main_H
#define __main_H

#include "GPIO.h"
#include "IWDG.h"
#include "Delay.h"
#include "USART.h"

#include "IIC.h"
#include "Oled.h"

#include "Adc.h"
#include "Encode.h"
#include "LED_Key.h" 
#include "SystemTim.h"
#include "Probe_Cap.h"
#include "GPIO_Init.h"
#include "Driver_Init.h"
#include "Driver_Control.h"

#include "STMFlash.h"
#include "Probe_Calibrate.h"

#include "stm32f4xx_conf.h"

extern u16	CALI_STA;
extern u8 UpdateCoefficientFlag;

#endif
