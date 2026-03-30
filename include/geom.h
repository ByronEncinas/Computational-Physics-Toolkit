#ifndef GEOM_H
#define GEOM_H

#include <math.h>
#include <stdlib.h>

#define PI        3.141592654
#define TOLERANCE 1e-6f

//something something... we win

typedef struct vertex {
    float x, y;
    int (*compare_vtx)(const struct vertex *self, const struct vertex *other);
} vertex;

typedef struct edge {
    vertex v0, v1;
    int (*compare_edg)(const struct edge *self, const struct edge *other);
} edge;

typedef struct triangle {
    vertex v0, v1, v2;
    vertex (*circum_center)  (const struct triangle *tri);
    float  (*circum_radius)  (const struct triangle *tri);
    int    (*in_circumcircle)(const struct triangle *tri, const vertex *v);

} triangle;

triangle init_tri          (vertex v0, vertex v1, vertex v2);
vertex   init_vertex       (float x, float y);
edge     init_edge         (vertex v0, vertex v1);
vertex   circum_center     (const triangle *tri);
float    circum_radius     (const triangle *tri);
int      in_circumcircle   (const triangle *tri, const vertex *v);
int      compare_vtx       (const vertex *self, const vertex *other);
int      compare_edg       (const edge *self, const edge *other);
triangle super_triangle    (const float *points, int n);
float    rand_within       (float low, float high);

void populate_polygon_array(const triangle *bad_tris, const int bad_count, edge *polygons);
int  equal_triangles       (const triangle *a, const triangle *b);

#endif
