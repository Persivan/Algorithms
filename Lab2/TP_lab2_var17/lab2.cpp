#include <iostream>
#include <fstream>

using namespace std;

struct Matrix
{
   // конструкторы
    Matrix(int** a, int h, int u, int l)
    {
        arr = a;
        height = h;
        depth = u + l + 1;
        upperWidth = u;
        lowerWidth = l;
    }

    Matrix()
    {
        arr = nullptr;
        height = NULL;
        depth = NULL;
        upperWidth = NULL;
        lowerWidth = NULL;
    };

    Matrix(int** a, int h)
    {
        arr = a;
        height = h;
        depth = NULL;
        upperWidth = NULL;
        lowerWidth = NULL;
    };

    int** arr;      // двумерный массив для хранения значений
    int height;     // высота матрицы
    int depth;      // глубина
    int upperWidth; // верхняя ширина
    int lowerWidth; // нижняя ширина
};

struct CCSMatrix
{
    CCSMatrix(int* a, int* i, int* j, unsigned int c, unsigned int cLJ)
    {
        A = a;
        LI = i;
        LJ = j;
        count = c;
        countLJ = cLJ;
    }

    int* A;                // массив значений
    int* LI;               // массив номеров строк
    int* LJ;               // массив номеров первого 
    // ненулевого элемента в столбце
    unsigned int count;	    // количество элементов
    unsigned int countLJ;	// количество элементов в LJ
};

void coutPrint(Matrix* matrix, bool isDecompilated)
{
    unsigned int number = isDecompilated ? matrix->height : matrix->depth;
    for (int i = 0; i < matrix->height; i++)
    {
        for (int j = 0; j < number; j++)
        {
            cout << matrix->arr[i][j] << " ";
        }
        cout << endl;
    }
}

void CCSPrint(CCSMatrix* matrix)
{
    cout << "A: ";
    for (unsigned int i = 0; i < matrix->count; i++)
    {
        cout << matrix->A[i] << " ";
    }
    cout << endl;

    cout << "LI: ";
    for (unsigned int i = 0; i < matrix->count; i++)
    {
        cout << matrix->LI[i] << " ";
    }
    cout << endl;

    cout << "LJ: ";
    for (unsigned int i = 0; i < matrix->countLJ; i++)
    {
        cout << matrix->LJ[i] << " ";
    }
    cout << endl;
}

Matrix* compressLentMatrix(int** arr, unsigned int height, unsigned int loWidth, unsigned int upWidth)
{
    // создаем матрицу
    Matrix* matrix = new Matrix;
    // заполняем поля
    matrix->depth = upWidth + loWidth + 1;
    matrix->upperWidth = upWidth;
    matrix->lowerWidth = loWidth;
    matrix->height = height;

    int M = matrix->depth;
    // инициализируем массив матрицы
    matrix->arr = new int* [height];

    for (int i = 0; i < height; i++)
    {
        matrix->arr[i] = new int[M];
        for (int j = 0; j < M; j++)
        {
            matrix->arr[i][j] = 0;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // если arr[i][j] находится в пределах 
            // упакованного массива
            if (i <= j && j - i <= upWidth || i >= j && i - j <= loWidth) {
                matrix->arr[i][j - i + loWidth] = arr[i][j];
            }
        }
    }

    return matrix;
}

