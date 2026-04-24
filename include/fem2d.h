#ifndef FEM2D
#define FEM2D


#include<stdio.h>

void B_mat(int size, float ks[size][3], float a[size][3], float b[size][3], float c[size][3], float B[size][size],float Kx_mat[size][size],float Ky_mat[size][size],float P_mat[size][size],int nodes[(size-2)*3]);
void C_mat(int size,float* C, float a[size-2][3], float b[size-2][3], float c[size-2][3], float Q_val,int nodes[(size-2)*3]);
void abcs_val(int size,float pointsx[size-2][3], float pointsy[size-2][3], float a[size-2][3], float b[size-2][3], float c[size-2][3]);
void BC(int size, float B[size][size], float* C, float bc[size-2][2]);
void zeros(int size, float matrix[size][size]);
void printing_assembly(int size, float B[size][size], float* C);
void printing_points(float* pointsx, float* pointsy, int num_triang);
void print_result(int size, float B[size][size], float* C);



#endif

