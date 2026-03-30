#include "../include/geom.h" // contains the vertex, edge and triangle structs
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void bowyer_watson_mesh(float *points, int n, triangle *mesh, int *out_count) {
/*	2D tesselation algorithm

paramters:

input:
        points: array with n point coordinates which we want to mesh (size n)
	n:      size of points // 2
output:
	...
	*/
	// initializes super triangle
	triangle sup_tri = super_triangle(points, n);

	printf("\n  (%.2f, %.2f)\n", sup_tri.v0.x, sup_tri.v0.y);     // 2 decimal places
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
	/*
	For a planar triangulation,

	V: number of corners (a point)
	E: number of edges   (a line)
	F: number of faces (interior triangles + outer face)

	There will be 3*F - 3 interior edges if there are F interior triangles
	Edges are shared, there will the number of interior edges is roughly 2E - h
	with h being the boundary edges (points in the convex hull)

	this leads to the number of triangles

	F = 2V - 1 - h
	E = 3n - 3 - h

	if we ignore the h as a safety net

        (# triangles) = 2 (# points) - 1
        (# edges)     = 3 (# points) - 3

	source:
	  - https://math.stackexchange.com/questions/1286800/euler-formula-for-planar-graphs?utm_source=chatgpt.com
	*/

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

                        //printf("FOUND BAD TRIANGLES %d:\n", bad_count);
                        //printf("  v0: (%.2f, %.2f)\n", t.v0.x, t.v0.y);
                        //printf("  v1: (%.2f, %.2f)\n", t.v1.x, t.v1.y);
                        //printf("  v2: (%.2f, %.2f)\n", t.v2.x, t.v2.y);
			}
		}

		tri_count = tri_count - bad_count;

		// --- FIND BOUNDARY POLYGON ---

		free(poly_arr);
		poly_arr = malloc(3 * bad_count * sizeof(edge));
		poly_count = populate_polygon_array(bad_triangles, bad_count, poly_arr);

                //printf("poly_count is: %u\n", poly_count);

		// --- REMOVE BAD TRIANGLES ---
		triangle *rebuilt = calloc(max_triangle_size, sizeof(triangle)); // whatever the size nee>
		rebuilt_count = 0; // reasign

		// loop over triangulation
                for (unsigned int each_tri = 0; each_tri < good_count; each_tri++)
                {
			// compare elements of triangulation with bad_triangles
			is_gt_in_bd = 0;
                        triangle gt = triangulation[each_tri];

	                for (unsigned int each_trj = 0; each_trj < bad_count; each_trj++)
			{
				triangle bt = bad_triangles[each_trj];
				// if gt and bt are similar, then bad_triangle in triangulation,
				// gt is not useful
	                        if (equal_triangles(&bt, &gt))
				{
					is_gt_in_bd = 1;
					tri_count = tri_count - 1;
					break;
				}

			}
			if (is_gt_in_bd) {continue;}

                	//printf("good triangle? yes - %d:\n", rebuilt_count);
                        //printf("REMOVE BAD TRIANGLES %d:\n", each_tri);
                        //printf("  v0: (%.2f, %.2f)\n", gt.v0.x, gt.v0.y);
                        //printf("  v1: (%.2f, %.2f)\n", gt.v1.x, gt.v1.y);
                        //printf("  v2: (%.2f, %.2f)\n", gt.v2.x, gt.v2.y);

                        rebuilt[rebuilt_count++] = gt;
			tri_count = tri_count + 1;
                }

		//free(triangulation);
		triangulation = realloc(triangulation, (poly_count + rebuilt_count) * sizeof(triangle));

                //printf("mesh size: %d:\n", tri_count);

		memcpy(triangulation, rebuilt, rebuilt_count * sizeof(triangle));
		good_count = rebuilt_count;
		//free(rebuilt);

		// --- RE-TRIANGULATE THE HOLE ---
		// append to triangulation new triangles
		for (unsigned int each_edg = 0; each_edg < poly_count; each_edg++)
		{
			edge edge_i = poly_arr[each_edg];
			triangle new_tri = init_tri(edge_i.v0, edge_i.v1, p);
			triangulation[good_count++] = new_tri;

		//printf("good triangles -> %d", good_count);
                //printf("triangle %d:\n", count);
                //printf("  v0: (%.2f, %.2f)\n", mesh[count].v0.x, mesh[count].v0.y);
                //printf("  v1: (%.2f, %.2f)\n", mesh[count].v1.x, mesh[count].v1.y);
                //printf("  v2: (%.2f, %.2f)\n", triangulation[count].v2.x, triangulation[count].v2.y);
		}
		tri_count = tri_count + good_count;

                //printf("mesh size: %d:\n", tri_count);
	}

	// --- CLEANUP ---
	// if triangle shares vertex with sup_triangle, then delete
	triangle *rebuilt = realloc(rebuilt, good_count * sizeof(triangle));
	rebuilt_count = 0;
	mesh_count = 0;

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
                //printf("  v0: (%.2f, %.2f)\n", mesh[rebuilt_count-1].v0.x, mesh[rebuilt_count-1].v0.y);
                //printf("  v1: (%.2f, %.2f)\n", mesh[rebuilt_count].v1.x, mesh[rebuilt_count].v1.y);
                //printf("  v2: (%.2f, %.2f)\n", mesh[rebuilt_count].v2.x, mesh[rebuilt_count].v2.y);
		}
	}

        printf("mesh size: %d:\n", mesh_count);
	free(bad_triangles);
	free(poly_arr);
	free(triangulation);
	mesh = realloc(mesh, mesh_count * sizeof(triangle)); 
	*out_count = mesh_count;
        //free(rebuilt);
	*out_count = mesh_count;
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

