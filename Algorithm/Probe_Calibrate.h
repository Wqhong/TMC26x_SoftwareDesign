#ifndef __PROBE_CALIBRATE_H
#define __PROBE_CALIBRATE_H

#include "IWDG.h"
#include "USART.h"

void Parameter_WriteToFlash(u32 ReadAddr,double* X);
void Parameter_ReadFromFlash(u32 ReadAddr,double* X);


void Get_Probe_Value(void);
double Get_Probe_Array_Val(double* Array,double Cali);
double Polynomial_GetThicknessValue(double Probe_Cap_Hz);

void AmendPolynomialCalibrate(void);
void SubsectionPolynomial(double* Hz,double* Th,double* a,double* b,double* c,double* d);

double KeyIncOrDec_Value(double Val);
void KeyCalibrate_Value(double Hz,double Val);

void Interpolation(double* x,double* std,double* X0,u8 EightOrTwo);
void Interpolation_Polynomial_Calibrate(double* Arr, u8 ResetCali);

void FrequencyStd_WriteToFlash(void);
void FrequencyStd2Coefficient_ReadFromFlash(void);	//从Flash中获取上次校准好的参数值

extern double Probe_Measure_Val_Arr[30];

//void CaliSendToUart(void);


#endif
