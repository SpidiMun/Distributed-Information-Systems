#include <iostream>
#include <iomanip>

#define size 1500

double Calc1(int** matrix1, int** matrix2) // +
{
    int** matrix = new int* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
        }
    }

    clock_t start, end;
    start = clock();

    for (int i = 0; i < size; i++) { // строки
        for (int j = 0; j < size; j++) { // столбцы
            for (int k = 0; k < size; k++) { // счёт
                matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    end = clock();

    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return double(end - start) / double(CLOCKS_PER_SEC);;
}

double Calc2(int** matrix1, int** matrix2) // +
{
    int** matrix = new int* [size];
    for (int i = 0; i < size; i++) { 
        matrix[i] = new int[size];
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
        }
    }

    clock_t start, end;
    start = clock();

    for (int i = 0; i < size; i++) { // строки
        for (int k = 0; k < size; k++) { // счёт
            for (int j = 0; j < size; j++) { // столбцы
                matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    end = clock();

    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return double(end - start) / double(CLOCKS_PER_SEC);;
}

double Calc3(int** matrix1, int** matrix2) // +
{
    int** matrix = new int* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
        }
    }

    clock_t start, end;
    start = clock();

    for (int j = 0; j < size; j++) { // столбцы
        for (int i = 0; i < size; i++) { // строки
            for (int k = 0; k < size; k++) { // счёт
                matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    end = clock();

    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return double(end - start) / double(CLOCKS_PER_SEC);
}

double Calc4(int** matrix1, int** matrix2)
{
    int** matrix = new int* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
        }
    }

    clock_t start, end;
    start = clock();

    for (int j = 0; j < size; j++) { // столбцы
        for (int k = 0; k < size; k++) { // счёт
            for (int i = 0; i < size; i++) { // строки
                matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    end = clock();

    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return double(end - start) / double(CLOCKS_PER_SEC);;
}

double Calc5(int** matrix1, int** matrix2)
{
    int** matrix = new int* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
        }
    }

    clock_t start, end;
    start = clock();


    for (int k = 0; k < size; k++) { // счёт
        for (int i = 0; i < size; i++) { // строки
            for (int j = 0; j < size; j++) { // столбцы
                matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    end = clock();

    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return double(end - start) / double(CLOCKS_PER_SEC);;
}

double Calc6(int** matrix1, int** matrix2)
{
    int** matrix = new int* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
        }
    }

    clock_t start, end;
    start = clock();

    for (int k = 0; k < size; k++) { // счёт
        for (int j = 0; j < size; j++) { // столбцы
            for (int i = 0; i < size; i++) { // строки
                matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    end = clock();

    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return double(end - start) / double(CLOCKS_PER_SEC);;
}

int main()
{
    system("chcp 1251>nul");
    srand(time(NULL));

    int** matrix1 = new int* [size],
       ** matrix2 = new int* [size];
    for (int i = 0; i < size; i++) {
        matrix1[i] = new int[size];
        matrix2[i] = new int[size];
        for (int j = 0; j < size; j++) {
            matrix1[i][j] = rand();
            matrix2[i][j] = rand();
        }
    }

    std::cout << "Время работы Calc1: " << std::fixed
        << Calc1(matrix1, matrix2) << std::setprecision(5);
    std::cout << " сек" << std::endl;

    std::cout << "Время работы Calc2: " << std::fixed
        << Calc2(matrix1, matrix2) << std::setprecision(5);
    std::cout << " сек" << std::endl;

    std::cout << "Время работы Calc3: " << std::fixed
        << Calc3(matrix1, matrix2) << std::setprecision(5);
    std::cout << " сек" << std::endl;



    for (int i = 0; i < size; i++) {
        delete[] matrix1[i];
        delete[] matrix2[i];
    }
    delete[] matrix1;
    delete[] matrix2;
    
    //std::cout << "Время работы Calc4: " << std::fixed
    //    << Calc4(matrix1, matrix2) << std::setprecision(5);
    //std::cout << " сек" << std::endl;

    //std::cout << "Время работы Calc5: " << std::fixed
    //    << Calc5(matrix1, matrix2) << std::setprecision(5);
    //std::cout << " сек" << std::endl;

    //std::cout << "Время работы Calc6:" << std::fixed
    //    << Calc6(matrix1, matrix2) << std::setprecision(5);
    //std::cout << " сек" << std::endl;
}