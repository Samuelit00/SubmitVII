# Script de compilación para Windows PowerShell
# Compila los programas de ordenamiento y búsqueda paralela

Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "Compilando Algoritmos Paralelos" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

# Verificar si existe g++
$gppExists = Get-Command g++ -ErrorAction SilentlyContinue

if ($gppExists) {
    Write-Host "Usando compilador: g++" -ForegroundColor Green
    Write-Host ""
    
    # Compilar Shearsort
    Write-Host "Compilando shearsort.cpp..." -ForegroundColor Yellow
    g++ -std=c++14 -fopenmp -O3 shearsort.cpp -o shearsort.exe
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ shearsort.exe compilado exitosamente" -ForegroundColor Green
    } else {
        Write-Host "✗ Error al compilar shearsort.cpp" -ForegroundColor Red
    }
    
    Write-Host ""
    
    # Compilar Binary Search
    Write-Host "Compilando binarysearch.cpp..." -ForegroundColor Yellow
    g++ -std=c++14 -fopenmp -O3 binarysearch.cpp -o binarysearch.exe
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ binarysearch.exe compilado exitosamente" -ForegroundColor Green
    } else {
        Write-Host "✗ Error al compilar binarysearch.cpp" -ForegroundColor Red
    }
    
} else {
    # Intentar con cl (MSVC)
    $clExists = Get-Command cl -ErrorAction SilentlyContinue
    
    if ($clExists) {
        Write-Host "Usando compilador: MSVC (cl)" -ForegroundColor Green
        Write-Host ""
        
        # Compilar Shearsort
        Write-Host "Compilando shearsort.cpp..." -ForegroundColor Yellow
        cl /EHsc /openmp /O2 /std:c++14 shearsort.cpp /Fe:shearsort.exe
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✓ shearsort.exe compilado exitosamente" -ForegroundColor Green
        } else {
            Write-Host "✗ Error al compilar shearsort.cpp" -ForegroundColor Red
        }
        
        Write-Host ""
        
        # Compilar Binary Search
        Write-Host "Compilando binarysearch.cpp..." -ForegroundColor Yellow
        cl /EHsc /openmp /O2 /std:c++14 binarysearch.cpp /Fe:binarysearch.exe
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✓ binarysearch.exe compilado exitosamente" -ForegroundColor Green
        } else {
            Write-Host "✗ Error al compilar binarysearch.cpp" -ForegroundColor Red
        }
        
    } else {
        Write-Host "✗ No se encontró ningún compilador C++" -ForegroundColor Red
        Write-Host "Por favor instala:" -ForegroundColor Yellow
        Write-Host "  - MinGW (g++): https://www.mingw-w64.org/" -ForegroundColor Yellow
        Write-Host "  - O Visual Studio con herramientas C++" -ForegroundColor Yellow
        exit 1
    }
}

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "Compilación Completada" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Para ejecutar los programas:" -ForegroundColor Cyan
Write-Host "  .\shearsort.exe" -ForegroundColor White
Write-Host "  .\binarysearch.exe" -ForegroundColor White
Write-Host ""
