#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Búsqueda binaria secuencial (BSA)
int sequentialBinarySearch(const vector<int>& arr, int target) {
    int lower = 0;
    int higher = arr.size() - 1;
    
    while (lower <= higher) {
        int middle = lower + (higher - lower) / 2;
        
        if (arr[middle] == target) {
            return middle;
        } else if (arr[middle] < target) {
            lower = middle + 1;
        } else {
            higher = middle - 1;
        }
    }
    
    return -1; // No encontrado
}

// Búsqueda binaria paralela (P-BSA)
int parallelBinarySearch(const vector<int>& arr, int target, int numProcessors) {
    int n = arr.size();
    int result = -1;
    bool found = false;
    
    #pragma omp parallel num_threads(numProcessors) shared(found, result)
    {
        int tid = omp_get_thread_num();
        int numThreads = omp_get_num_threads();
        
        // Dividir el arreglo en P subsecuencias
        int chunkSize = (n + numThreads - 1) / numThreads;
        int start = tid * chunkSize;
        int end = min(start + chunkSize - 1, n - 1);
        
        // Cada procesador verifica los extremos de su subsecuencia
        if (!found && start <= end) {
            // Verificar si el target está en el rango de esta subsecuencia
            if (arr[start] <= target && target <= arr[end]) {
                // Realizar búsqueda binaria en esta subsecuencia
                int local_lower = start;
                int local_higher = end;
                
                while (local_lower <= local_higher && !found) {
                    int middle = local_lower + (local_higher - local_lower) / 2;
                    
                    if (arr[middle] == target) {
                        #pragma omp critical
                        {
                            if (!found) {
                                found = true;
                                result = middle;
                            }
                        }
                        break;
                    } else if (arr[middle] < target) {
                        local_lower = middle + 1;
                    } else {
                        local_higher = middle - 1;
                    }
                }
            }
        }
    }
    
    return result;
}

// P-BSA mejorado con división logarítmica
int improvedParallelBinarySearch(const vector<int>& arr, int target, int numProcessors) {
    int lower = 0;
    int higher = arr.size() - 1;
    int result = -1;
    bool found = false;
    
    while (lower <= higher && !found) {
        int range = higher - lower + 1;
        
        // Si el rango es pequeño, hacer búsqueda secuencial
        if (range <= numProcessors * 10) {
            for (int i = lower; i <= higher; i++) {
                if (arr[i] == target) {
                    return i;
                }
            }
            return -1;
        }
        
        // Dividir el rango en P partes
        vector<int> checkPoints(numProcessors + 1);
        int step = range / numProcessors;
        
        for (int i = 0; i <= numProcessors; i++) {
            checkPoints[i] = lower + min(i * step, range);
        }
        
        // Verificar cada checkpoint en paralelo
        int foundProcessor = -1;
        
        #pragma omp parallel for num_threads(numProcessors)
        for (int p = 0; p < numProcessors; p++) {
            if (!found) {
                int start = checkPoints[p];
                int end = checkPoints[p + 1] - 1;
                
                if (start <= end && arr[start] <= target && target <= arr[end]) {
                    #pragma omp critical
                    {
                        if (!found) {
                            found = true;
                            foundProcessor = p;
                        }
                    }
                }
            }
        }
        
        if (foundProcessor == -1) {
            return -1; // No encontrado
        }
        
        // Actualizar los límites para el siguiente paso
        lower = checkPoints[foundProcessor];
        higher = checkPoints[foundProcessor + 1] - 1;
        
        // Verificar si encontramos el elemento exacto
        if (lower == higher) {
            return (arr[lower] == target) ? lower : -1;
        }
    }
    
    return result;
}

// Generar arreglo ordenado con valores aleatorios
vector<int> generateSortedArray(int size, int maxValue) {
    vector<int> arr;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, maxValue);
    
    for (int i = 0; i < size; i++) {
        arr.push_back(dis(gen));
    }
    
    sort(arr.begin(), arr.end());
    return arr;
}

// Generar arreglo NO ordenado
vector<int> generateRandomArray(int size, int maxValue) {
    vector<int> arr;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, maxValue);
    
    for (int i = 0; i < size; i++) {
        arr.push_back(dis(gen));
    }
    
    return arr;
}

