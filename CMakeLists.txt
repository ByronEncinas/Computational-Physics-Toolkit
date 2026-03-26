# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(CPT C)

# Use the C11 standard for compilation
set(CMAKE_C_STANDARD 11)

# Find all .c source files recursively inside the src/ directory
file(GLOB_RECURSE SOURCES "src/*.c")

# Remove the main program file from the library sources
# (we don't want main() inside the shared library)
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.c")

# If there are source files besides main.c
if(SOURCES)

    # Create a shared library from the source files
    # This generates:
    #   Linux  -> libmylib.so
    #   macOS  -> libmylib.dylib
    #   Windows -> mylib.dll
    add_library(cpt SHARED ${SOURCES})

    # Add the include/ directory so headers can be found
    target_include_directories(cpt PUBLIC include)
    target_link_libraries(cpt m)
endif()

# Build the executable program from main.c
add_executable(main src/main.c)

# Allow the executable to see headers in include/
target_include_directories(main PUBLIC include)

# If the shared library was created, link it to the executable
# so main can call functions from the library
if(SOURCES)
    target_link_libraries(main PRIVATE cpt m)
    target_link_libraries(cpt m)
endif()
