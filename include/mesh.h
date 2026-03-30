#ifndef MESH_H
#define MESH_H

#include "geom.h"

//something something... we win

void linear_mesh (float *nodes, int *elems, float *h, const int n, const int m);
void bowyer_watson_mesh(float *points, int n, triangle *mesh, unsigned int *out_count);

#endif
