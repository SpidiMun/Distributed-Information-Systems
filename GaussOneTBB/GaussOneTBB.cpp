#include <iostream>
#include <vector>
#include <tbb/tbb.h>
#include "omp.h"

using namespace std;

#define epsilon 1e-6
#define matrix_size 1000

vector<double> GaussSolve(vector<double>& matrix)
{
    vector<double> solution(matrix_size);

    if (matrix_size == 0) {
        cout << "No matrix!\n";
        return solution; // Матрицы нет
    }

    for (int i = 0; i < matrix_size - 1; i++) { // Прямой ход
        tbb::parallel_for(tbb::blocked_range<int>(i + 1, matrix_size), [&](const tbb::blocked_range<int>& range) {
            for (int j = range.begin(); j < range.end(); j++) {
                double coef = matrix[matrix_size * j + i] / matrix[matrix_size * i + i];
                for (int k = i; k < matrix_size + 1; k++) {
                    matrix[matrix_size * j + k] -= matrix[matrix_size * i + k] * coef;
                }
            }
        });
    }

    for (int i = matrix_size - 1; i >= 0; i--) { // Обратный ход 
        if (abs(matrix[matrix_size * i + i]) < epsilon) {
            return solution; // Решения нет
        }

        for (int j = i + 1; j < matrix_size; j++) {
            matrix[matrix_size * i + matrix_size] -= matrix[matrix_size * i + j] * solution[j];
        }
        solution[i] = matrix[matrix_size * i + matrix_size] / matrix[matrix_size * i + i];
    }
    return solution;
}

int main()
{
    srand(time(NULL));

    vector<double> matrix(matrix_size * (matrix_size + 1));
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            matrix[matrix_size * i + j] = rand() % 100 + 1;
        }
        matrix[matrix_size * i + matrix_size] = rand() % matrix_size + 1;
    }

    clock_t start, end;
    start = clock();
    vector<double> solution = GaussSolve(matrix);
    end = clock();

    cout << "Work time: " << double(end - start) / double(CLOCKS_PER_SEC) << " sec\n";
}