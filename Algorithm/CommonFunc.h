#ifndef __COMMONFUNC_H
#define __COMMONFUNC_H

#include "math.h"
#include "GPIO.h"


void Array_Copy(double* x, double* y, u8 len);
double Get_ArrayAvg(double* Hz, int start, int end);

u8 Selection_Sort(double* Hz, u8 N);
double WindowFilter(double* Hz,double Hz0,double AvgValue,u8 len);

u8 ArrayTakeOut(double* Array,u8 N,double* NewArray,u8 start,u8 n);

#endif

