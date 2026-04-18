#include "../include/geom.h" // contains the vertex, edge and triangle structs
#include "../include/imesh.h"
#include "../include/mesh.h"
#include <stdio.h>
#include <stdlib.h>


const int hull_size = 53;
const float hull[] = {
    0.0000, 0.2210,
    0.0000, 0.3296,
    0.0000, 0.4395,
    0.0000, 0.5493,
    0.0000, 0.6591,
    0.0000, 0.7689,
    0.0000, 0.8787,
    0.0286, 0.9576,
    0.0926, 1.0000,
    0.1843, 0.9806,
    0.2741, 0.9115,
    0.3692, 0.8242,
    0.4196, 0.7507,
    0.4556, 0.6727,
    0.4690, 0.5600,
    0.4595, 0.4461,
    0.4157, 0.3467,
    0.3494, 0.2480,
    0.2397, 0.1628,
    0.2294, 0.1425,
    0.2090, 0.1374,
    0.1909, 0.1266,
    0.1769, 0.1111,
    0.1678, 0.0921,
    0.1647, 0.0713,
    0.1647, 0.0712,
    0.1647, 0.0604,
    0.1647, 0.0603,
    0.1640, 0.0496,
    0.1640, 0.0331,
    0.1640, 0.0165,
    0.1640, 0.0000,
    0.1324, 0.0315,
    0.1313, 0.0466,
    0.1302, 0.0616,
    0.1191, 0.0774,
    0.1028, 0.0879,
    0.0837, 0.0913,
    0.0647, 0.0874,
    0.0564, 0.0786,
    0.0481, 0.0698,
    0.0145, 0.0725,
    0.0216, 0.0870,
    0.0287, 0.1016,
    0.0358, 0.1161,
    0.0390, 0.1242,
    0.0390, 0.1243,
    0.0455, 0.1377,
    0.0507, 0.1556,
    0.0497, 0.1742,
    0.0426, 0.1916,
    0.0302, 0.2054,
    0.0139, 0.2145,
    0.0000, 0.2210
};

int point_inside_hull(const float *hull, const int length, const float x, const float y) {

        int crossings = 0;

        for (int i = 0; i < length; i++)
        {
		int j = (i + 1) % length;
                float vix = hull[2*i], viy = hull[2*i+1];
                float vjx = hull[2*j], vjy = hull[2*j+1];
		if (fabsf(vjy - viy) < TOLERANCE) {continue;}
                float intersect = vix + (vjx - vix) * (y - viy) / (vjy - viy);
		if (((viy > y) != (vjy > y)) && (x < intersect)){crossings++;}
        }
        return (crossings % 2 == 1);}

void try_imesh() {

        int side = 50;
        int mesh_size;

        float *points = calloc(2*side*side, sizeof(float)); // 2*n
        //float* generate_afinne_grid(const float x_low, const float x_high,
        //                const float y_low, const float y_high, const int side)
        //points = generate_afinne_grid(xl, xh, yl, yh, side);
        points = generate_uniform_grid(-0.5, 1, side);
        printf("[\n");
        for (unsigned int i=0; i < side*side; i++)
        {
                int l = point_inside_hull(hull, hull_size, points[2*i], points[2*i+1]);
		printf("(%.5f, %.5f, %u),\n", points[2*i], points[2*i+1], l);
        }
        printf("]\n");

	int *mesh = (int*)bidimensional_mesh(points, side*side, &mesh_size);

        printf("\n# Elements %d:\n", mesh_size);
        for (unsigned int i=0; i < mesh_size; i++)
        {
                printf("\nElement %d:\n", i);
                printf("E: %d\n", mesh[i]);
        }
}

void try_bw_mesh() {

        int side = 3;
	int mesh_size;

        float *points = calloc(2*side*side, sizeof(float)); // 2*n

        points = generate_random_grid(0, 1, n);

        for (unsigned int i=0; i < side*side; i++)
        {
               printf("\nPoints %d:\n", i);
               printf("  P: %u (%.5f, %.5f)\n", i, points[2*i], points[2*i+1]);
        }

        triangle *mesh = bowyer_watson_mesh(points, side*side, &mesh_size);

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
	try_imesh();
        return 0;

}
