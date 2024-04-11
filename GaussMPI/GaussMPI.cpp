#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <mpi.h>

using namespace std;

#define epsilon 1e-6
#define matrix_size 1000

int main()
{
    MPI_Init(NULL, NULL);

    int rank, // Ранг процесса
        threadCount; // Количество процессов

    MPI_Comm_size(MPI_COMM_WORLD, &threadCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<double> matrix(matrix_size * (matrix_size + 1)),
        solution(matrix_size);

    int rowProc = threadCount != 1 ? matrix_size / (threadCount - 1) : matrix_size;
    double elemCount = threadCount != 1 ? (float)matrix_size / (threadCount - 1) : matrix_size;

    if (rowProc != elemCount)
    {
        cerr << "Error: " << rowProc << " != " << elemCount << endl;
        return -1;
    }

    if (rank == 0)
    {
        double start, end;
        start = MPI_Wtime();
        vector<double> mpiMatrix(rowProc * (matrix_size + 1));
        int index = 0;
        for (int thread = 1; thread < threadCount; thread++)
        {
            for (int i = 0; i < rowProc * (matrix_size + 1); i++)
            {
                mpiMatrix[i] = matrix[index];
                index++;
            }
            MPI_Send(&mpiMatrix.front(), rowProc * (matrix_size + 1), MPI_DOUBLE, thread, 0, MPI_COMM_WORLD);
        }
        index = 0;
        for (int thread = 1; thread < threadCount; thread++)
        {
            MPI_Recv(&mpiMatrix.front(), rowProc * (matrix_size + 1), MPI_DOUBLE, thread, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < rowProc * (matrix_size + 1); i++)
            {
                matrix[index] = mpiMatrix[i];
                index++;
            }
        }
        for (int i = matrix_size - 1; i >= 0; i--) {
            if (abs(matrix[matrix_size * i + i]) < epsilon) {
                break; // Решения нет
            }

            for (int j = i + 1; j < matrix_size; j++) {
                matrix[matrix_size* i + matrix_size] -= matrix[matrix_size * i + j] * solution[j];
            }
            solution[i] = matrix[matrix_size * i + matrix_size] / matrix[matrix_size * i + i];
        }
        end = MPI_Wtime();
        cout << "Work time: " << fixed << (end - start) << setprecision(5) << " sec " << endl;
    }
    else
    {
        int rowStart = rowProc * (rank - 1),
            rowEnd = rank * rowProc - 1;
        vector<double> mpiMatrix(rowProc * (matrix_size + 1)),
            mpiCoef(rowProc * (matrix_size + 1));

        MPI_Recv(&mpiMatrix.front(), rowProc * (matrix_size + 1), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        for (int thread = 1; thread < rank; thread++) {
            MPI_Recv(&mpiCoef.front(), rowProc * (matrix_size + 1), MPI_DOUBLE, thread, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            for (int i = 0; i < rowProc; i++) {
                for (int j = 0; j < rowProc; j++) {
                    double coef = mpiCoef[matrix_size * i + j + rowProc * (thread - 1)];
                    for (int k = matrix_size + 1; k >= j; k--) {
                        mpiMatrix[matrix_size * i + k] -= coef * mpiCoef[matrix_size * j + k];
                    }
                }
            }
        }

        int counter = 0;
        for (int i = 0; i < rowProc; i++) {
            double coef = mpiMatrix[rowStart + (matrix_size + 1) * i + counter];
            for (int j = (matrix_size + 1); j >= rowStart + counter; j--) {
                mpiMatrix[matrix_size * i + j] /= coef;
            }
            for (int j = i + 1; j < rowProc; j++) {
                coef = mpiMatrix[rowStart + matrix_size * j + counter];
                for (int k = matrix_size + 1; k >= rowStart + counter; k--) {
                    mpiMatrix[matrix_size * j + k] -= mpiMatrix[matrix_size * i + k] * coef;
                }
            }
        }

        for (int thread = rank + 1; thread < threadCount; thread++)
        {
            MPI_Send(&mpiMatrix.front(), rowProc * (matrix_size + 1), MPI_DOUBLE, thread, 0, MPI_COMM_WORLD);
        }
        MPI_Send(&mpiMatrix.front(), rowProc * (matrix_size + 1), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}