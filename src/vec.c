#include <stdio.h>
#include "../include/vec.h"
#include <stdlib.h>
#include <math.h>

//initialization of matrices
matrix mcreate(int i, int j){
    matrix mat; mat.m = i; mat.n = j; mat.data = malloc(i * j * sizeof(double)); 
    if (mat.data == NULL){ return (matrix){0, 0, NULL};}
    return mat;
}

//matrix multiplicaiton
int matmult(matrix *result, const matrix *a, const matrix *b){
    //first, some flags to avoid errors due to the size of matrices
    if (a->n != b->m || result->m != a->m || result->n != b->n){ return -1;}
    for (int i = 0; i < result->m * result->n; i++) {
        result->data[i] = 0.0;
    }

    for (int i = 0; i < a->m; i++){
        for (int k = 0; k < a->n; k++){
            for (int j = 0; j < b->n; j++){
                result->data[i*result->n+j] += a->data[i*a->n+k] * b->data[k*b->n+j];
            }
        }
    } 
    return 0;
}

//initialization of vectors
vector vcreate(int j){
    matrix mat = mcreate(1,j);
    if (mat.data == NULL){ return (vector){0, NULL}; }
    return (vector){mat.n, mat.data};
}

void mfree(matrix *m){ free(m->data); m->data = NULL; m->m = m->n = 0; }
void vfree(vector *v){ free(v->data); v->data = NULL; v->n = 0; }


int matrow(vector *v, const matrix *a, int row){
    if (a->data == NULL || row < 0 || row >= a->m){return -1;}
    v -> n = a -> n; v -> data = &(a->data[row * a->n]);
    return 0;
}


//addition of vector
int vadd(vector *result, const vector *a, const vector *b){
    if (a -> n != b -> n || a -> n != result -> n){ return -1;}
    for (int i = 0; i < (a -> n); i++){
        result -> data[i] = (a -> data[i]) + (b -> data[i]);
    } return 0;
} 

//substraction of vectors
int vsubs(vector *result, const vector *a, const vector *b){
    if (a -> n != b -> n || a -> n != result -> n){ return -1;}
    for (int i = 0; i < (a -> n); i++){
        result -> data[i] = (a -> data[i]) - (b -> data[i]);
    } return 0;
} 

//cross product, defined for vectors in R^3
int cross(vector *result,const vector *a, const vector *b) {
    if (a -> n != 3 || b -> n != 3 || result -> n != 3){return -1;}
    result -> data[0] = (a->data[1] * b->data[2]) - (a->data[2] * b->data[1]);
    result -> data[1] = (a->data[2] * b->data[0]) - (a->data[0] * b->data[2]);
    result -> data[2] = (a->data[0] * b->data[1]) - (a->data[1] * b->data[0]);
    return 0;
}

//dot product
int dot(double *dt, const vector *a, const vector *b){
    if (a->n != b->n){ return -1;}
    *dt = 0.0;
    for (int i = 0; i<(a->n); i++){
        *dt += a->data[i]*b->data[i];
    } return 0;
}

//euclidean norm
int norm(double *result, const vector *a){
    double sum = 0.0;
    if (a == NULL || a->data == NULL){ return -1;}
    for (int i = 0; i<(a->n); i++){
        sum += a->data[i] * a->data[i];
    } 
    *result = sqrt(sum);
    return 0;
}

//to find a unit vector 'u'
int unitvec(vector *u, const vector *v){
    if (v == NULL || v -> data == NULL){ return -1;}
    u -> n = v -> n;
    double mag = 0.0; norm(&mag, v);
    if (mag == 0.0){ return -1;}
    for (int i = 0; i < u->n; i++){
        u -> data[i] = (v -> data[i]) / mag;
    } return 0;
}











