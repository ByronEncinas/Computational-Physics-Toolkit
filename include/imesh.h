#include <stdio.h>
#include "geom.h"

static int ccw( const int ax, const int ay,
                const int bx, const int by,
                const int cx, const int cy);
static int segments_intersection( const int ax, const int ay,
                                  const int bx, const int by,
                                  const int cx, const int cy,
                                  const int dx, const int dy);
float* generate_custom_grid(int plen, const float *hull, const int hlen,
                        const float l, const float h);
int point_inside_hull(const float *hull, const int length, const float x, const float y);

static float* generate_afinne_grid(const float x_low, const float x_high,
                        const float y_low, const float y_high, const int side);
static int populate_polygon_array_index(const triangle *bad_tris, const int bad_count,
                        int *bad_index, edge *polygons, int *poly_index);

void* bidimensional_mesh(const float *points, const int n, int *out_count,
                                                const float *hull, const int hull_size);
