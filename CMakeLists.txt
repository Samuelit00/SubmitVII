cmake_minimum_required(VERSION 3.10)
project(ParallelAlgorithms)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar OpenMP
find_package(OpenMP REQUIRED)

if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP encontrado")
else()
    message(FATAL_ERROR "OpenMP no encontrado. Por favor instálalo.")
endif()

# Agregar ejecutables
add_executable(shearsort shearsort.cpp)
add_executable(binarysearch binarysearch.cpp)

# Enlazar OpenMP
target_link_libraries(shearsort OpenMP::OpenMP_CXX)
target_link_libraries(binarysearch OpenMP::OpenMP_CXX)

# Opciones de compilación para optimización
if(MSVC)
    target_compile_options(shearsort PRIVATE /O2 /openmp)
    target_compile_options(binarysearch PRIVATE /O2 /openmp)
else()
    target_compile_options(shearsort PRIVATE -O3 -fopenmp)
    target_compile_options(binarysearch PRIVATE -O3 -fopenmp)
endif()
