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
