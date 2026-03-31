#include "../include/geom.h" // contains the vertex, edge and triangle structs
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int write_mesh_to_file(const triangle *mesh, const int mesh_size, const char *name) {

    FILE *fptr = fopen(name, "w");

    if (fptr == NULL) { fprintf(stderr, "failed to open file\n"); return 1; }

    for (int l = 0; l < mesh_size; l++)
    {
	fprintf(fptr, "triangle %u:\n", l);
	fprintf(fptr, "  v0: (%.5f, %.5f)\n", mesh[l].v0.x, mesh[l].v0.y);
	fprintf(fptr, "  v1: (%.5f, %.5f)\n", mesh[l].v1.x, mesh[l].v1.y);
	fprintf(fptr, "  v2: (%.5f, %.5f)\n", mesh[l].v2.x, mesh[l].v2.y);
    }
    fflush(fptr);
    fclose(fptr);
    return 0;
}

int read_mesh_to_file(const triangle *mesh, const int mesh_size) {

    FILE *fptr = fopen("mesh.txt", "w");

    if (fptr == NULL) { fprintf(stderr, "failed to open file\n"); return 1; }

    for (int l = 0; l < mesh_size; l++)
    {
        fprintf(fptr, "%u (%.5f, %.5f)  (%.5f, %.5f)  (%.5f, %.5f)\n",
                        l, mesh[l].v0.x, mesh[l].v0.y,
                           mesh[l].v1.x, mesh[l].v1.y,
                           mesh[l].v2.x, mesh[l].v2.y);    }
    fflush(fptr);
    fclose(fptr);
    return 0;
}
// updated
triangle* bowyer_watson_mesh(float *points, int n, int *out_count) {
	// define super triangle
        triangle sup_tri = super_triangle(points, n);

        unsigned int max_triangle_size = 2 * 2 * n;
        unsigned int max_edge_size     = 3 * 2 * n;

        unsigned int bad_count         = 0;
        unsigned int tri_count         = 1;
        unsigned int tri_capacity      = max_triangle_size;

        // initialize output arrays
        triangle *mesh          = calloc(max_triangle_size       , sizeof(triangle));
        triangle *triangulation = calloc(max_triangle_size       , sizeof(triangle));
        triangle *bad_triangles = calloc(max_triangle_size       , sizeof(triangle));

        edge     *poly_arr      = calloc(max_edge_size    , sizeof(edge    ));

        triangulation[0] = sup_tri;

        for (unsigned int i = 0; i < n; i++)
	{
		bad_count = 0; // reset bad triangles count

                // convert point p into vertex type
                vertex p = init_vertex(points[2*i], points[2*i + 1]);

		// worse case, all triangles are bad
                free(bad_triangles);
                bad_triangles = calloc(tri_count, sizeof(triangle));

                // --- FIND BAD TRIANGLES ---
                for (unsigned int each_tri = 0; each_tri < tri_count; each_tri++)
                {
                        triangle t = triangulation[each_tri];
                        if (in_circumcircle(&t, &p)) {bad_triangles[bad_count++] = t;}
                }

                // --- FIND BOUNDARY POLYGON ---

                if (bad_count == 0) {continue;}
                free(poly_arr);
                poly_arr = calloc(3 * bad_count, sizeof(edge));

                int poly_count = populate_polygon_array(bad_triangles, bad_count, poly_arr);

                // --- REMOVE BAD TRIANGLES ---

                triangle *re_triangulation = calloc(tri_count, sizeof(triangle));
		int count = 0;
		int good_in_bad_lists = 0;

                for (unsigned int each_tri = 0; each_tri < tri_count; each_tri++)
                {
                        good_in_bad_lists = 0;
                        triangle gt = triangulation[each_tri];

                        for (unsigned int each_trj = 0; each_trj < bad_count; each_trj++)
                        {
                                triangle bt = bad_triangles[each_trj];
                                if (equal_triangles(&bt, &gt))
                                {
                                        good_in_bad_lists = 1;
                                        break;
                                }
                        }
                        if (good_in_bad_lists) {continue;}
                        re_triangulation[count++] = gt;
                }

		// tri_count <- good triangles at the beginning of the loop
		// bad_count <- bad  triangles found in triangulation
		// count     <- tri_count - bad_count - remaining triangles

                memcpy(triangulation, re_triangulation, count * sizeof(triangle));
                free(re_triangulation);
                tri_count = count;

                // --- RE-TRIANGULATE THE HOLE ---
                for (unsigned int each_edg = 0; each_edg < poly_count; each_edg++)
                {
                        if (tri_count >= tri_capacity)
                        {
                        	tri_capacity *= 2;
				triangle *tmp = realloc(triangulation, tri_capacity * sizeof(triangle));
				if (!tmp) {
				    free(triangulation);
				    free(bad_triangles);
				    free(poly_arr);
				    free(mesh);
				    return NULL;
				}

				triangulation = tmp;
                        }

                        edge edge_i = poly_arr[each_edg];
                        triangle new_tri = init_tri(edge_i.v0, edge_i.v1, p);
                        triangulation[tri_count++] = new_tri;
                }
        }
        // --- CLEANUP ---
        free(bad_triangles); free(poly_arr);
        int mesh_count = 0;
	int is_shared = 0;
	mesh = realloc(mesh, tri_capacity * sizeof(triangle));

        for (unsigned int each_tri = 0; each_tri < tri_count; each_tri++)
        {
                triangle t = triangulation[each_tri];

                // match all possible combinations of vertex between t and sup_tri
                is_shared = compare_vtx(&sup_tri.v0, &t.v0) ||
                            compare_vtx(&sup_tri.v1, &t.v0) ||
                            compare_vtx(&sup_tri.v2, &t.v0) ||
                            compare_vtx(&sup_tri.v0, &t.v1) ||
                            compare_vtx(&sup_tri.v1, &t.v1) ||
                            compare_vtx(&sup_tri.v2, &t.v1) ||
                            compare_vtx(&sup_tri.v0, &t.v2) ||
                            compare_vtx(&sup_tri.v1, &t.v2) ||
                            compare_vtx(&sup_tri.v2, &t.v2);

                if (!is_shared) { mesh[mesh_count++] = t;}
        }
        *out_count = mesh_count;
	free(triangulation);
        write_mesh_to_file(mesh, mesh_count, "triangles.txt");
	write_mesh_to_file(&sup_tri, 1, "sup_triangle.txt");
        return mesh;}

