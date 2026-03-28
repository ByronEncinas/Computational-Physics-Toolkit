# To compile your test 

with the help of the code in `include/` and `src/`

Make sure your `test.c` imports correctly the libraries needed.

If we take as an example `mesh_test.c`, we can see the following.

    #include "../include/geom.h"
    #include "../include/mesh.h"
    #include <stdio.h>
    #include <stdlib.h>

We don't need to import the files at `src/` because the header files already pull the functions from there. But, and here is the tricky part, we will need to pre-compile them.

**In this example we use the compiler `gcc`**

From the `tests/` we do the following

    gcc -c mesh.c      → mesh.o
    gcc -c geom.c      → geom.o
    gcc -c mesh_test.c → mesh_test.o

This will create object files with extension `.o`, this files can be passed onto the compiler once again to assemble the executable.

    gcc mesh_test.o mesh.o geom.o -o executable

This will yield an error. Why? because we are using the math library. We need to link it too with the executable, and for that we use the `-lm` flag.

    gcc mesh_test.o mesh.o geom.o -lm -o executable

And finally, we got it. We create an executable