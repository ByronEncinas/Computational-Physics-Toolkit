#ifndef VECTOR_H
#define VECTOR_H

// vector basic operations


//for a generic n-sized vector
typedef struct {
    int n;
    double *data;
} vector;

typedef struct {
    int m, n;
    double *data;
} matrix;


int vadd(vector *result, const vector *a, const vector *b);
int vsubs(vector *result, const vector *a, const vector *b);
void mfree(matrix *m); void vfree(vector *v);
int matmult(matrix *result, const matrix *a, const matrix *b);
int cross(vector *result, const vector *a, const vector *b);
int dot(double *dt, const vector *a, const vector *b);
int norm(double *result, const vector *a);
int unitvec(vector *u, const vector *v);
vector vcreate(int n);
matrix mcreate(int m, int n);


#endif