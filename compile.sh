#!/bin/bash
# Script de compilación para Linux/Docker
# Compila los programas de ordenamiento y búsqueda paralela

echo "====================================="
echo "Compilando Algoritmos Paralelos"
echo "====================================="
echo ""

# Verificar si existe g++
if command -v g++ &> /dev/null; then
    echo "Usando compilador: g++"
    echo ""
    
    # Compilar Shearsort
    echo "Compilando shearsort.cpp..."
    g++ -std=c++14 -fopenmp -O3 shearsort.cpp -o shearsort.exe
    
    if [ $? -eq 0 ]; then
        echo "✓ shearsort.exe compilado exitosamente"
    else
        echo "✗ Error al compilar shearsort.cpp"
    fi
    
    echo ""
    
    # Compilar Binary Search
    echo "Compilando binarysearch.cpp..."
    g++ -std=c++14 -fopenmp -O3 binarysearch.cpp -o binarysearch.exe
    
    if [ $? -eq 0 ]; then
        echo "✓ binarysearch.exe compilado exitosamente"
    else
        echo "✗ Error al compilar binarysearch.cpp"
    fi
    
else
    echo "✗ No se encontró g++"
    echo "Por favor instala: sudo apt-get install g++"
    exit 1
fi

echo ""
echo "====================================="
echo "Compilación Completada"
echo "====================================="
echo ""
echo "Para ejecutar los programas:"
echo "  ./shearsort.exe"
echo "  ./binarysearch.exe"
echo ""
