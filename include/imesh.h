

float* generate_afinne_grid(const float x_low, const float x_high,
                        const float y_low, const float y_high, const int side);
int populate_polygon_array_index(const triangle *bad_tris, const int bad_count,
                        int *bad_index, edge *polygons, int *poly_index);
void* bidimensional_mesh(float *points, int n, int *out_count);
