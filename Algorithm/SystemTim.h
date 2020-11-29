#include "GPIO.h"
#include "USART.h"

#define PMVA_len 30
#define CALI_len 8

void SystemTim_Init(u32 arr,u16 psc);

extern u16	CALI_STA;
extern u32	TIM5CH1_CAPTURE_VAL;


extern double Probe_Measure_Val_Arr[PMVA_len];
extern double Measure_One_Thickness(void);

double Measure_One_Thickness(void);
u8 Judge_Measure_isVal(double* Probe_Measure_Val_Arr);
void Save_Latest_Val(u32 TIM5CH1_CAPTURE_VAL, double* Probe_Measure_Val_Arr);


extern double Seg1,Seg2,Seg3,EndHz;
extern double ThicknessValue,lastHzValue,InfValue,AvgHzValue;
extern void OLED_ShowValue(double Value);
extern double KeyIncOrDec_Value(double Val);
extern void KeyCalibrate_Value(double Hz,double Val);

double Calibrate_Measure2Avg(void);
void Polynomial_Calibrate(u8 ResetCali);

