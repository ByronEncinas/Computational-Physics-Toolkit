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

void linear_mesh(float *nodes, int *elems, float *h, int n, int m) {
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

