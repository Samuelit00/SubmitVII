# Script para ejecutar todas las pruebas y generar reporte

Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "Ejecutando Pruebas de Algoritmos" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

$timestamp = Get-Date -Format "yyyy-MM-dd_HH-mm-ss"
$reportFile = "reporte_$timestamp.txt"

# Función para ejecutar y registrar
function Run-Test {
    param (
        [string]$ProgramName,
        [string]$ExecutablePath
    )
    
    Write-Host "Ejecutando $ProgramName..." -ForegroundColor Yellow
    Write-Host "=====================================" -ForegroundColor Gray
    
    $output = & $ExecutablePath 2>&1 | Out-String
    Write-Host $output
    
    # Guardar en archivo de reporte
    Add-Content -Path $reportFile -Value "`n`n========================================`n"
    Add-Content -Path $reportFile -Value "$ProgramName - $(Get-Date)`n"
    Add-Content -Path $reportFile -Value "========================================`n"
    Add-Content -Path $reportFile -Value $output
    
    Write-Host "=====================================" -ForegroundColor Gray
    Write-Host ""
}

# Crear archivo de reporte
"REPORTE DE PRUEBAS - ALGORITMOS PARALELOS" | Out-File -FilePath $reportFile
"Generado: $(Get-Date)" | Add-Content -Path $reportFile
"========================================" | Add-Content -Path $reportFile

# Verificar que los ejecutables existen
if (Test-Path ".\shearsort.exe") {
    Run-Test "SHEARSORT (Ordenamiento Paralelo)" ".\shearsort.exe"
} else {
    Write-Host "✗ shearsort.exe no encontrado. Compila primero con .\compile.ps1" -ForegroundColor Red
}

if (Test-Path ".\binarysearch.exe") {
    Run-Test "P-BSA (Búsqueda Binaria Paralela)" ".\binarysearch.exe"
} else {
    Write-Host "✗ binarysearch.exe no encontrado. Compila primero con .\compile.ps1" -ForegroundColor Red
}

Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "Pruebas Completadas" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Reporte guardado en: $reportFile" -ForegroundColor Green
Write-Host ""
Write-Host "Información del Sistema:" -ForegroundColor Cyan
Write-Host "  CPU: $((Get-WmiObject Win32_Processor).Name)" -ForegroundColor White
Write-Host "  Núcleos: $((Get-WmiObject Win32_Processor).NumberOfCores)" -ForegroundColor White
Write-Host "  Hilos lógicos: $((Get-WmiObject Win32_Processor).NumberOfLogicalProcessors)" -ForegroundColor White
Write-Host "  RAM: $([math]::Round((Get-WmiObject Win32_ComputerSystem).TotalPhysicalMemory/1GB, 2)) GB" -ForegroundColor White
Write-Host ""
