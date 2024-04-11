#include <iostream>
#include <vector>
#include "omp.h"

using namespace std;

#define epsilon 1e-6
#define matrix_size 1500

vector<double> GaussSolve(vector<vector<double>>& matrix)
{
    int size = matrix.size();
    vector<double> solution(size);

    if (size == 0) {
        cout << "No matrix!\n";
        return solution; // Матрицы нет
    }

    int colCount = matrix[0].size();
    for (int i = 0; i < size - 1; i++) { // Прямой ход
#pragma omp parallel for
        for (int j = i + 1; j < size; j++) {
            double coef = matrix[j][i] / matrix[i][i];
            for (int k = i; k < colCount; k++) {
                matrix[j][k] -= matrix[i][k] * coef;
            }
        }
    }

    for (int i = size - 1; i >= 0; i--) { // Обратный ход 
        if (abs(matrix[i][i]) < epsilon) {
            return solution; // Решения нет
        }

        for (int j = i + 1; j < size; j++) {
            matrix[i][size] -= matrix[i][j] * solution[j];
        }
        solution[i] = matrix[i][size] / matrix[i][i];
    }
    return solution;
}

int main()
{
    srand(time(NULL));

    vector<vector<double>> matrix(matrix_size);
    for (int i = 0; i < matrix_size; i++) {
        matrix[i].resize(matrix_size + 1);
        for (int j = 0; j < matrix_size; j++) {
            matrix[i][j] = rand() % 100 + 1;
        }
        matrix[i][matrix_size] = rand() % matrix_size + 1;
    }

    clock_t start, end;
    start = clock();
    vector<double> solution = GaussSolve(matrix);
    end = clock();

    cout << "Work time: " << double(end - start) / double(CLOCKS_PER_SEC) << " sec\n";
}