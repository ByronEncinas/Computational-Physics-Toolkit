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


void vadd(vector *result, const vector *a, const vector *b);
void vsubs(vector *result, const vector *a, const vector *b);
void mfree(matrix *m); void vfree(vector *v);
void matmult(matrix *result, const matrix *a, const matrix *b);
void cross(vector *result, const vector *a, const vector *b);
double dot(const vector *a, const vector *b);
double norm(const vector *a);
vector vcreate(int n);
matrix mcreate(int m, int n);


#endif