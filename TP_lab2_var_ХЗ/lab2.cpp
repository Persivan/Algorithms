#include <iostream>
#include <fstream>

using namespace std;

//Получить число из консоли
int cinGetInt(bool isCannotBeZero, bool isMustBePositive, const char* question)
{
    int value = 0;                              // переменная для записи в неё числа из консоли
    while (true)                                // спрашиваем пока не получим подходящее число
    {
        cout << question;

        cin >> value;
        if (cin.fail())                    // если ввели символ или число вне диапазона int
        {
            cout << "Wrong number!" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');     // защита от поломки cin при вводе символов
            system("pause");
            continue;
        }
        cin.ignore(INT_MAX, '\n');         // защита от поломки при вводе нескольких чисел

        if (isCannotBeZero && value == 0)  // защита от ввода нуля если он недопустим
        {
            cout << "Value cannot be equal zero!" << endl;
            system("pause");
            continue;
        }

        if (isMustBePositive)
        {
            if (value > 0)                      // проверка на положительность числа
            {
                break;
            }
            else
            {
                cout << "Value must be positive!" << endl;
                system("pause");
                continue;
            }
        }

        break;
    }

    return value;
}

template<typename T>
struct MyVector                 // самописный вектор
{
    // конструкторы
    MyVector() {}
    MyVector(T* numbers, const unsigned int size)
    {
        this->arr = numbers;
        this->size = size;
    }

    void resize(unsigned int newSize)
    {
        T* newArr = new T[newSize]{};
        // переносим старые данные
        if (newSize < size)
        {
            size = newSize;
        }
        for (unsigned int i = 0; i < size; i++)
        {
            newArr[i] = arr[i];
        }

        // сохраняем
        arr = newArr;
        size = newSize;
    }

    // добавление нового элемента
    void push_back(T newNode)
    {
        resize(size + 1);
        arr[size - 1] = newNode;
    }

    void pop_back()
    {
        size--;
    }

    T* arr = nullptr;
    unsigned int size = 0;
};

// структура хранения CRS формата
class CRSMatrix
{
public:
    // конструкторы
    CRSMatrix() {}
    CRSMatrix(MyVector<int>* a, MyVector<int>* i, MyVector<int>* j)
    {
        A = *a;
        LI = *i;
        LJ = *j;
    }

    void print()
    {
        cout << "A: ";
        for (unsigned int i = 0; i < this->A.size; i++)
        {
            cout << this->A.arr[i] << " ";
        }
        cout << endl;

        cout << "LJ: ";
        for (unsigned int i = 0; i < this->LJ.size; i++)
        {
            cout << this->LJ.arr[i] << " ";
        }
        cout << endl;

        cout << "LI: ";
        for (unsigned int i = 0; i < this->LI.size; i++)
        {
            cout << this->LI.arr[i] << " ";
        }
        cout << endl;
    }

    MyVector<int> A;                 // массив значений
    MyVector<int> LJ;                // массив номеров строк
    MyVector<int> LI;                // массив номеров первого ненулевого элемента в строке
};

class Matrix                        // структура матрицы
{
    struct MatrixNode               // структура для хранения значений
    {                               // как мне кажется, лучше хранить массив таких структур вместо трёх массивов
        // конструкторы
        MatrixNode() {}

        MatrixNode(unsigned int x, unsigned int y, unsigned int value)
        {
            this->x = x;            // координата x
            this->y = y;            // координата y
            this->value = value;    // значение
        }

        unsigned int x = 0;
        unsigned int y = 0;
        int value = 0;
    };

public:
    // конструкторы
    Matrix() {}
    Matrix(int width, int height)
    {
        this->width = width;
        this->height = height;
    }

    // добавление элемента в массив значений
    void add(unsigned int* x, unsigned int* y, int* value)
    {
        numbers.push_back(MatrixNode(*x, *y, *value));
    }

    // ввод из консоли
    void cinInput()                         // ввод матрицы из консоли
    {
        // ширина
        width = cinGetInt(true, true, "Enter width of matrix: ");

        // высота
        height = cinGetInt(true, true, "Enter height of matrix: ");

        int value = 0;                      // переменная для временного хранения значения из консоли

        for (unsigned int i = 0; i < height; i++)
        {
            for (unsigned int j = 0; j < width; j++)
            {
                cout << "Matrix[" << i + 1 << "][" << j + 1 << "]: ";
                value = cinGetInt(false, false, "");

                if (value != 0)             // записываем только ненулевые значения
                {
                    numbers.push_back(MatrixNode(j, i, value));
                }
            }
        }
    }

    // ввод из файла
    void finInput(const char* filename)     // ввод матрицы из файла
    {
        ifstream fin(filename);

        if (fin.is_open())
        {
            // ширина
            fin >> width;
            if (fin.fail())
            {
                cout << "Error! Wrong width!" << endl;
                system("pause");
                exit(-1);
            }
            if (width <= 0)
            {
                cout << "Error! Not positive width!" << endl;
                system("pause");
                exit(-1);
            }

            // высота
            fin >> height;
            if (fin.fail())
            {
                cout << "Error! Wrong height!" << endl;
                system("pause");
                exit(-1);
            }
            if (height <= 0)
            {
                cout << "Error! Not positive height!" << endl;
                system("pause");
                exit(-1);
            }

            int value = 0;

            // читаем матрицу
            for (unsigned int i = 0; i < height; i++)
            {
                for (unsigned int j = 0; j < width; j++)
                {
                    fin >> value;
                    if (fin.fail())
                    {
                        cout << "Error! Wrong value! Open the file and fix format!" << endl;
                        system("pause");
                        exit(-1);
                    }
                    if (value != 0)             // записываем только ненулевые значения
                    {
                        numbers.push_back(MatrixNode(j, i, value));
                    }
                }
            }
        }
        else
        {
            cout << "File does not exist!" << endl;
            system("pause");
            exit(-1);
        }
    }

