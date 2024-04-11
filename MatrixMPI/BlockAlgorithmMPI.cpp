#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

using namespace std;

#define sz 1500
#define row_size sz
#define column_size sz

int* Mul(int* matrix1, int* matrix2, int* result, int blockSize)
{
    for (int i = 0; i < blockSize; i++) { // строки
        for (int k = 0; k < row_size; k++) { // счёт
            for (int j = 0; j < column_size; j++) { // столбцы
                result[row_size * i + j] += matrix1[row_size * i + k] * matrix2[row_size * k + j];
            }
        }
    }
    return result;
}

int main()
{
    MPI_Init(NULL, NULL);

    int rank, // Ранг процесса
        threadCount; // Количество процессов

    MPI_Comm_size(MPI_COMM_WORLD, &threadCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = row_size * column_size,
        * matrix1 = new int[size],
        * matrix2 = new int[size],
        * matrixResult = new int[size];
    
    double start, end;

    srand(time(NULL));
    if (rank == 0) {
        for (int i = 0; i < row_size; i++) {
            for (int j = 0; j < column_size; j++) {
                matrix1[row_size * i + j] = rand() % 100 + 1;
                matrix2[row_size * i + j] = rand() % 100 + 1;
                matrixResult[row_size * i + j] = 0;
            }
        }
        start = MPI_Wtime();
    }

    const int blockSize = size / threadCount, // Общий размер блока
        rowBlock = row_size / threadCount, // Размер блока по столбцу
        blockCount = size / blockSize;

    if (threadCount != blockCount) {
        cerr << "Error: " << threadCount << " != " << blockCount << endl;
        delete[] matrix1;
        delete[] matrix2;
        delete[] matrixResult;
        MPI_Finalize();
        return -1;
    }
    MPI_Bcast(matrix2, size, MPI_INT, 0, MPI_COMM_WORLD);

    int * mpiMatrix1 = new int[blockSize],
        * mpiMatrixResult = new int[blockSize];

    for (int i = 0; i < blockSize; i++) {
        mpiMatrixResult[i] = 0;
    }

    MPI_Scatter(matrix1, blockSize, MPI_INT, mpiMatrix1, blockSize, MPI_INT, 0, MPI_COMM_WORLD);
    Mul(mpiMatrix1, matrix2, mpiMatrixResult, rowBlock);
    MPI_Gather(mpiMatrixResult, blockSize, MPI_INT, matrixResult, blockSize, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        end = MPI_Wtime();
        cout << "Work time: " << fixed << (end - start) << setprecision(5) << " sec " << endl;
    }
    MPI_Finalize();

    delete[] matrix1;
    delete[] matrix2;
    delete[] matrixResult;
    delete[] mpiMatrix1;
    delete[] mpiMatrixResult;
}
