#include "../include/geom.h" // contains the vertex, edge and triangle structs
#include "../include/imesh.h" // contains the vertex, edge and triangle structs
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int populate_polygon_array_index(const triangle *bad_tris, const int bad_count,
                        int *bad_index, edge *polygons, int *poly_index) {

        int poly_count = 0;
        int is_shared = 0;

        // each triangle has 3 edges, and I have `bad_count` triangles
        //edge *polygons = malloc( 3 * bad_count * sizeof(edge));

        // i want to store the corresponding index of each polygon
        unsigned int edge_index[2];

        for (int i = 0; i < bad_count; i++)
        {

                // unpack bad triangle and their indexes - let's hope they are in order
                triangle bti = bad_tris[i];
                //bad_elems[3*bad_count] = l; bad_elems[3*bad_count+1] = m; ... etc.
                unsigned int a = bad_index[3*i], b = bad_index[3*i+1], c = bad_index[3*i+2];
                //printf("Curr Element Indexes - a: %d, b: %d, c: %d\n", a,b,c);

                // each triangle type has 3 vertex types
                // create edges of current triangle bt
                is_shared = 0;

                // unpack edges and their indexes
                edge e0 = init_edge(bti.v0, bti.v1);
                edge_index[0] = a;
                edge_index[1] = b;

                for (int j = 0; j < bad_count; j++)
                {
                        triangle btj = bad_tris[j];
                        unsigned int d = bad_index[3*j], e = bad_index[3*j+1], f = bad_index[3*j+2];
                        // compare with other triangles, if they are the same, skip

                        if (a == d && b == e && c == f) {continue;}

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
                polygons[poly_count] = e0;
                poly_index[2*poly_count] = edge_index[0];
                poly_index[2*poly_count+1] = edge_index[1];
                poly_count++;
                }

                is_shared = 0;
                edge e1 = init_edge(bti.v1, bti.v2);
                edge_index[0] = b;
                edge_index[1] = c;


                for (int j = 0; j < bad_count; j++)
                {
                        triangle btj = bad_tris[j];
                        unsigned int d = bad_index[3*j], e = bad_index[3*j+1], f = bad_index[3*j+2];
                        // compare with other triangles, if they are the same, skip
                        if (a == d && b == e && c == f) {continue;}

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
                polygons[poly_count] = e1;
                poly_index[2*poly_count] = edge_index[0];
                poly_index[2*poly_count+1] = edge_index[1];
                poly_count++;
                }

                is_shared = 0;
                edge e2 = init_edge(bti.v0, bti.v2);
                edge_index[0] = a;
                edge_index[1] = c;
                for (int j = 0; j < bad_count; j++)
                {
                        triangle btj = bad_tris[j];
                        unsigned int d = bad_index[3*j], e = bad_index[3*j+1], f = bad_index[3*j+2];
                        // compare with other triangles, if they are the same, skip
                        if (a == d && b == e && c == f) {continue;}

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
                polygons[poly_count] = e2;
                poly_index[2*poly_count] = edge_index[0];
                poly_index[2*poly_count+1] = edge_index[1];
                poly_count++;
                }
        }
        return poly_count;
}

float* generate_afinne_grid(const float x_low, const float x_high,
			const float y_low, const float y_high, const int side) {
        // hexagonal grid

        // it always generates form (0,0)
        float *points =  malloc(2 * side * side * sizeof(float)); // 2*n only support 2D
        float sep_x = (x_high-x_low)/side;
        float sep_y = sep_x * sqrtf(3.0f)/2.0f;
        int   idx = 0;
        float height = 0;
        float disp =0;
        for (unsigned int i = 0; i < side; i++) // row
        {
        height = y_low + i*sep_y ;

                for (unsigned int j = 0; j < side; j++) // column
                {
                        disp = x_low + j * sep_x;
                        if (i % 2 == 1)
                        {
                        points[idx] = sep_x/2.0f + disp;
                        }
                        else
                        {
                        points[idx] = disp;
                        }

                        points[idx+1] = height;

                        idx++;
                        idx++;
                }
        }
        return points;
}

