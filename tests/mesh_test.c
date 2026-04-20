#include <stdio.h>
#include <stdlib.h>
#include "../include/toolkit.h"

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

void try_imesh() {

        int side = 5;
        int mesh_size;
        float *points = calloc(2*side*side, sizeof(float));
	//points        = generate_custom_grid(side, hull, hull_size, -0.5, 1);
        points        = generate_uniform_grid(-0.5, 1, side);
        printf("nodes = [\n");
        for (unsigned int i=0; i < side*side; i++)
        {
               printf("%u (%.5f, %.5f)\n", i, points[2*i], points[2*i+1]);
        }
        printf("]\n");

	int *mesh = (int*)bidimensional_mesh(points, side*side, &mesh_size, hull, hull_size);

        printf("elements = [\n");
        for (unsigned int i=0; i < mesh_size; i++)
        {
                printf("(%u, %d, %d),\n", i, mesh[3*i], mesh[3*i+1], mesh[3*i+2]);
        }
        printf("]\n");

}

void try_bw_mesh() {

        int side = 3;
        int mesh_size;

        float *points = calloc(2*side*side, sizeof(float));

        points = generate_uniform_grid(0, 1, side);
        printf("nodes = [\n");
        for (unsigned int i=0; i < side*side; i++)
        {
               printf("(%.5f, %.5f),\n", i, points[2*i], points[2*i+1]);
        }
        printf("]\n");
        triangle *mesh = bowyer_watson_mesh(points, side*side, &mesh_size);

        printf("elements = {\n");
        for (unsigned int i=0; i < mesh_size; i++)
        {
                printf("'%d':\n", i);
		printf("{\n'v0': (%.2f, %.2f),\n", mesh[i].v0.x, mesh[i].v0.y);
                printf(   "'v1': (%.2f, %.2f),  \n", mesh[i].v1.x, mesh[i].v1.y);
                printf(   "'v2': (%.2f, %.2f) },\n", mesh[i].v2.x, mesh[i].v2.y);
        }
        printf("}\n");
        printf("\nElements: %d\n", mesh_size-1);
}

int main() {
	try_imesh();
	//try_bw_mesh();
        return 0;

}
