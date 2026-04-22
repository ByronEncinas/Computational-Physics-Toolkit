#ifndef FEM2D
#define FEM2D

#include<stdio.h>

void B_mat(float ks[2][3], float a[2][3], float b[2][3], float c[2][3], float B[4][4],int nodes[6]);
void C_mat(float* C, float a[2][3], float b[2][3], float c[2][3], float Q_val,int nodes[6]);
void abcs_val(float pointsx[2][3], float pointsy[2][3], float a[2][3], float b[2][3], float c[2][3]);
void printing_points(float* pointsx, float* pointsy, int num_triang);
void print_result(float B[4][4], float* C);


#endif