/*
void read_inp(float *nodes, float *elems, FILE *fptr) {
    
    int ids[4];
    double x[4], y[4], z[4];
    char buff[256];

    int count = 0;    
    while (fgets(buff, sizeof(buff), fptr) != NULL) {
        if (sscanf(buff, " %d, %lf, %lf, %lf",
                   &ids[count], &x[count], &y[count], &z[count]) == 4) {
            count++;
        }
    }
    // now loop over what you parsed
    for (int i = 0; i < count; i++) {
        printf("Node %d: (%.1f, %.1f, %.1f)\n", ids[i], x[i], y[i], z[i]);
    }    
}


int mesh_from_file() {

    printf("Supported Formats:\n"
           "4 ------- 3\n"
           "|  \\  T2 |\n"
           "| T1  \\  |\n"
           "1 ------- 2\n\n");

    FILE *fptr = fopen("file.txt", "r");
    char buff[256];

    if (fptr == NULL) {
        printf("File not opened!\n");
        return 1;
    }

    float *nodes = malloc(4 * sizeof(float)); // ID, X, Y, Z
    float *elems = malloc(4 * sizeof(float)); // ID, I, J, K

    int line_count = 0;
    int node_count = 0;
    int elem_count = 0;
    
    char NODE_FLAG[10];
    char ELEM_FLAG[10];
    //read_inp(nodes, elems, buff, fptr);        
    while (fgets(buff, sizeof(buff), fptr) != NULL)
    {
        if (line_count == 0) {
            
            if (strstr(buff, "*NODE"))
            {
            char type = 'N';
            strcpy(ELEM_FLAG, "*NODE");
            strcpy(ELEM_FLAG, "*ELEMENT");
            } 
            else if (strstr(buff, "GRID" ))
            {
            char type = 'G'; // which file we'll read
            strcpy(ELEM_FLAG, "GRID");
            strcpy(ELEM_FLAG,"CTRIA");
            }
        } 
        if (!strstr(buff, ELEM_FLAG) && line_count != 0)
        {// if buff is found after NODE_FLAG
            
            printf("%s", buff);
            node_count++;
        }

        line_count++;
        
        if (strstr(buff, ELEM_FLAG)) 
        {
            break;
        }
    }
    elem_count = line_count - node_count;
    printf("\n#Lines, #Nodes, #Elems (%i, %i, %i)\n", line_count, node_count, elem_count);

    float *nodes = malloc(4 * sizeof(float)); // ID, X, Y, Z
    float *elems = malloc(4 * sizeof(float)); // ID, I, J, K


    fclose(fptr);
    return 0;

}

*/
