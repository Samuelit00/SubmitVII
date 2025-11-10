#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>
#include <iomanip>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Función para imprimir la matriz
void printMatrix(const vector<vector<int>>& M, const string& title) {
    cout << "\n" << title << ":\n";
    for (const auto& row : M) {
        for (int val : row) {
            cout << setw(4) << val << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

// Ordena la fila tid-ésima de M en orden ascendente si ascending es true
void sortRow(vector<vector<int>>& M, int row, bool ascending) {
    if (ascending) {
        sort(M[row].begin(), M[row].end());
    } else {
        sort(M[row].begin(), M[row].end(), greater<int>());
    }
}

// Ordena la columna column-ésima de M en orden ascendente
void sortColumn(vector<vector<int>>& M, int column) {
    int n = M.size();
    vector<int> temp(n);
    
    // Extraer columna
    for (int i = 0; i < n; i++) {
        temp[i] = M[i][column];
    }
    
    // Ordenar
    sort(temp.begin(), temp.end());
    
    // Colocar de vuelta
    for (int i = 0; i < n; i++) {
        M[i][column] = temp[i];
    }
}

// Shearsort paralelo - Ordena M en orden snake-like
void shearsort(int n, vector<vector<int>>& M) {
    int iterations = static_cast<int>(log2(n)) + 1;
    
    for (int iter = 0; iter < iterations; iter++) {
        // Ordenar todas las filas en paralelo
        #pragma omp parallel for
        for (int tid = 0; tid < n; tid++) {
            sortRow(M, tid, tid % 2 == 0);
        }
        
        // Ordenar todas las columnas en paralelo
        #pragma omp parallel for
        for (int tid = 0; tid < n; tid++) {
            sortColumn(M, tid);
        }
    }
}

// Versión alternativa de shearsort usando solo sort-row
void alternativeShearsort(int n, vector<vector<int>>& M) {
    int iterations = static_cast<int>(log2(n)) + 1;
    
    for (int iter = 0; iter < iterations; iter++) {
        // Ordenar todas las filas en paralelo
        #pragma omp parallel for
        for (int tid = 0; tid < n; tid++) {
            sortRow(M, tid, tid % 2 == 0);
        }
        
        // Transponer la matriz
        vector<vector<int>> temp(n, vector<int>(n));
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                temp[j][i] = M[i][j];
            }
        }
        M = temp;
        
        // Ordenar todas las "filas" (que son columnas originales) en paralelo
        #pragma omp parallel for
        for (int tid = 0; tid < n; tid++) {
            sortRow(M, tid, true);
        }
        
        // Transponer de vuelta
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                temp[j][i] = M[i][j];
            }
        }
        M = temp;
    }
}

// Shearsort secuencial para comparación
void sequentialShearsort(int n, vector<vector<int>>& M) {
    int iterations = static_cast<int>(log2(n)) + 1;
    
    for (int iter = 0; iter < iterations; iter++) {
        // Ordenar todas las filas secuencialmente
        for (int tid = 0; tid < n; tid++) {
            sortRow(M, tid, tid % 2 == 0);
        }
        
        // Ordenar todas las columnas secuencialmente
        for (int tid = 0; tid < n; tid++) {
            sortColumn(M, tid);
        }
    }
}

// Generar matriz con valores aleatorios
vector<vector<int>> generateRandomMatrix(int n, int maxValue) {
    vector<vector<int>> M(n, vector<int>(n));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, maxValue);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            M[i][j] = dis(gen);
        }
    }
    
    return M;
}

// Verificar si la matriz está ordenada en snake-like order
bool isSnakeSorted(const vector<vector<int>>& M) {
    int n = M.size();
    vector<int> linear;
    
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            for (int j = 0; j < n; j++) {
                linear.push_back(M[i][j]);
            }
        } else {
            for (int j = n - 1; j >= 0; j--) {
                linear.push_back(M[i][j]);
            }
        }
    }
    
    for (size_t i = 1; i < linear.size(); i++) {
        if (linear[i] < linear[i-1]) {
            return false;
        }
    }
    return true;
}