/*

triangle* bowyer_watson_mesh(float *points, int n, int *out_count) {

	// initializes super triangle
	triangle sup_tri = super_triangle(points, n);

	printf("\n  (%.2f, %.2f)\n", sup_tri.v0.x, sup_tri.v0.y);
	printf(
    "            /\\\n"
    "           /  \\\n"
    "          /    \\\n"
    "         /      \\\n"
    "        /________\\\n\n");
        printf("(%.2f, %.2f)   (%.2f, %.2f)\n", sup_tri.v1.x, sup_tri.v1.y, sup_tri.v2.x, sup_tri.v2.y);

	// how can i fiture out the max number of triangles,
	// that can be formed from n points in a form of a mesh?
	// V - E + F = 2?

	//For a planar triangulation,

	//V: number of corners (a point)
	//E: number of edges   (a line)
	//F: number of faces (interior triangles + outer face)

	//There will be 3*F - 3 interior edges if there are F interior triangles
	//Edges are shared, there will the number of interior edges is roughly 2E - h
	//with h being the boundary edges (points in the convex hull)

	//this leads to the number of triangles

	//F = 2V - 1 - h
	//E = 3n - 3 - h

	//if we ignore the h as a safety net

        //(# triangles) = 2 (# points) - 1
        //(# edges)     = 3 (# points) - 3

	//source:	  - https://math.stackexchange.com/questions/1286800/euler-formula-for-planar-graphs?utm_source=chatgpt.com

        unsigned int max_triangle_size = 2 * 2 * n;
        unsigned int max_edge_size     = 2 * 3 * n;
        unsigned int good_count        = 1;
        unsigned int rebuilt_count     = 0;
	unsigned int poly_count        = 0;
        unsigned int is_shared         = 0;
        unsigned int bad_count         = 0;
        unsigned int is_gt_in_bd       = 0;
        unsigned int mesh_count        = 0;

	unsigned int tri_count         = 1;   // how many triangles are USED
	unsigned int tri_capacity      = max_triangle_size;  // how many are ALLOCATED

	printf("max_triangle_size : %u\n", max_triangle_size);
	printf("max_edge_size     : %u\n", max_edge_size);
	printf("count             : %u\n", good_count);
	printf("rebuilt_count     : %u\n", rebuilt_count);
	printf("poly_count        : %u\n", poly_count);
	printf("is_shared         : %u\n", is_shared);
	printf("bad_count         : %u\n", bad_count);
	printf("is_gt_in_bd       : %u\n", is_gt_in_bd);

	// initialize output triangulation
	triangle *mesh          = calloc(good_count       , sizeof(triangle));
	triangle *triangulation = calloc(max_triangle_size, sizeof(triangle));
	triangle *bad_triangles = calloc(max_triangle_size, sizeof(triangle)); // or 1 if you really need only 1
	edge     *poly_arr      = calloc(max_edge_size    , sizeof(edge    ));

	triangulation[0] = sup_tri;

	// points contains 3*n values
	for (unsigned int i = 0; i < n; i++)

	{
                bad_count = 0;

		// convert point p into vertex type
                vertex p = init_vertex(points[2*i], points[2*i + 1]);

                printf("#(index of point p)  : %u\n", i);
                printf("#(bad triangles)     : %u\n", bad_count);
                printf("(%.2f, %.2f)\n", p.x, p.y);

		// allocate in heap for dynamic resizing
		free(bad_triangles);
		bad_triangles = calloc(max_triangle_size, sizeof(triangle));

		// --- FIND BAD TRIANGLES ---
		for (unsigned int each_tri = 0; each_tri < good_count; each_tri++)
		{
			triangle t = triangulation[each_tri];

			if (in_circumcircle(&t, &p))
			{
				bad_triangles[bad_count++] = t;

			}
		}


		// --- FIND BOUNDARY POLYGON ---

		free(poly_arr);

		if (bad_count == 0) {continue;}

		poly_arr = malloc(3 * bad_count * sizeof(edge));
		poly_count = populate_polygon_array(bad_triangles, bad_count, poly_arr);

		// --- REMOVE BAD TRIANGLES ---
		triangle *rebuilt = calloc(max_triangle_size, sizeof(triangle)); // whatever the size nee>
		rebuilt_count = 0; // reasign

                for (unsigned int each_tri = 0; each_tri < good_count; each_tri++)
                {
			is_gt_in_bd = 0;
                        triangle gt = triangulation[each_tri];

	                for (unsigned int each_trj = 0; each_trj < bad_count; each_trj++)
			{
				triangle bt = bad_triangles[each_trj];
	                        if (equal_triangles(&bt, &gt))
				{
					is_gt_in_bd = 1;
					break;
				}
			}
			if (is_gt_in_bd) {continue;}
                        rebuilt[rebuilt_count++] = gt;
                }

		//free(triangulation);
		//triangulation = realloc(triangulation, (poly_count + rebuilt_count) * sizeof(triangle));
		memcpy(triangulation, rebuilt, rebuilt_count * sizeof(triangle));
		good_count = rebuilt_count;
		free(rebuilt);
                printf("R copied into T with size %u -- ", good_count);
		// In here there is some important writing past end
		printf("Max Size of T %u -- ", max_triangle_size);

		// --- RE-TRIANGULATE THE HOLE ---
		for (unsigned int each_edg = 0; each_edg < poly_count; each_edg++)
		{
			if (good_count >= max_triangle_size)
			{// temporary solution
			max_triangle_size *= 2;
			triangulation = realloc(triangulation, max_triangle_size * sizeof(triangle));
			}

			edge edge_i = poly_arr[each_edg];
			triangle new_tri = init_tri(edge_i.v0, edge_i.v1, p);
			triangulation[good_count++] = new_tri;
		}
	}
	// --- CLEANUP ---
	// if triangle shares vertex with sup_triangle, then delete
	rebuilt_count = 0;
	mesh_count = 0;
	triangle *rebuilt = malloc(good_count * sizeof(triangle));
	for (unsigned int each_tri = 0; each_tri < good_count; each_tri++)
	{
		triangle t = triangulation[each_tri];
		// match all possible combinations of vertex between t and sup_tri
		is_shared = compare_vtx(&sup_tri.v0, &t.v0) ||
			    compare_vtx(&sup_tri.v1, &t.v0) ||
                            compare_vtx(&sup_tri.v2, &t.v0) ||
                            compare_vtx(&sup_tri.v0, &t.v1) ||
                            compare_vtx(&sup_tri.v1, &t.v1) ||
                            compare_vtx(&sup_tri.v2, &t.v1) ||
                            compare_vtx(&sup_tri.v0, &t.v2) ||
                            compare_vtx(&sup_tri.v1, &t.v2) ||
                            compare_vtx(&sup_tri.v2, &t.v2);
		if (!is_shared) // if not shared, save triangle
		{
		mesh[mesh_count++] = t;
		}
	}

        printf("mesh size: %d:\n", mesh_count);
	free(bad_triangles);
	free(poly_arr);
	free(triangulation);

	mesh = realloc(mesh, mesh_count * sizeof(triangle));
	*out_count = mesh_count;

        free(bad_triangles);
        free(poly_arr);
        free(triangulation);

	write_mesh_to_file(mesh, mesh_count);

	return mesh;
}
*/
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