int** decompressLentMatrix(Matrix* matrix) {
    // объявление предрезультирующей матрицы
    int** resFull = new int* [matrix->height];

    for (unsigned int i = 0; i < matrix->height; i++)
    {
        resFull[i] = new int[2 * matrix->height] {0};
    }

    for (unsigned int i = 0; i < matrix->height; i++)
    {
        for (unsigned int j = 0; j < matrix->depth; j++)
        {
            resFull[i][j + i] = matrix->arr[i][j];
        }
    }

    bool isZeros = true;
    int countOfZerosFieldsLeft = -1, countOfZerosFieldsRight = -1;

    while (isZeros)
    {
        for (unsigned int i = 0; i < matrix->height; i++)
        {
            if (resFull[i][countOfZerosFieldsLeft + 1] != 0) isZeros = false;
        }
        countOfZerosFieldsLeft++;
    }

    isZeros = true;

    while (isZeros)
    {
        for (unsigned int i = matrix->height - 1; i > 0; i--)
        {
            if (resFull[i][2 * matrix->height - (countOfZerosFieldsRight + 1)] != 0) isZeros = false;
        }
        countOfZerosFieldsRight++;
    }

    // объявление результирующей матрицы
    int** res = new int* [matrix->height];

    unsigned int jj = 0;

    for (unsigned int i = 0; i < matrix->height; i++)
    {
        res[i] = new int[matrix->height] { 0 };
    }

    for (unsigned int i = 0; i < matrix->height; i++)
    {
        for (unsigned int j = countOfZerosFieldsLeft; j < countOfZerosFieldsLeft + matrix->height; j++)
        {
            res[i][jj] = resFull[i][j];
            jj++;
        }
        jj = 0;
    }

    return res;
}

Matrix multyplyLentMatrix(Matrix* matrix1, Matrix* matrix2)
{
    if (matrix1->height == matrix2->height)
    {
        int** matrix1D = decompressLentMatrix(matrix1);
        int** matrix2D = decompressLentMatrix(matrix2);

        int** arr = new int* [matrix2->height]; // результирующий массив

        //умножнаем
        for (int i = 0; i < matrix2->height; i++)
        {
            arr[i] = new int[matrix1->height];
            for (int j = 0; j < matrix2->height; j++)
            {
                arr[i][j] = 0;
                for (int k = 0; k < matrix1->height; k++)
                {
                    arr[i][j] += matrix1D[i][k] * matrix2D[k][j];
                }
            }
        }

        //очищаем память
        delete[] matrix1D;
        delete[] matrix2D;

        // возвращаем матрицу
        return Matrix(arr,
            matrix2->height,
            matrix1->upperWidth > matrix2->upperWidth ? matrix1->upperWidth : matrix2->upperWidth,
            matrix1->lowerWidth > matrix2->lowerWidth ? matrix1->lowerWidth : matrix2->lowerWidth);
    }
    //если матрицы разного размера
    else
    {
        cout << "Матрицы дожны иметь одинаковый размер!" << endl;
        system("pause");
        exit(-3);
    }
}

CCSMatrix toCCSMatrix(Matrix* matrix)
{
    unsigned int wh = matrix->height;

    int* A = new int[wh];
    int* LI = new int[wh];
    int* LJ = new int[wh];
    int cA = 0, cLI = 0, cLJ = 0;

    for (unsigned int j = 0; j < wh; j++)
    {
        for (unsigned int i = 0; i < wh; i++)
        {
            if (matrix->arr[i][j] != 0)
            {
                A[cA] = matrix->arr[i][j];
                cA++;
                LI[cLI] = (i + 1);
                cLI++;
                while (cLJ <= j)
                {
                    LJ[cLJ] = cA;
                    cLJ++;
                }
            }
        }
    }

    LJ[cLJ] = (cA + 1);
    cLJ++;

    CCSMatrix res(A, LI, LJ, cA, cLJ);
    return res;
}

Matrix decompressCCSMatrix(CCSMatrix* matrix)
{

    int** res = new int* [matrix->count];
    int* columns = new int[matrix->count]; // кол-во ненулевых 
    // элементов столбца

    for (int i = 0; i < matrix->count; i++)
    {
        columns[i] = matrix->LJ[i + 1] - matrix->LJ[i] - 1;
        res[i] = new int[matrix->count] {0};
    }

    for (int i = 0, k = 0; i < matrix->count; i++)
    {
        for (int j = 0, m = 0; j < matrix->count; j++)
        {
            if (matrix->LI[k] == j + 1)
            {
                if (columns[i] >= 0 && m <= columns[i])
                {
                    // заполняем элементы
                    res[j][i] = matrix->A[k];
                    k++;
                    m++;
                }
                else
                    res[j][i] = 0;
            }
            else
                res[j][i] = 0;
        }
    }

    // очищаем память
    delete[] columns;
    columns = nullptr;

    return Matrix(res, matrix->countLJ - 1);
}