int main() {
    cout << "=== SHEARSORT: Algoritmo de Ordenamiento Paralelo ===" << endl;
    cout << "===================================================\n" << endl;
    
    // Ejemplo pequeño para demostración
    cout << "1. EJEMPLO DE DEMOSTRACIÓN (Matriz 3x3)" << endl;
    cout << "----------------------------------------" << endl;
    vector<vector<int>> demo = {
        {5, 8, 2},
        {9, 1, 7},
        {3, 6, 4}
    };
    
    printMatrix(demo, "Matriz Original");
    shearsort(3, demo);
    printMatrix(demo, "Matriz Ordenada (Snake-like order)");
    cout << "Verificación: " << (isSnakeSorted(demo) ? "✓ CORRECTA" : "✗ INCORRECTA") << endl;
    
    // Pruebas de rendimiento
    vector<int> sizes = {8, 16, 32, 64, 128};
    int numThreads[] = {1, 2, 4, 8};
    
    cout << "\n2. PRUEBAS DE RENDIMIENTO" << endl;
    cout << "-------------------------" << endl;
    
    for (int size : sizes) {
        cout << "\n--- Tamaño de Matriz: " << size << "x" << size << " ---" << endl;
        
        // Generar matriz de prueba
        vector<vector<int>> original = generateRandomMatrix(size, 1000);
        
        // Prueba secuencial
        auto M_seq = original;
        auto start = high_resolution_clock::now();
        sequentialShearsort(size, M_seq);
        auto end = high_resolution_clock::now();
        auto duration_seq = duration_cast<microseconds>(end - start);
        
        cout << "  Secuencial: " << duration_seq.count() / 1000.0 << " ms";
        cout << " | Verificación: " << (isSnakeSorted(M_seq) ? "✓" : "✗") << endl;
        
        // Pruebas paralelas con diferentes números de hilos
        for (int threads : numThreads) {
            omp_set_num_threads(threads);
            auto M_par = original;
            
            start = high_resolution_clock::now();
            shearsort(size, M_par);
            end = high_resolution_clock::now();
            auto duration_par = duration_cast<microseconds>(end - start);
            
            double speedup = static_cast<double>(duration_seq.count()) / duration_par.count();
            
            cout << "  Paralelo (" << threads << " hilos): " 
                 << duration_par.count() / 1000.0 << " ms"
                 << " | Speedup: " << fixed << setprecision(2) << speedup << "x"
                 << " | Verificación: " << (isSnakeSorted(M_par) ? "✓" : "✗") << endl;
        }
    }
    
    // Comparación con versión alternativa
    cout << "\n3. COMPARACIÓN: SHEARSORT vs ALTERNATIVE-SHEARSORT" << endl;
    cout << "---------------------------------------------------" << endl;
    
    for (int size : {16, 32, 64}) {
        cout << "\nTamaño: " << size << "x" << size << endl;
        vector<vector<int>> original = generateRandomMatrix(size, 1000);
        
        omp_set_num_threads(4);
        
        // Shearsort normal
        auto M1 = original;
        auto start1 = high_resolution_clock::now();
        shearsort(size, M1);
        auto end1 = high_resolution_clock::now();
        auto dur1 = duration_cast<microseconds>(end1 - start1);
        
        // Alternative shearsort
        auto M2 = original;
        auto start2 = high_resolution_clock::now();
        alternativeShearsort(size, M2);
        auto end2 = high_resolution_clock::now();
        auto dur2 = duration_cast<microseconds>(end2 - start2);
        
        cout << "  Shearsort (sort-row + sort-column): " << dur1.count() / 1000.0 << " ms" << endl;
        cout << "  Alternative (solo sort-row + transpose): " << dur2.count() / 1000.0 << " ms" << endl;
        cout << "  Diferencia: " << abs(dur1.count() - dur2.count()) / 1000.0 << " ms" << endl;
    }
    
    cout << "\n=== ANÁLISIS COMPLETADO ===" << endl;
    
    return 0;
}
