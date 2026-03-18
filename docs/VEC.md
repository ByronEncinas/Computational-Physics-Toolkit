# vec — C Scientific Computing Library

A lightweight linear algebra library in C providing vector and matrix operations for scientific computing.

---

## Building

```bash
gcc -O2 -march=native vec.c -o your_program -lm
```

`-lm` is required for `sqrt` and `fabs`. `-O2` is strongly recommended for performance.

---

## Data Structures

### `vector`
A dynamically allocated n-dimensional vector.
```c
typedef struct {
    int n;        // number of elements
    double *data; // heap-allocated array of doubles
} vector;
```

### `matrix`
A dynamically allocated m×n matrix stored in **row-major order** — rows are contiguous in memory.
```c
typedef struct {
    int m, n;     // m rows, n columns
    double *data; // heap-allocated flat array of size m*n
} matrix;
```
Element `[i][j]` is accessed as `data[i*n + j]`.

---

## Memory Management

> **Important:** Every `mcreate` and `vcreate` call allocates heap memory. Always free with `mfree` or `vfree` when done.

### `matrix mcreate(int m, int n)`
Allocates a matrix of size m×n. Returns `{0, NULL}` on allocation failure — always check before use.
```c
matrix a = mcreate(3, 3);
if (a.data == NULL) { /* handle error */ }
```

### `void mfree(matrix *m)`
Frees the matrix data and zeroes the struct.
```c
mfree(&a);
```

### `vector vcreate(int n)`
Allocates a vector of size n. Internally uses `mcreate`. Returns `{0, NULL}` on failure.
```c
vector v = vcreate(3);
if (v.data == NULL) { /* handle error */ }
```

### `void vfree(vector *v)`
Frees the vector data and zeroes the struct.
```c
vfree(&v);
```

---

## Vector Operations

### `void vadd(vector *result, const vector *a, const vector *b)`
Element-wise addition: `result = a + b`. All three vectors must be the same size, silently returns on mismatch.
```c
vector a = vcreate(3), b = vcreate(3), r = vcreate(3);
a.data[0]=1; a.data[1]=2; a.data[2]=3;
b.data[0]=4; b.data[1]=5; b.data[2]=6;
vadd(&r, &a, &b); // r = [5, 7, 9]
```

### `void vsubs(vector *result, const vector *a, const vector *b)`
Element-wise subtraction: `result = a - b`. All three vectors must be the same size.
```c
vsubs(&r, &a, &b); // r = a - b
```

### `double dot(const vector *a, const vector *b)`
Returns the dot product of two vectors. Returns `0.0` on size mismatch.
```c
double d = dot(&a, &b);
```

### `void cross(vector *result, const vector *a, const vector *b)`
Cross product. **All three vectors must be in R³ (n=3)**, silently returns otherwise.
```c
vector a = vcreate(3), b = vcreate(3), r = vcreate(3);
cross(&r, &a, &b);
```

### `double norm(const vector *a)`
Returns the Euclidean norm (magnitude) of a vector.
```c
double magnitude = norm(&v);
```

---

## Matrix Operations

### `void matmult(matrix *result, const matrix *a, const matrix *b)`
Matrix multiplication: `result = a * b`.

- `a` must be m×k, `b` must be k×n, `result` must be pre-allocated as m×n
- Silently returns on dimension mismatch
- Uses cache-friendly loop ordering for performance

```c
matrix a = mcreate(2, 3); // 2x3
matrix b = mcreate(3, 4); // 3x4
matrix r = mcreate(2, 4); // 2x4 result
// ... fill a and b ...
matmult(&r, &a, &b);
```

> **Note:** `result` must be allocated before calling `matmult`. The function zeroes it internally.

### `void matrow(vector *v, const matrix *a, int row)`
Makes a vector point to a row of a matrix **without copying**. The vector borrows the matrix's memory.

```c
vector v;
matrow(&v, &mat, 0); // v now points to row 0 of mat
```

> ⚠️ **Ownership warning:** Do NOT call `vfree` on a vector obtained from `matrow` — it does not own the memory. The matrix owns it. Freeing `v` will corrupt the matrix.
> 
> Also, if the matrix is freed, `v` becomes a dangling pointer and must not be used.

---

## Common Patterns

### Matrix-vector access
```c
matrix weights = mcreate(4, 3);
// ... fill weights ...

vector row;
matrow(&row, &weights, 2); // borrow row 2
double n = norm(&row);     // safe — just reading
// do NOT vfree(&row)
mfree(&weights);           // free the matrix when done
```

### Chaining operations
```c
vector a = vcreate(3), b = vcreate(3), result = vcreate(3);
// fill a and b ...
vadd(&result, &a, &b);
double magnitude = norm(&result);
printf("magnitude: %f\n", magnitude);
vfree(&a); vfree(&b); vfree(&result);
```

---

## Internals

### Memory layout
Matrices are stored in row-major order as a flat 1D array. Element `[i][j]` of an m×n matrix lives at index `i*n + j` in `data`.

### Why row-major matters for performance
`matmult` uses a reordered loop (ikj instead of ijk) so the inner loop always walks sequentially through memory. This keeps data in CPU cache and avoids the costly random-access pattern of the naive nested loop.

### Error handling convention
All functions that receive mismatched sizes return silently (void functions) or return `0.0` (dot product). Always ensure dimensions match before calling. A future version may add explicit error codes.

---

## What's not yet implemented
- Scalar multiplication (`vscale`, `mscale`)
- Matrix transpose
- Identity matrix initialization
- LU decomposition / linear system solving
