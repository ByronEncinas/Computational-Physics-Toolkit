#include "../include/geom.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.141592654
#define TOLERANCE 1e-6f

vertex init_vertex(float x, float y) {
        vertex vtx;           // declares a matrix variable on the stack
        vtx.x = x;            // store row count
        vtx.y = y;            // store column count
        vtx.compare = compare_vtx;
        return vtx;
}

edge init_edge(vertex v0, vertex v1) {
        edge edg;           // declares a matrix variable on the stack
        edg.v0 = v0;            // store row count
        edg.v1 = v1;            // store column count
        edg.compare = compare_edg;
        return edg;
}

triangle init_tri(vertex v0, vertex v1, vertex v2) {
	triangle tri;
	tri.v0 = v0;
	tri.v1 = v1;
	tri.v2 = v2;
	tri.circum_center   = circum_center;
	tri.circum_radius   = circum_radius;
	tri.in_circumcircle = in_circumcircle;
	return tri;
}

vertex circum_center(const triangle *tri) {
    float ax = tri->v0.x, ay = tri->v0.y;
    float bx = tri->v1.x, by = tri->v1.y;
    float cx = tri->v2.x, cy = tri->v2.y;

    float D = 2 * (ax*(by-cy) + bx*(cy-ay) + cx*(ay-by));

    if (fabsf(D) < TOLERANCE) {
        return init_vertex(0.0f, 0.0f);
    }

    float ax2 = ax*ax + ay*ay;
    float bx2 = bx*bx + by*by;
    float cx2 = cx*cx + cy*cy;

    float ux = (ax2*(by-cy) + bx2*(cy-ay) + cx2*(ay-by)) / D;
    float uy = (ax2*(cx-bx) + bx2*(ax-cx) + cx2*(bx-ax)) / D;

    return init_vertex(ux, uy);

}

float circum_radius(const triangle *tri) {
    vertex cc = circum_center(tri);
    float dx = tri->v0.x - cc.x;
    float dy = tri->v0.y - cc.y;
    return sqrtf(dx*dx + dy*dy);
}

int in_circumcircle(const triangle *tri, const vertex *v) {
    vertex cc = circum_center(tri);
    float dx = cc.x - v->x;
    float dy = cc.y - v->y;
    // tri is already a pointer, so it can be passed without it
    // it being copied
    float r = circum_radius(tri);
    return (dx*dx + dy*dy) <= (r*r);
}

int compare_vtx(const vertex *self, const vertex *other){
	return fabsf(self->x - other->x) < TOLERANCE &&
				fabsf(self->y - other->y) < TOLERANCE;
}


int compare_edg(const edge *self, const edge *other){

	int forw = fabsf(self->v0.x - other->v0.x) < TOLERANCE &&
                  fabsf(self->v0.y - other->v0.y) < TOLERANCE &&
		  fabsf(self->v1.x - other->v1.x) < TOLERANCE &&
                  fabsf(self->v1.y - other->v1.y) < TOLERANCE;

	int back = fabsf(self->v0.x - other->v1.x) < TOLERANCE &&
                  fabsf(self->v0.y - other->v1.y) < TOLERANCE &&
                  fabsf(self->v1.x - other->v0.x) < TOLERANCE &&
                  fabsf(self->v1.y - other->v0.y) < TOLERANCE;

	return forw || back;
}

triangle super_triangle(const float *points, const int n) {

	// conditions for the points mesh, they cannot be colinear or have negative coordinates
	float x_lw = points[0]; float x_up = points[0];
        float y_lw = points[1]; float y_up = points[1];

        for (int i = 0; i < n; i++) {
		// forming the super triangle, find bounds in x and y
		// update to lowest/largest component in 'points'
		if (x_lw > points[2*i]) { x_lw = points[2*i]; }
		if (x_up < points[2*i]) { x_up = points[2*i]; }

                if (y_lw > points[2*i+1]) { y_lw = points[2*i+1]; }
                if (y_up < points[2*i+1]) { y_up = points[2*i+1]; }
        }

	//corner coordinates      ( x , y )   ( x , y )   ( x , y )   ( x , y )
	//float sup_rectangle[8] = {x_lw, y_up, x_lw, y_lw, x_up, y_up, x_up, y_lw};

	// Find super triangule inscribing super rectangle
	float rec_height = y_up - y_lw;
	float rec_width  = x_up - x_lw;

	// sup triangle info
	float tri_side = rec_width + 2*rec_height/sqrtf(3);
	float tri_height = tri_side * sqrtf(3)/2;

	// create vertex that form the xuper_triangle
	vertex v0 = init_vertex(x_lw + rec_width/2, tri_height + y_lw);
        vertex v1 = init_vertex(x_lw + rec_width/2 - tri_side/2,  y_lw);
        vertex v2 = init_vertex(x_lw + rec_width/2 + tri_side/2,  y_lw);

	// declare sup_triangle
	triangle sup_tri = init_tri(v0, v1, v2);

	return sup_tri;
}

// generate a pseudo random number between 'low' and 'high'
float rand_within(float low, float high) {
        //srand(time(NULL))
        float x  = (float)rand() / RAND_MAX;
        return low + x * (high - low);
}

