#include <stdio.h>
#include "../include/vec.h"
#include <stdlib.h>

int main(){

    vector a = vcreate(3);
    a.data[0] = 1; a.data[1] = 0; a.data[2] = 0;
    vector b = vcreate(3);
    b.data[0] = 0; b.data[1] = 1; b.data[2] = 0;
    vector c = vcreate(3);

    cross(&c, &a, &b);

    printf("%f,%f,%f", c.data[0], c.data[1], c.data[2]);
    
//Eliminate this comment later
    vadd(&c, &a, &b);
    printf("%f,%f,%f", c.data[0], c.data[1], c.data[2]);

    double d = dot(&a, &b);
    printf("%f", d);

    return 0;
}