    // распечатать распакованную матрицу
    void printMatrix()
    {
        // создание матрицы
        int** matrix = new int*[height];
        for (unsigned int i = 0; i < height; i++)
        {
            matrix[i] = new int[width] {0};
        }

        // заносим значения
        for (unsigned int i = 0; i < numbers.size; i++)
        {
            matrix[numbers.arr[i].y][numbers.arr[i].x] = numbers.arr[i].value;
        }

        // печатаем
        for (unsigned int i = 0; i < height; i++)
        {
            for (unsigned int j = 0; j < width; j++)
            {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    // получить ширину
    unsigned int getWidth()
    {
        return width;
    }

    // получить высоту
    unsigned int getHeight()
    {
        return height;
    }

    // поиск значения по координатам
    int findByCoordinates(int x, int y)
    {
        for (unsigned int i = 0; i < numbers.size; i++)
        {
            if (numbers.arr[i].x == x && numbers.arr[i].y == y)
            {
                return numbers.arr[i].value;
            }
        }
        return 0;
    }

    // перевод в CRS формат
    CRSMatrix toCRSMatrix()
    {
        unsigned int width = this->width;
        unsigned int height = this->width;


        MyVector<int> A;                            // массив значений
        MyVector<int> LJ;                           // массив номеров столбцов
        MyVector<int> LI;                           // массив местоположений первого ненулевого элемента каждой строки
        int value = 0;
        unsigned int counter = 0, temp = 0;
        int lastRecordedI = 0;

        for (unsigned int i = 0; i < height; i++)
        {
            for (unsigned int j = 0; j < width; j++)
            {
                value = this->findByCoordinates(j, i);
                if (value != 0)                     // если такой элемент найден
                {
                    A.push_back(value);
                    LJ.push_back(j + 1);

                    counter++;

                    while (LI.size <= i)
                    {
                        LI.push_back(counter);
                    }
                }
            }
        }

        // добавляем последний элемент N+1
        LI.push_back(counter + 1);

        return CRSMatrix(
            &A,
            &LI,
            &LJ
        );
    }

    // тестовый вывод значений в координатной матрице, можно удалять
    void test()
    {
        for (unsigned int i = 0; i < this->numbers.size; i++)
        {
            std::cout << numbers.arr[i].x << " " << numbers.arr[i].y << " " << numbers.arr[i].value << std::endl;
        }
    }

private:
    MyVector<MatrixNode> numbers;          // массив пар x-y-value
    unsigned int width = 0;                // длина матрицы
    unsigned int height = 0;               // ширина матрицы
};

Matrix multiplyMatrix(Matrix* matrix1, Matrix* matrix2)
{
    Matrix res(matrix1->getHeight(), matrix2->getWidth());

    // для перемножения матриц ширина первой должна быть равна высоте второй
    if (matrix1->getWidth() == matrix2->getHeight())
    {
        int value = 0;                     // временная переменная для произведения
        for (unsigned int j = 0; j < res.getWidth(); j++)
        {
            for (unsigned int i = 0; i < res.getHeight(); i++)
            {
                value = 0;
                for (unsigned int k = 0; k < matrix1->getWidth(); k++)
                {
                    value += matrix1->findByCoordinates(k, i) * matrix2->findByCoordinates(j, k);
                }
                if (value != 0)
                {
                    res.add(&j, &i, &value);
                }
            }
        }

        return res;
    }
    else
    {
        cout << "First matrix must have width equal height of second matrix!" << endl;
        system("pause");
        exit(-2);
    }
}

int main()
{
    Matrix matrix1, matrix2;

    int input = 0;
    bool isChooseInputType = true;

    while (isChooseInputType)
    {
        cout << "Input from:";
        cout << "\n1) Console";
        cout << "\n2) File" << endl << endl;

        input = cinGetInt(true, true, "Write command number: ");

        switch (input)
        {
        case 1:                                     // из консоли
            system("cls");
            cout << "Enter info about first matrix" << endl;
            matrix1.cinInput();
            system("cls");
            cout << "Enter info about second matrix" << endl;
            matrix2.cinInput();
            isChooseInputType = false;
            break;
        case 2:                                     // из файла
            system("cls");
            matrix1.finInput("1.txt");
            matrix2.finInput("2.txt");
            isChooseInputType = false;
            break;
        default:
            cout << "Wrong command!" << endl;
            system("pause");
            system("cls");
            break;
        }
    }

    // вывод исходной матрицы
    cout << "\nEntered first matrix:" << endl;
    matrix1.printMatrix();
    cout << "\nEntered second matrix:" << endl;
    matrix2.printMatrix();
    cout << endl;

    // перемножение матрицы и вывод результата
    Matrix multipliedMatrix = multiplyMatrix(&matrix1, &matrix2);
    cout << "\nMultiplied matrix:" << endl;
    multipliedMatrix.printMatrix();
    cout << endl;

    // перевод в CRS формат
    CRSMatrix result = multipliedMatrix.toCRSMatrix();
    cout << "In CRS format:" << endl;
    result.print();

    // тест CRS для матрицы из лекции (https://i.imgur.com/7TlxUzz.png)
    //std::cout << std::endl;
    //Matrix devTest;
    //devTest.finInput("test.txt");
    //CRSMatrix tes = devTest.toCRSMatrix();
    //tes.print();
}