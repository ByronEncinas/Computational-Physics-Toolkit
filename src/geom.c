#include "../include/geom.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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

	float margin = fmaxf(rec_width, rec_height) * 3.0f;
	vertex v0 = init_vertex( x_lw + rec_width/2              , tri_height + y_lw + margin );
	vertex v1 = init_vertex( x_lw + rec_width/2 - tri_side/2 - margin, y_lw - margin );
	vertex v2 = init_vertex( x_lw + rec_width/2 + tri_side/2 + margin, y_lw - margin );

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

int equal_triangles(const triangle *a, const triangle *b) {

        // verify that both three coincide with compare_vtx()
        int first        = compare_vtx(&a->v0, &b->v0) ||
                           compare_vtx(&a->v0, &b->v1) ||
                           compare_vtx(&a->v0, &b->v2);

        int second       = compare_vtx(&a->v1, &b->v0) ||
                           compare_vtx(&a->v1, &b->v1) ||
                           compare_vtx(&a->v1, &b->v2);

        int third        = compare_vtx(&a->v2, &b->v0) ||
                           compare_vtx(&a->v2, &b->v1) ||
                           compare_vtx(&a->v2, &b->v2);

        return first && second && third;}



// This has to be delegated to its own function
// it will return an array called polygon with edge type en>
// populate_polygon_array(triangulation, bad_triangles);

// REMOVE BAD TRIANGLES
// rebuild_triangulation(triangulation, bad_triangles, coun>

// RE-TRIANGULATE
// re_triangulation(polygon, triangulation, count, p);

int populate_polygon_array(const triangle *bad_tris, const int bad_count, edge *polygons) {

	int poly_count = 0;
	int is_shared = 0;

	// each triangle has 3 edges, and I have `bad_count` triangles
	//edge *polygons = malloc( 3 * bad_count * sizeof(edge));

        for (int i = 0; i < bad_count; i++)
        {
		triangle bti = bad_tris[i];

		// each triangle type has 3 vertex types
		// create edges of current triangle bt
		is_shared = 0;
		edge e0 = init_edge(bti.v0, bti.v1);

	        for (int j = 0; j < bad_count; j++)
		{
	                triangle btj = bad_tris[j];
			// compare with other triangles, if they are the same, skip
			if (equal_triangles(&bti, &btj)) {continue;}

			// ok sure, triangles bti and btj are not the same
			// does btj has e0?

			// e1.compare(&e1, &e2);
			edge ej0 = init_edge(btj.v0, btj.v1);
                        edge ej1 = init_edge(btj.v0, btj.v2);
                        edge ej2 = init_edge(btj.v2, btj.v1);

			is_shared = e0.compare(&e0, &ej0) ||
				    e0.compare(&e0, &ej1) ||
				    e0.compare(&e0, &ej2);


			if (is_shared) {break;}
		}

                if (!is_shared)
                {
                polygons[poly_count++] = e0;
                }

                is_shared = 0;
                edge e1 = init_edge(bti.v1, bti.v2);

                for (int j = 0; j < bad_count; j++)
                {
                        triangle btj = bad_tris[j];
                        // compare with other triangles, if they are the same, skip
                        if (equal_triangles(&bti, &btj)) {continue;}

                        // ok sure, triangles bti and btj are not the same
                        // does btj has e0?
                        edge ej0 = init_edge(btj.v0, btj.v1);
                        edge ej1 = init_edge(btj.v0, btj.v2);
                        edge ej2 = init_edge(btj.v2, btj.v1);

                        is_shared = e1.compare(&e1, &ej0) ||
                                    e1.compare(&e1, &ej1) ||
                                    e1.compare(&e1, &ej2);

                        if (is_shared) {break;}
                }
                if (!is_shared)
                {
                polygons[poly_count++] = e1;
                }

                is_shared = 0;
                edge e2 = init_edge(bti.v0, bti.v2);

                for (int j = 0; j < bad_count; j++)
                {
                        triangle btj = bad_tris[j];
                        // compare with other triangles, if they are the same, skip
                        if (equal_triangles(&bti, &btj)) {continue;}

                        // ok sure, triangles bti and btj are not the same
                        // does btj has e0?
                        edge ej0 = init_edge(btj.v0, btj.v1);
                        edge ej1 = init_edge(btj.v0, btj.v2);
                        edge ej2 = init_edge(btj.v2, btj.v1);

                        is_shared = e2.compare(&e2, &ej0) ||
                                    e2.compare(&e2, &ej1) ||
                                    e2.compare(&e2, &ej2);

                        if (is_shared) {break;}
                }

                if (!is_shared)
                {
                polygons[poly_count++] = e2;
                }
	}
	return poly_count;
}
/*
void write_mesh() {
	printf("Man, I wish this thing could write itself xd");
}
*/
float* generate_random_grid(float low, float high, int n) {

        float *points =  malloc(2 * n * sizeof(float)); // 2*n only support 2D
	//srand(time(NULL));
        for (unsigned int i = 0; i < n; i++)
	{
                points[2*i]     = rand_within(low, high);
                points[2*i + 1] = rand_within(low, high);
        }
	return points;
}

float* generate_uniform_grid(float low, float high, int side) {
	// hexagonal grid

	// it always generates form (0,0)
        float *points =  malloc(2 * side * side * sizeof(float)); // 2*n only support 2D
	float sep_x = (high-low)/side;
        float sep_y = sep_x * sqrtf(3.0f)/2.0f;
	int   idx = 0;
	float height = 0;
	float disp =0;
	/*
	i want to add a random number +- epsilon to sep_x such that the triangulations doesn't have problems
	sep_x = rand_within(-1/(100*n), 1/(100*n))
	*/
        for (unsigned int i = 0; i < side; i++) // row
	{
	height = i*sep_y + rand_within(-1/(1000*side), 1/(1000*side));

	        for (unsigned int j = 0; j < side; j++) // column
		{
                        disp = j * sep_x;
			if (i % 2 == 1)
			{
			points[idx] = sep_x/2.0f + disp + rand_within(-1/(100*side), 1/(100*side));
			}
			else
			{
			points[idx] = disp + rand_within(-1/(100*side), 1/(100*side));
			}

        	        points[idx+1] = height;

			idx++;
			idx++;
        	}
        }
	return points;
}
/*
void generate_in_custom_geometry(float *hull, float density) {
	// give me a hull, and I will generate points inside that geometry
	// will try to have them as packed as density ()
	int x = 1;
}
*/
