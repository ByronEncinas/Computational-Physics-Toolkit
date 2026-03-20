# vec — C Scientific Computing Library

A lightweight linear algebra library in C providing vector and matrix operations for scientific computing. (documentation created with the help of Claude)

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

## Error Handling Convention

All operations return `int`: **0 on success, -1 on error**. Errors include size mismatches, NULL pointers, and invalid inputs (e.g. zero vector for `unitvec`). Always check the return value — a result written to an output pointer is only valid if the function returned 0.

```c
double result;
if (dot(&result, &a, &b) == -1){ /* handle error */ }
```

---

## Memory Management

> **Important:** Every `mcreate` and `vcreate` call allocates heap memory. Always free with `mfree` or `vfree` when done.

### `matrix mcreate(int m, int n)`
Allocates a matrix of size m×n. Returns `{0, 0, NULL}` on allocation failure — always check before use.
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

### `int vadd(vector *result, const vector *a, const vector *b)`
Element-wise addition: `result = a + b`. All three vectors must be the same size. Returns `-1` on size mismatch.
```c
vector a = vcreate(3), b = vcreate(3), r = vcreate(3);
a.data[0]=1; a.data[1]=2; a.data[2]=3;
b.data[0]=4; b.data[1]=5; b.data[2]=6;
if (vadd(&r, &a, &b) == -1) { /* size mismatch */ }
// r = [5, 7, 9]
```

### `int vsubs(vector *result, const vector *a, const vector *b)`
Element-wise subtraction: `result = a - b`. All three vectors must be the same size. Returns `-1` on size mismatch.
```c
if (vsubs(&r, &a, &b) == -1) { /* size mismatch */ }
```

### `int dot(double *result, const vector *a, const vector *b)`
Dot product of two vectors, written to `*result`. Returns `-1` on size mismatch.
```c
double d;
if (dot(&d, &a, &b) == -1) { /* size mismatch */ }
```

### `int cross(vector *result, const vector *a, const vector *b)`
Cross product. **All three vectors must be in R³ (n=3)**. Returns `-1` otherwise.
```c
vector a = vcreate(3), b = vcreate(3), r = vcreate(3);
if (cross(&r, &a, &b) == -1) { /* not R³ */ }
```

### `int norm(double *result, const vector *a)`
Euclidean norm (magnitude) of a vector, written to `*result`. Returns `-1` if `a` or `a->data` is NULL.
```c
double mag;
if (norm(&mag, &v) == -1) { /* null vector */ }
```

### `int unitvec(vector *u, const vector *v)`
Writes the unit vector of `v` into `u`. Returns `-1` if `v` is NULL, `v->data` is NULL, or `v` is the zero vector.
```c
vector u = vcreate(3);
if (unitvec(&u, &v) == -1) { /* null or zero vector */ }
```

---

## Matrix Operations

### `int matmult(matrix *result, const matrix *a, const matrix *b)`
Matrix multiplication: `result = a * b`.

- `a` must be m×k, `b` must be k×n, `result` must be pre-allocated as m×n
- Returns `-1` on dimension mismatch

```c
matrix a = mcreate(2, 3); // 2x3
matrix b = mcreate(3, 4); // 3x4
matrix r = mcreate(2, 4); // 2x4 result
// ... fill a and b ...
if (matmult(&r, &a, &b) == -1) { /* dimension mismatch */ }
```

> **Note:** `result` must be allocated before calling `matmult`. The function zeroes it internally.

### `int matrow(vector *v, const matrix *a, int row)`
Makes a vector point to a row of a matrix **without copying**. The vector borrows the matrix's memory. Returns `-1` if `a->data` is NULL or `row` is out of bounds.

```c
vector v;
if (matrow(&v, &mat, 0) == -1) { /* out of bounds or null */ }
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
double n;
norm(&n, &row);            // safe — just reading
// do NOT vfree(&row)
mfree(&weights);           // free the matrix when done
```

### Chaining operations
```c
vector a = vcreate(3), b = vcreate(3), result = vcreate(3);
// fill a and b ...
vadd(&result, &a, &b);
double magnitude;
norm(&magnitude, &result);
printf("magnitude: %f\n", magnitude);
vfree(&a); vfree(&b); vfree(&result);
```

---

## Internals

### Memory layout
Matrices are stored in row-major order as a flat 1D array. Element `[i][j]` of an m×n matrix lives at index `i*n + j` in `data`.

---

## What's not yet implemented
- Scalar multiplication (`vscale`, `mscale`)
- Matrix transpose
- Identity matrix initialization
- LU decomposition / linear system solving
