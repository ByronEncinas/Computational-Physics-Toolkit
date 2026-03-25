#include "geom.h" // contains the vertex, edge and triangle structs
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

        int n = 10;
        float *nodes = malloc(n * sizeof(float)); // in heap
        int *elems = malloc(n * 2 * sizeof(int));
        float h[] = {0.0, 0.2, 0.3, 0.4, 0.5, 0.4, 0.3, 0.2, 0.5};
        int m = 0;

        mesh1d(nodes, elems, h, n, 1);


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


        for (int i = 0; i < 100; i++)
        {
                printf("%f\n", rand_within(0.0, 1.0));
        }
        return 0;
        }