Matrix finInput(const char* filename)
{
    ifstream fin(filename);
    int wh = 0, loWidth = 0, upWidth = 0;

    if (fin.is_open())
    {
        unsigned int i = 0, j = 0;
        while (!fin.eof())
        {
            fin >> wh; // размер
            if (fin.fail())
            {
                cout << "Ошибка! Неправильная длина-ширина." << endl;
                system("pause");
                exit(-1);
            }
            if (wh <= 0)
            {
                cout << "Ошибка! Не положительная длина-ширина." << endl;
                system("pause");
                exit(-1);
            }

            fin >> loWidth; //нижняя ширина
            if (fin.fail())
            {
                cout << "Ошибка! Неверная нижняя ширина." << endl;
                system("pause");
                exit(-1);
            }
            if (loWidth > wh)
            {
                cout << "Нижняя ширина должна быть меньше, чем ширина!" << endl;
                system("pause");
                exit(-1);
            }

            fin >> upWidth; //нижняя ширина
            if (fin.fail())
            {
                cout << "Ошибка! Неверная верхняя ширина." << endl;
                system("pause");
                exit(-1);
            }
            if (upWidth > wh)
            {
                cout << "Верхняя ширина должна быть меньше, чем ширина!" << endl;
                system("pause");
                exit(-1);
            }

            if (loWidth + upWidth + 1 > wh)
            {
                cout << "Ошибка! Глубина должна быть меньше, чем размер матрицы!" << endl;
                system("pause");
                exit(-1);
            }

            //создание массива
            int** arr = new int* [wh];   //матрица
            for (unsigned int i = 0; i < wh; i++)
            {
                arr[i] = new int[wh] {0};
            }

            int value = 0;           //получаемое значение

            //считываем числа
            for (i = 0; i < wh; i++)
            {
                for (j = 0; j < wh; j++)
                {
                    fin >> value;
                    if (fin.fail())
                    {
                        cout << "Ошибка! Неправильное число!" << endl;
                        system("pause");
                        exit(-1);
                    }
                    arr[i][j] = value;
                    value = NULL;
                }
            }

            return *compressLentMatrix(arr, wh, loWidth, upWidth);
        }
    }
    else
    {
        cout << "Ошибка! Файл не найден!" << endl;
        system("pause");
        exit(-2);
    }
}

int main(int args, char* argv[])
{
    setlocale(LC_ALL, "rus");
    Matrix matrix1, matrix2;

    //если ввод с файла
    if (args == 3)
    {
        matrix1 = finInput(argv[1]);
        matrix2 = finInput(argv[2]);
    }
    //защита когда недостаточно/много файлов
    else
    {
        cout << "Ошибка! Неправильное количество параметров!" << endl;
        system("pause");
        return -2;
    }

    //вывод сжатых данных
    cout << "Первая сжатая матрица:" << endl;
    coutPrint(&matrix1, false);
    cout << endl;
    cout << "Вторая сжатая матрица:" << endl;
    coutPrint(&matrix2, false);
    cout << endl;
    system("pause");
    cout << endl;

    //объявляем матрицу для хранения результата перемножения
    Matrix matrixMultyplied;

    //перемножение и вывод
    cout << "Перемноженная матрица:" << endl;
    matrixMultyplied = multyplyLentMatrix(&matrix1, &matrix2);
    coutPrint(&matrixMultyplied, true);
    cout << endl;
    system("pause");

    //перевод в CCS и её вывод
    CCSMatrix result = toCCSMatrix(&matrixMultyplied);
    cout << "\nCCS формат:" << endl;
    CCSPrint(&result);
    cout << endl;
    system("pause");

    //декомпиляция CCS матрицы и её вывод
    cout << "\nРаспакованный CCS формат:" << endl;
    Matrix tempDecompressedCCS = decompressCCSMatrix(&result);
    coutPrint(&tempDecompressedCCS, true);
}