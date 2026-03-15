#include <stdio.h>
#include "../include/vec.h"
#include <stdlib.h>

int vadd(vector *result, const vector *a, const vector *b){
    if (a -> n != b -> n || a -> n != result -> n){ return-1;}
    for (int i = 0; i < (a -> n); i++){
        result -> data[i] = (a -> data[i]) + (b -> data[i]);
    }
    return 0;
} 

vec3 v3add(const vec3 a, const vec3 b){
    return (vec3){ a.x+b.x, a.y+b.y, a.z+b.z};
}
//defined for vectors in R^3
vec3 v3cross(const vec3 a, const vec3 b) {
    return (vec3){ 
        (a.y * b.z) - (a.z * b.y), // X component
        (a.z * b.x) - (a.x * b.z), // Y component
        (a.x * b.y) - (a.y * b.x)  // Z component
    };
}

double dot(const vector *a, const vector *b){
    double dt = 0.0;
    for (int i = 0; i<(a->n); i++){
        dt += a->data[i]*b->data[i];
    } return dt;
}







