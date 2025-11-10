#!/bin/bash
# Script para ejecutar todas las pruebas en Linux/Docker

echo "========================================="
echo "Ejecutando Pruebas de Algoritmos"
echo "========================================="
echo ""

timestamp=$(date +"%Y-%m-%d_%H-%M-%S")
reportFile="reporte_$timestamp.txt"

# Función para ejecutar y registrar
run_test() {
    local program_name=$1
    local executable=$2
    
    echo "Ejecutando $program_name..."
    echo "========================================="
    
    ./$executable | tee -a "$reportFile"
    
    echo "========================================="
    echo ""
}

# Crear archivo de reporte
echo "REPORTE DE PRUEBAS - ALGORITMOS PARALELOS" > "$reportFile"
echo "Generado: $(date)" >> "$reportFile"
echo "========================================" >> "$reportFile"
echo "" >> "$reportFile"

# Verificar que los ejecutables existen
if [ -f "./shearsort.exe" ]; then
    run_test "SHEARSORT (Ordenamiento Paralelo)" "shearsort.exe"
else
    echo "✗ shearsort.exe no encontrado. Compila primero."
fi

if [ -f "./binarysearch.exe" ]; then
    run_test "P-BSA (Búsqueda Binaria Paralela)" "binarysearch.exe"
else
    echo "✗ binarysearch.exe no encontrado. Compila primero."
fi

echo "========================================="
echo "Pruebas Completadas"
echo "========================================="
echo ""
echo "Reporte guardado en: $reportFile"
echo ""
echo "Información del Sistema:"
echo "  CPU: $(lscpu | grep 'Model name' | cut -d ':' -f 2 | xargs)"
echo "  Núcleos: $(nproc)"
echo "  Arquitectura: $(uname -m)"
echo "  Kernel: $(uname -r)"
echo ""
