#ifndef VECTOR_H
#define VECTOR_H

// vector basic operations


//for a generic n-sized vector
typedef struct {
    int n;
    double *data;
} vector;

typedef union{
    struct {double x,y,z; };
    double raw[3];
} vec3;

typedef struct {
    int m, n;
    double *data;
} matrix;


int vadd(vector *result, const vector *a, const vector *b);
double dot(const vector *a, const vector *b);
vec3 cross(const vec3 a, const vec3 b);


#endif