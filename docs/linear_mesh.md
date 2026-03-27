# How to use 'linear_mesh function'

To use the function linear mesh at its current form, one has to allocate to the 
heap an array with the size that corresponds to the number of nodes needed.

As one can see in `mesh.h`, the function `linear_mesh` returns a void, and takes 
five arguments. The most importants differentiate bewteen special cases.

| Parameter | Type         | Purpose                                                                 | Notes / Example |
|-----------|-------------|-------------------------------------------------------------------------|----------------|
| `nodes`  | `float*`    | Stores coordinates of each node                                         | Size ≥ n, e.g., `[0, 1, 2, 3]` |
| `elems`  | `int (ptr)`       | Stores connectivity of elements (pairs of node indices)                 | Size ≥ 2*n, e.g., `[0,1, 1,2, 2,3, 3,4]` |
| `h`      | `float (ptr)`    | Node spacing: single float (uniform) or array (adaptive)               | `0.5` (uniform) or `[0.1,0.2,0.3]` (adaptive) |
| `n`      | `int`       | Number of nodes                                                         | e.g., `4` |
| `m`      | `int`       | Mesh type flag: 0 = uniform, 1 = adaptive 


```
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/mesh.h"

int main() {

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

    return 0;
}
```

# Algorithm for Bowyer Watson from Wikipedia

```
SETUP
    allocate a dynamic array of triangles  ← your "triangulation"
    create the super-triangle
    triangles[0] = super-triangle
    count = 1
```

```
FOR EACH point P:

    // --- FIND BAD TRIANGLES ---
    allocate badTriangles array (at most 'count' entries)
    bad_count = 0

    for i in 0..count:
        if P is inside circumcircle of triangles[i]:
            badTriangles[bad_count++] = triangles[i]


    // --- FIND BOUNDARY POLYGON ---
    // an edge is on the boundary if it appears in
    // exactly ONE bad triangle (not shared by two)
    allocate polygon array of edges
    poly_count = 0

    for each triangle T in badTriangles:
        for each edge E of T:                   ← 3 edges per triangle
            shared = false
            for each other triangle T2 in badTriangles:
                if T2 has edge E:
                    shared = true
                    break
            if not shared:
                polygon[poly_count++] = E


    // --- REMOVE BAD TRIANGLES ---
    // you can't just free() individual entries from an array
    // simplest approach: rebuild the array skipping bad ones
    new_count = 0
    for i in 0..count:
        if triangles[i] is NOT in badTriangles:
            triangles[new_count++] = triangles[i]
    count = new_count


    // --- RE-TRIANGULATE THE HOLE ---
    for each edge E in polygon:
        new_triangle = init_tri(E.v0, E.v1, P)
        // check capacity before inserting
        if count >= capacity:
            capacity *= 2
            realloc triangles
        triangles[count++] = new_triangle

```

```
CLEANUP
    new_count = 0
    for i in 0..count:
        if triangles[i] shares NO vertex with super-triangle:
            triangles[new_count++] = triangles[i]
    count = new_count

    return triangles, count
```
