# Algoritmos de Ordenamiento y Búsqueda en Paralelo

Implementación de algoritmos paralelos usando OpenMP en C++:
- **Shearsort**: Ordenamiento paralelo de matrices n×n
- **P-BSA**: Búsqueda binaria paralela

## 📦 Archivos

### Código Fuente
- `shearsort.cpp` - Algoritmo Shearsort (secuencial, paralelo y alternativo)
- `binarysearch.cpp` - Algoritmo P-BSA (búsqueda binaria paralela)

### Ejecutables
- `shearsort.exe` - Programa compilado de Shearsort
- `binarysearch.exe` - Programa compilado de P-BSA

### Documentación
- `README.md` - Este archivo

## � Compilación

### Linux/Docker:

# manualmente:
g++ -std=c++14 -fopenmp -O3 shearsort.cpp -o shearsort.exe
g++ -std=c++14 -fopenmp -O3 binarysearch.cpp -o binarysearch.exe
```

### Windows:
# manualmente:
g++ -std=c++14 -fopenmp -O3 shearsort.cpp -o shearsort.exe
g++ -std=c++14 -fopenmp -O3 binarysearch.cpp -o binarysearch.exe
```

## 🚀 Ejecución

```bash
# Ejecutar Shearsort
./shearsort.exe

# Ejecutar Binary Search
./binarysearch.exe

# Ejecutar todo y generar reporte
bash run_tests.sh  # Linux
.\run_tests.ps1    # Windows
```

## 📊 Resultados

### Shearsort
- Speedup máximo: **1.77x** con 4 hilos (matriz 128×128)
- Efectivo para matrices grandes (n ≥ 64)
- Complejidad: O(n² log² n) secuencial, O(n log² n) paralelo

### P-BSA
- Speedup < 1.0 (overhead supera beneficio)
- Búsqueda binaria es O(log n) - poco trabajo para paralelizar
- Mejor para múltiples búsquedas simultáneas


## 📚 Referencias

1. Akl, S. G. (1997). *Parallel Computation: Models and Methods*
2. Quinn, M. J. (2004). *Parallel Programming in C with MPI and OpenMP*
3. OpenMP Architecture Review Board. *OpenMP API Version 4.5*
