# **step-by-step explanation of `CMakeLists.txt` at this scope**.

| Step | CMake Code                                                   | What it Does                                                                                              | Why it Matters                                                                        |
| ---- | ------------------------------------------------------------ | --------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------- |
| 1    | `cmake_minimum_required(VERSION 3.10)`                       | Specifies the minimum CMake version required to configure the project.                                    | Prevents older CMake versions from running the project incorrectly.                   |
| 2    | `project(CPT C)`                                             | Declares the project name (`CPT`) and enables the C language.                                             | Initializes internal variables and tells CMake which compiler toolchain to configure. |
| 3    | `set(CMAKE_C_STANDARD 11)`                                   | Sets the C language standard to C11.                                                                      | Ensures the compiler uses `-std=c11` or equivalent.                                   |
| 4    | `file(GLOB_RECURSE SOURCES "src/*.c")`                       | Searches the `src/` directory recursively and collects all `.c` source files into the `SOURCES` variable. | Automatically gathers all C source files instead of listing them manually.            |
| 5    | `list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.c")` | Removes `src/main.c` from the `SOURCES` list.                                                             | Prevents the `main()` program from being compiled into the shared library.            |
| 6    | `if(SOURCES)`                                                | Checks whether the `SOURCES` list is not empty.                                                           | Prevents creating a library if there are no source files.                             |
| 7    | `add_library(cpt SHARED ${SOURCES})`                         | Creates a **shared library target** called `cpt` from the files in `SOURCES`.                             | Produces a shared object used by other programs (e.g., Python `ctypes`).              |
| 8    | `target_include_directories(cpt PUBLIC include)`             | Adds the `include/` directory to the compiler’s header search path for the library.                       | Allows library source files to use headers with `#include`.                           |
| 9    | `endif()`                                                    | Ends the conditional block that builds the library.                                                       | Keeps the build logic organized and conditional.                                      |
| 10   | `add_executable(main src/main.c)`                            | Creates an executable program called `main` from `src/main.c`.                                            | Builds a runnable program for testing or using the library.                           |
| 11   | `target_include_directories(main PUBLIC include)`            | Adds the `include/` directory to the compiler search path for the executable.                             | Allows `main.c` to include project headers.                                           |
| 12   | `if(SOURCES)`                                                | Checks again that the library sources exist.                                                              | Ensures the executable only links the library if it was created.                      |
| 13   | `target_link_libraries(main PRIVATE cpt)`                    | Links the executable `main` against the `cpt` library.                                                    | Allows the program to call functions defined in the library.                          |
| 14   | `endif()`                                                    | Ends the conditional linking block.                                                                       | Completes the conditional logic.                                                      |

---

### Build result

After running:

```bash
cmake -S . -B build
cmake --build build
```

Typical output on Linux:

```
build/
├── libcpt.so   ← shared library
└── main        ← executable
```

The **`libcpt.so`** file is what Python loads with `ctypes`.

Example:

```python
import ctypes
lib = ctypes.CDLL("./libcpt.so")
```

---
