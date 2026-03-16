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
void cross(vector *result, const vector *a, const vector *b);
double dot(const vector *a, const vector *b);

#endif