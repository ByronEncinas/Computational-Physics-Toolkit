cmake_minimum_required(VERSION 3.10)
project(MyProject C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")

list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.c")

add_executable(main src/main.c)
target_include_directories(main PUBLIC include/)

# Only build lib if there are other source files
if(SOURCES)
    add_library(lib ${SOURCES} ${HEADERS})
    target_include_directories(lib PUBLIC include/)
    target_link_libraries(main lib)
endif()
