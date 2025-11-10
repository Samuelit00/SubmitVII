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

### Scripts
- `compile.sh` / `compile.ps1` - Compilar los programas
- `run_tests.sh` / `run_tests.ps1` - Ejecutar pruebas y generar reporte

### Documentación
- `README.md` - Este archivo
- `informe.tex` - Informe completo en LaTeX
- `CMakeLists.txt` - Configuración para CMake (opcional)

## � Compilación

### Linux/Docker:
```bash
bash compile.sh
# o manualmente:
g++ -std=c++14 -fopenmp -O3 shearsort.cpp -o shearsort.exe
g++ -std=c++14 -fopenmp -O3 binarysearch.cpp -o binarysearch.exe
```

### Windows:
```powershell
.\compile.ps1
# o manualmente:
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

## � Informe

El archivo `informe.tex` contiene el análisis completo:
- Implementación de algoritmos
- Resultados experimentales
- Análisis asintótico
- Comparación teoría vs práctica
- Conclusiones

Para compilar el informe:
```bash
pdflatex informe.tex
pdflatex informe.tex  # Dos veces para referencias
```

O usar [Overleaf](https://www.overleaf.com) (online, sin instalación).

## ⚙️ Requisitos

- **Compilador**: g++ con soporte C++14
- **OpenMP**: Para paralelización
- **Sistema**: Linux, Windows o macOS

## 📚 Referencias

1. Akl, S. G. (1997). *Parallel Computation: Models and Methods*
2. Quinn, M. J. (2004). *Parallel Programming in C with MPI and OpenMP*
3. OpenMP Architecture Review Board. *OpenMP API Version 4.5*
