#ifndef RK4
#define RK4

#include <stdio.h>
#include <stddef.h>
#include <math.h>

float matrix_copy(float *matrix1,float *matrix2, int len);
float function(float *matrix1, float *matrix2, int len, int eqs, float *consts);
float matrix_var(float *matrix, float var1, float var2, int len);
float runge_kutta_o4(float h,float *matrix1, int eqs,int len, float *consts);
void solution(float *matrix, float total_time, float h, float number, float *consts);

#endif