int main() {
    cout << "=== P-BSA: Búsqueda Binaria Paralela ===" << endl;
    cout << "========================================\n" << endl;
    
    // Ejemplo de demostración
    cout << "1. EJEMPLO DE DEMOSTRACIÓN" << endl;
    cout << "--------------------------" << endl;
    
    vector<int> demo = {4, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 58, 65, 80, 90};
    int target = 80;
    
    cout << "Arreglo: ";
    for (int val : demo) cout << val << " ";
    cout << "\nBuscando: " << target << endl;
    
    int pos_seq = sequentialBinarySearch(demo, target);
    int pos_par = parallelBinarySearch(demo, target, 4);
    
    cout << "Posición (Secuencial): " << pos_seq << endl;
    cout << "Posición (Paralelo): " << pos_par << endl;
    cout << "Verificación: " << (pos_seq == pos_par ? "✓ CORRECTA" : "✗ INCORRECTA") << endl;
    
    // Pruebas de rendimiento con valores ORDENADOS
    cout << "\n2. PRUEBAS DE RENDIMIENTO (Valores ORDENADOS)" << endl;
    cout << "----------------------------------------------" << endl;
    
    vector<int> sizes = {100000, 500000, 1000000, 5000000, 10000000};
    int numThreads[] = {1, 2, 4, 8};
    int numSearches = 1000;
    
    for (int size : sizes) {
        cout << "\n--- Tamaño del arreglo: " << size << " elementos ---" << endl;
        
        // Generar arreglo ordenado
        vector<int> arr = generateSortedArray(size, size * 10);
        
        // Generar valores a buscar (algunos existentes, algunos no)
        vector<int> searchValues;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, size - 1);
        
        for (int i = 0; i < numSearches; i++) {
            if (i < numSearches / 2) {
                searchValues.push_back(arr[dis(gen)]); // Valores que existen
            } else {
                searchValues.push_back(-1); // Valores que no existen
            }
        }
        
        // Búsqueda secuencial
        auto start = high_resolution_clock::now();
        int found_seq = 0;
        for (int val : searchValues) {
            if (sequentialBinarySearch(arr, val) != -1) found_seq++;
        }
        auto end = high_resolution_clock::now();
        auto duration_seq = duration_cast<microseconds>(end - start);
        
        cout << "  Secuencial: " << duration_seq.count() / 1000.0 << " ms"
             << " | Encontrados: " << found_seq << "/" << numSearches << endl;
        
        // Búsquedas paralelas
        for (int threads : numThreads) {
            start = high_resolution_clock::now();
            int found_par = 0;
            
            #pragma omp parallel for reduction(+:found_par) num_threads(threads)
            for (int i = 0; i < numSearches; i++) {
                if (parallelBinarySearch(arr, searchValues[i], 2) != -1) {
                    found_par++;
                }
            }
            
            end = high_resolution_clock::now();
            auto duration_par = duration_cast<microseconds>(end - start);
            double speedup = static_cast<double>(duration_seq.count()) / duration_par.count();
            
            cout << "  Paralelo (" << threads << " hilos): "
                 << duration_par.count() / 1000.0 << " ms"
                 << " | Speedup: " << fixed << setprecision(2) << speedup << "x"
                 << " | Encontrados: " << found_par << "/" << numSearches << endl;
        }
    }
    
    // Pruebas con valores NO ORDENADOS
    cout << "\n3. PRUEBAS CON VALORES NO ORDENADOS" << endl;
    cout << "------------------------------------" << endl;
    
    for (int size : {10000, 50000, 100000}) {
        cout << "\nTamaño: " << size << " elementos" << endl;
        
        // Generar arreglo NO ordenado
        vector<int> arr_unordered = generateRandomArray(size, size * 10);
        
        // Intentar búsqueda (debería fallar o dar resultados incorrectos)
        int target_val = arr_unordered[size / 2];
        
        cout << "  Arreglo NO ordenado - La búsqueda binaria NO funcionará correctamente" << endl;
        cout << "  (La búsqueda binaria requiere datos ordenados)" << endl;
        
        // Ordenar y buscar
        sort(arr_unordered.begin(), arr_unordered.end());
        
        auto start = high_resolution_clock::now();
        int pos = sequentialBinarySearch(arr_unordered, target_val);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        cout << "  Después de ordenar: Encontrado en posición " << pos 
             << " | Tiempo: " << duration.count() << " μs" << endl;
    }
    
    // Análisis de complejidad
    cout << "\n4. ANÁLISIS ASINTÓTICO" << endl;
    cout << "----------------------" << endl;
    cout << "Búsqueda Binaria Secuencial:" << endl;
    cout << "  - Complejidad temporal: O(log n)" << endl;
    cout << "  - Complejidad espacial: O(1)" << endl;
    cout << "\nBúsqueda Binaria Paralela (P-BSA):" << endl;
    cout << "  - Complejidad temporal: O(log n / P) donde P = número de procesadores" << endl;
    cout << "  - Complejidad espacial: O(P)" << endl;
    cout << "  - Speedup teórico máximo: P (limitado por Ley de Amdahl)" << endl;
    cout << "  - Eficiencia: 1/log(n) * (log(n) / P) = 1/P" << endl;
    
    cout << "\n=== ANÁLISIS COMPLETADO ===" << endl;
    
    return 0;
}
