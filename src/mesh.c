#include "../include/geom.h" // contains the vertex, edge and triangle structs
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void bowyer_watson_mesh(float *points, int n) {
/*	2D tesselation algorithm

paramters:

input:
        points: array with n point coordinates which we want to mesh (size n)
	n:      size of points // 2
output:
	...
*/
	// initializes super triangle
	triangle sup_tri = super_triangle( points, n);
        unsigned short ll = 2*n, count = 1;

	// initialize output triangulation
	triangle *triangulation = malloc( ll * sizeof(triangle)); // whatever the size needed to store a type triangle, plus the  number of triangles ot allocate

        triangulation[0] = sup_tri;

	// points contains 3*n values
	for (int i = 0; i < n; i++)

	{
		// extract all coordinates of a specific point
		float x = points[3*i];
                float y = points[3*i + 1];

		// index of invalid/bad triangles in triangulation
		short *index = malloc( ll * sizeof(short));

		// count of bad triangles in this instance of the loop
                unsigned short bad_count = 0;

		// convert point p into vertex type
                vertex p = init_vertex(x, y);

		// allocate in stack for faster access
		triangle bad_triangles[count];

		// FIND BAD TRIANGLES--------------------
		// append_bad_triangle_lists(triangulation, count)

		for (int each_tri = 0; each_tri < count; each_tri++)
		{

			triangle t = triangulation[each_tri];

			if (in_circumcircle(&t, &p))
			{
				bad_triangles[bad_count++] = t;
			}
		// This has to be delegated to its own function
		// it will return an array called polygon with edge type entries
		// populate_polygon_array(triangulation, bad_triangles);

		// REMOVE BAD TRIANGLES
		// rebuild_triangulation(triangulation, bad_triangles, count);

		// RE-TRIANGULATE
		// re_triangulation(polygon, triangulation, count, p);

		// step 3: Connect P(x,y,z) to every edge on the boundary of that hole
		}

	// cleanup, remove every triangle that shares a vertex with the supertriangle
	// the super triangle was never part of the input

	// at this point, i have a list of triangles,
	}
}

void linear_mesh(float *nodes, int *elems, float *h, int n, int m) {
	/*
	mesh1d: this functions generates a 1D mesh of nodes and elements.

	paramters:
		nodes: empty array to save nodes coordinates (size n)
		elems: empty array, connections of an element (size 2n)
			contains indices of the nodes connected to that element
		bounds: array of size 2,
		h: separation between nodes
		   - float: fixed separation
 	*/

        // verify if method is 0 (static) or 1 (adaptative)
        if (m == 0) {
	        // if m==0 then h is float
		for (int i = 0; i < n; i++) {
	                nodes[i]       = i*(*h); // dereference, h:= is an address, *h  a value
		        elems[2*i]     = i;
			elems[2*i + 1] = i+1;
			}
                //call static method
        } else if (m == 1) {         // if m==1 then h is array size of elems
                for (int i = 0; i < n; i++) {
			if (i == 0) {nodes[i] = 0; elems[0] = 0; elems[1] = 1; continue;}
                        nodes[i]       = nodes[i-1] + h[i];
                        elems[2*i]     = i;
                        elems[2*i + 1] = i+1;
		}
	}
}

void mesh_from_file(){
	printf("Placeholder Text\n");
}