void* bidimensional_mesh(float *points, int n, int *out_count) {

        // define super triangle
        triangle sup_tri = super_triangle(points, n);

        unsigned int max_triangle_size = 2 * 2 * n;
        unsigned int max_edge_size     = 3 * 2 * n;

        unsigned int bad_count         = 0;
        unsigned int tri_count         = 1;
        unsigned int tri_capacity      = max_triangle_size;

        // initialize output arrays
        // triangle *mesh          = calloc(max_triangle_size       , sizeof(triangle));
        triangle *triangulation = calloc(max_triangle_size       , sizeof(triangle));
        triangle *bad_triangles = calloc(max_triangle_size       , sizeof(triangle));

        edge         *poly_arr  = calloc(max_edge_size           , sizeof(edge    ));
        unsigned int *poly_index= calloc(3 * bad_count           , sizeof(unsigned int));

        // store triangulation based on index with respect to points array
        unsigned int p_index = 0;
        unsigned int xi = 0;
        unsigned int yi = 0;
        unsigned int *mesh_elems= calloc (3 * max_triangle_size+3, sizeof(unsigned int));
        unsigned int *elems     = calloc (3 * max_triangle_size+3, sizeof(unsigned int));
        unsigned int *bad_elems = calloc (3 * max_triangle_size+3, sizeof(unsigned int));

        elems[0] = 0; elems[1] = 1; elems[2] = 2;

        triangulation[0] = sup_tri;

        for (unsigned int pi = 0; pi < n; pi++)
        {
                bad_count = 0; // reset bad triangles count

                // convert point p into vertex type
                p_index = 2*pi;
		xi = 2 * pi; yi = xi + 1;

                vertex p = init_vertex(points[xi], points[yi]);

                free(bad_triangles);
                bad_triangles = calloc(tri_count, sizeof(triangle));

                // --- FIND BAD TRIANGLES ---
                for (unsigned int each_tri = 0; each_tri < tri_count; each_tri++)
                {
                        // at i=0, the only triangle in triangulation is the sup_tri
                        // its index is zero
                        triangle t = triangulation[each_tri];

                        // same way we unpack triangles in triangulation, we unpack their indices
                        unsigned int o = elems[3*each_tri], p_ = elems[3*each_tri + 1], q = elems[3*each_tri + 2];

                        if (in_circumcircle(&t, &p))
                        {
                                bad_triangles[bad_count] = t;
                                bad_elems[3*bad_count]   = o;
				bad_elems[3*bad_count+1] = p_;
				bad_elems[3*bad_count+2] = q;
                                bad_count++;
                        }
                }

                // --- FIND BOUNDARY POLYGON ---

                if (bad_count == 0) {continue;}
                free(poly_arr);
                poly_arr   = calloc(3 * bad_count, sizeof(edge));

                free(poly_index);
                poly_index = calloc(2 * 3 * bad_count, sizeof(unsigned int));

                int poly_count = populate_polygon_array_index(bad_triangles, bad_count,
                        bad_elems, poly_arr, poly_index);

                //int poly_count = populate_polygon_array(bad_triangles, bad_count, poly_arr);

                // --- REMOVE BAD TRIANGLES ---
                unsigned int *re_elems     = calloc (3 * tri_count  , sizeof(unsigned int));
                triangle *re_triangulation = calloc(tri_count    , sizeof(triangle));
                int count = 0;
                int good_in_bad_lists = 0;

                for (unsigned int each_tri = 0; each_tri < tri_count; each_tri++)
                {
                        good_in_bad_lists = 0;
                        triangle gt = triangulation[each_tri];
                        unsigned int a = elems[3*each_tri], b = elems[3*each_tri + 1], c = elems[3*each_tri + 2];

                        for (unsigned int each_trj = 0; each_trj < bad_count; each_trj++)
                        {
                                triangle bt = bad_triangles[each_trj];
                                unsigned int o = bad_elems[3*each_trj], p_ = bad_elems[3*each_trj + 1], q = bad_elems[3*each_trj + 2];

                                if (a == o && b == p_ && c == q)
                                {
                                        good_in_bad_lists = 1;
                                        break;
                                }
                        }
                        if (good_in_bad_lists) {continue;}
                        re_elems[3*count]       = a;
			re_elems[3*count+1]     = b;
			re_elems[3*count+2]     = c;
                        re_triangulation[count] = gt;
                        count++;
                }

                // tri_count <- good triangles at the beginning of the loop
                // bad_count <- bad  triangles found in triangulation
                // count     <- tri_count - bad_count - remaining triangles
                memcpy(elems, re_elems, 3 * count * sizeof(unsigned int));
                memcpy(triangulation, re_triangulation, count * sizeof(triangle));

                free(re_triangulation);

                free(re_elems);

                tri_count = count;

                // --- RE-TRIANGULATE THE HOLE ---
                for (unsigned int each_edg = 0; each_edg < poly_count; each_edg++)
                {
                        if (tri_count >= tri_capacity)
                        {
                                tri_capacity *= 2;
                                triangle *tmp = realloc(triangulation, tri_capacity * sizeof(triangle));
                                unsigned int *utmp = realloc(elems, 3 * tri_capacity *sizeof(unsigned int));

                                if (!tmp || !utmp) {
                                    printf("TMP ALLOCATIONS FAILED\n");
                                    free(triangulation);
                                    free(bad_triangles);
                                    free(poly_arr);
                                    free(elems);
                                    free(bad_elems);
                                    exit(-1);
                                }

                                triangulation = tmp;
                                elems = utmp;
                        }

                        edge edge_i = poly_arr[each_edg];
                        triangle new_tri = init_tri(edge_i.v0, edge_i.v1, p);
                        triangulation[tri_count] = new_tri;
                        // unsigned int a = poly_index[2*each_edg], b = poly_index[2*each_edg + 1];
                        // assuming the polygon function keeps track of all indices of the edges, then
                        // elems[3*tri_count] = a; elems[3*tri_count+1] = b; elems[3*tri_count+2] = c;
                        unsigned int a = poly_index[2*each_edg];
                        unsigned int b = poly_index[2*each_edg + 1];
                        unsigned int c = pi + 3;   // p is the i-th input point

                        elems[3*tri_count]   = a;
                        elems[3*tri_count+1] = b;
                        elems[3*tri_count+2] = c;
                        tri_count++;
                }
        }
        // --- CLEANUP ---
        free(bad_triangles);
        free(poly_arr);
        free(bad_elems);
        free(poly_index);
        int mesh_count = 0;
        int is_shared = 0;

        //mesh = realloc(mesh, tri_capacity * sizeof(triangle));

        for (unsigned int each_tri = 0; each_tri < tri_count; each_tri++)
        {
                triangle t = triangulation[each_tri];
                unsigned int a = elems[3*each_tri], b = elems[3*each_tri + 1], c = elems[3*each_tri + 2];

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

                if (!is_shared)
                {
                //mesh[mesh_count] = t;
		//printf("(%d, %d, %d)\n", a-3,b-3,c-3);
                mesh_elems[3*mesh_count]    = a - 3;
                mesh_elems[3*mesh_count+1]  = b - 3;
                mesh_elems[3*mesh_count+2]  = c - 3;
                mesh_count++;
                }
        }
        *out_count = mesh_count;
        free(triangulation);
        free(elems);
        //write_elems_to_file(mesh_elems, mesh_count, "elements.txt");
	return mesh_elems;
}

