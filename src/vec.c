#include <stdio.h>
#include "../include/vec.h"
#include <stdlib.h>
#include <math.h>

//initialization of vectors
vector vcreate(int j){
    vector v; v.n = j; v.data = malloc(j * sizeof(double));
    if (v.data == NULL){ return (vector){0, NULL};}
    return v;
}

//addition of vector
void vadd(vector *result, const vector *a, const vector *b){
    if (a -> n != b -> n || a -> n != result -> n){ return;}
    for (int i = 0; i < (a -> n); i++){
        result -> data[i] = (a -> data[i]) + (b -> data[i]);
    }
} 

//cross product, defined for vectors in R^3
void cross(vector *result,const vector *a, const vector *b) {
    if (a -> n != 3 || b -> n != 3 || result -> n != 3){return;}
    result -> data[0] = (a->data[1] * b->data[2]) - (a->data[2] * b->data[1]);
    result -> data[1] = (a->data[2] * b->data[0]) - (a->data[0] * b->data[2]);
    result -> data[2] = (a->data[0] * b->data[1]) - (a->data[1] * b->data[0]);
}

//dot product
double dot(const vector *a, const vector *b){
    if (a->n != b->n){ return 0.0;}
    double dt = 0.0;
    for (int i = 0; i<(a->n); i++){
        dt += a->data[i]*b->data[i];
    } return dt;
}

//euclidean norm
double norm(const vector *a){
    double sum = 0.0;
    if (a == NULL || a->data == NULL){ return 0.0;}
    if (a->n == 1){ return fabs(a->data[0]);}
    for (int i = 0; i<(a->n); i++){
        sum += a->data[i] * a->data[i];
    } return sqrt(sum);
}











