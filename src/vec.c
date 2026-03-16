#include <stdio.h>
#include "../include/vec.h"
#include <stdlib.h>


vector vcreate(int j){
    vector v; v.n = j; v.data = malloc(j * sizeof(double));
    if (v.data == NULL){ return (vector){0, NULL};}
    return v;
}

void vadd(vector *result, const vector *a, const vector *b){
    if (a -> n != b -> n || a -> n != result -> n){ return;}
    for (int i = 0; i < (a -> n); i++){
        result -> data[i] = (a -> data[i]) + (b -> data[i]);
    }
} 

//defined for vectors in R^3
void cross(vector *result,const vector *a, const vector *b) {
    if (a -> n != 3 || b -> n != 3 || result -> n != 3){return;}
    result -> data[0] = (a->data[1] * b->data[2]) - (a->data[2] * b->data[1]);
    result -> data[1] = (a->data[2] * b->data[0]) - (a->data[0] * b->data[2]);
    result -> data[2] = (a->data[0] * b->data[1]) - (a->data[1] * b->data[0]);
}

double dot(const vector *a, const vector *b){
    double dt = 0.0;
    for (int i = 0; i<(a->n); i++){
        dt += a->data[i]*b->data[i];
    } return dt;
}







