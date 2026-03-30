#include "../include/geom.h" // contains the vertex, edge and triangle structs
#include "../include/mesh.h"
#include <stdio.h>
#include <stdlib.h>


void try_bw_mesh() {

        unsigned int n = 1000;
	unsigned int mesh_size;

        float points[2*n]; // 2*n

        triangle *mesh = malloc(2 * n * sizeof(triangle));

        for (unsigned int i = 0; i < n; i++) {
                points[2*i]     = rand_within(0, 10);
                points[2*i + 1] = rand_within(0, 10);
		//printf("  p%d: (%.2f, %.2f)\n", i, points[2*i], points[2*i+1]);
        }
	// bowyer_watson_mesh(float *points, int n, triangle *triangle_mesh, int out_count)

        bowyer_watson_mesh(points, n, mesh, &mesh_size);

	for (unsigned int i=0; i < mesh_size; i++)
	{
		printf("\ntriangle %d:\n", i);
		printf("  v0: (%.2f, %.2f)\n", mesh[i].v0.x, mesh[i].v0.y);
		printf("  v1: (%.2f, %.2f)\n", mesh[i].v1.x, mesh[i].v1.y);
		printf("  v2: (%.2f, %.2f)", mesh[i].v2.x, mesh[i].v2.y);
	}
        printf("\npoint %d\n", mesh_size-1);
}

void try_linear_mesh() {

        int n = 10;
        float *nodes = malloc(n * sizeof(float)); // in heap
        int *elems = malloc(n * 2 * sizeof(int));
        float h[] = {0.0, 0.2, 0.3, 0.4, 0.5, 0.4, 0.3, 0.2, 0.5};
        int m = 0;

        linear_mesh(nodes, elems, h, n, 1);

        // print nodes
        printf("nodes:\n");

        for (int i = 0; i < n; i++)
        {
                printf("  nodes[%d] = %f\n", i, nodes[i]);
        }

        // print elems as pairs
        printf("elems:\n");

        for (int i = 0; i < n-1; i++)
        {
                printf("  elem[%d] = [%d, %d]\n", i, elems[2*i], elems[2*i+1]);
        }

        free(nodes);
        free(elems);


        for (int i = 0; i < 10; i++)
        {
                printf("%f\n", rand_within(0.0, 1.0));
        }
}

int main() {

        try_bw_mesh();

        return 0;
}
