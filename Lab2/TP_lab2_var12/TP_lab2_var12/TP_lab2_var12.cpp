#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Ввод из консоли с проверкой каждого числа на int
int consoleInt()
{
   int result = 0;
   string strFromConsole = "";
   while (true)
   {
      getline(cin, strFromConsole);
      try
      {
         result = stoi(strFromConsole);
         break;
      }
      catch (const exception& ex)
      {
         cout << "Error: " << ex.what() << endl;
         cout << "Enter a value again: " << endl;
      }
   }
   return result;
}

//Ввод из файла
int fileIntArray(string filename, int*& arr)
{
   unsigned int length = 0;
   int result = 0;
   ifstream fin(filename);
   if (fin.is_open())
   {
      string instr = "";
      unsigned int arrLength = 10;
      arr = new int[arrLength];
      while (getline(fin, instr))
      {
         try
         {
            result = stoi(instr);
            //Если length (количество слов) совпало с длиной массива
            if (length == arrLength)
            {
               int* tArr = new int[length];
               for (auto i = 0; i < length; i++)
               {
                  tArr[i] = arr[i];
               }
               delete[]arr;
               arrLength += 10;
               arr = new int[arrLength];
               for (auto i = 0; i < length; i++)
               {
                  arr[i] = tArr[i];
               }
               delete[]tArr;
            }
            arr[length++] = result;
         }
         catch (const std::exception& ex)
         {
            cout << "Error: " << ex.what() << endl;
         }
      }
   }
   else
   {
      cout << "File not found!" << endl;
   }
   return length;
}

//Самописный вектор
class MyVector
{
public:
   int* arr;                    //Указатель на первый элемент массива
   int length;                  //Количество элементов в массиве
   MyVector()
   {
      length = 0;
      arr = new int[0];
   }
   MyVector(int value)
   {
      length = 1;
      arr = new int[1];
      arr[0] = value;
   }
   ~MyVector()                 //Деструктор
   {
      delete[] arr;
   }

   void push(int value)     //Добавление нового элемента
   {
      //Создаём временный массив с размером
      int* tArr = new int[length];
      //Переносим из старого в новый
      for (auto i = 0; i < length; i++)
      {
         tArr[i] = arr[i];
      }
      //Удаляем старый
      delete[]arr;
      //Создаем новый
      length++;
      arr = new int[length];
      //Переписываем все значения из временного
      for (auto i = 0; i < length - 1; i++)
      {
         arr[i] = tArr[i];
      }
      //Удаляем временный
      delete[]tArr;
      //Добавляем новое значение
      arr[length - 1] = value;
   }

   void print()                  //Печать массива
   {
      if (length == 0)
      {
         cout << "empty!" << endl;
         return;
      }

      for (int i = 0; i < length - 1; i++)
      {
         cout << arr[i] << ", ";
      }
      cout << arr[length - 1] << endl;
   }

   int index(int value)
   {
      for (int i = 0; i < length; i++)
      {
         if (arr[i] == value)
         {
            return i;
         }
      }
      return -1;
   }

   //Удаляет повторяющиеся числа из конца массива (треубется для удаления хвоста в LI)
   void deleteEnd()
   {
      //Получим макс. число
      int num = 0;
      for (int i = 0; i < length; i++)
      {
         if (num < arr[i])
         {
            num = arr[i];
         }
      }
      //Получаем индекс на котором это число
      int index = 0;
      for (int i = 0; i < length; i++)
      {
         if (num == arr[i])
         {
            index = i;
            break;
         }
      }
      //Обрезаем массив до этого элемента
      //Создаём временный массив с размером
      int* tArr = new int[index + 1];
      //Переносим из старого в новый
      for (auto i = 0; i < index + 1; i++)
      {
         tArr[i] = arr[i];
      }
      //Удаляем старый
      delete[]arr;
      //Создаем новый
      length = index + 1;
      arr = new int[length];
      //Переписываем все значения из временного
      for (auto i = 0; i < length; i++)
      {
         arr[i] = tArr[i];
      }
      //Удаляем временный
      delete[]tArr;
   }
};

class SSSMatrix
{
private:
   MyVector AD;      //Главная диагональ
   MyVector AU;      //Ненулевые элементы стоящие над диагональю
   MyVector AL;      //Ненулевые элементы стоящие под диагональю
   MyVector LJ;      //Номер столбца в котором элементы стоящие над диагональю
   MyVector LI;      //Номер столбца в котором элементы стоящие под диагональю
   int widthHeight;  //Размер матрицы
public:
   void fillMatrixFromConsole()
   {
      //Узнаем размеры матрицы
      widthHeight = -1;
      cout << "Enter width / height of matrix : ";
      while (widthHeight < 0)
      {
         widthHeight = consoleInt();
         if (widthHeight < 0)
         {
            cout << "Please write correct number!" << endl;
         }
      }
      //Заполянем
      int value = 0;
      LI.push(0);
      for (int row = 0; row < widthHeight; row++)
      {
         int elemCount = 0;
         for (int column = row; column < widthHeight; column++)
         {
            cout << "Matrix[" << row << "][" << column << "]: ";
            value = consoleInt();
            //Добавляем в соотвс. вектор
            if (row == column)               //Если на диагонали
            {
               AD.push(value);
               continue;
            }
            else
            {
               //Нули от пользователя пропускаем
               if (value == 0)
               {
                  continue;
               }
               //Увеличивам счетчик
               //Сохраняем число в AU, а номер столбца в LJ
               elemCount++;
               AU.push(value);
               LJ.push(column);
               //Просим от пользователя число для симм. позиции
               cout << "Matrix[" << column << "][" << row << "]: ";
               value = consoleInt();
               while (value == 0)
               {
                  cout << "NOT 0! :" << endl;
                  value = consoleInt();
               }
               AL.push(value);
            }
         }
         if (LI.length == 0)
         {
            LI.push(elemCount);
         }
         else
         {
            LI.push(LI.arr[LI.length - 1] + elemCount);
         }
      }
      //Так как LI в for добавлял числа для каждой строки, 
      //но нам не требуется числа если они повторяются в конце, то удалим их
      LI.deleteEnd();
   }
   void fillMatrixFromFile(string fileName)
   {
      //Заполним дин. массив из файла
      int* simpleArray = nullptr;
      int size = fileIntArray(fileName, simpleArray);
      for (auto i = 0; i < size; i++)
      {
         cout << simpleArray[i] << " ";
      }
      cout << endl << "Array from file completed!" << endl;
      //Проверим полученный массив из файла
      //Пустой ли
      if (size == 0)
      {
         cout << "Does this file includes numbers?" << endl;
         return;
      }
      //Введем индекс для работы с массивом из файла
      int index = 0;
      //Узнаем размеры матрицы
      widthHeight = simpleArray[index];
      //Заполянем
      LI.push(0);
      int value = 0;
      for (int row = 0; row < widthHeight; row++)
      {
         int elemCount = 0;
         for (int column = row; column < widthHeight; column++)
         {
            if (index != size)
            {
               index++;
               value = simpleArray[index];
            }
            else
            {
               value = 0;
            }
            //Добавляем в соотвс. вектор
            if (row == column)               //Если на диагонали
            {
               AD.push(value);
               continue;
            }
            else
            {
               //Нули пропускаем
               if (value == 0)
               {
                  continue;
               }
               //Увеличивам счетчик
               //Сохраняем число в AU, а номер столбца в LJ
               elemCount++;
               AU.push(value);
               LJ.push(column);
               //Второе число
               if (index == size)
               {
                  index++;
                  value = simpleArray[index];
               }
               else
               {
                  cout << "Not enough number for a pair. Will use 1" << endl;
                  value = 1;
               }
               AL.push(value);
            }
         }
         if (LI.length == 0)
         {
            LI.push(elemCount);
         }
         else
         {
            LI.push(LI.arr[LI.length - 1] + elemCount);
         }
      }
      //Так как LI в for добавлял числа для каждой строки, 
      //но нам не требуется числа если они повторяются в конце, то удалим их
      LI.deleteEnd();
   }
   void print()
   {
      cout << "[print] Your matrix: ";
      if (widthHeight == 0)
      {
         cout << "empty!" << endl;
         return;
      }
      else
      {
         cout << endl;
      }
      cout << "AD: ";
      AD.print();
      cout << "AU: ";
      AU.print();
      cout << "AL: ";
      AL.print();
      cout << "LJ: ";
      LJ.print();
      cout << "LI: ";
      LI.print();
   }
   void printDecompressed()
   {
      cout << "[printDecompressed] Your matrix: " << endl;

      if (widthHeight == 0)
      {
         cout << "empty!" << endl;
         return;
      }

      //Не представляю как вывести на экран в сжатом формате
      //Поэтому переведу сначала в обычный
      double** a = new double* [widthHeight];
      for (int i = 0; i < widthHeight; i++)
      {
         a[i] = new double[widthHeight];
      }
      //Заполняю
      //Печать матрицы
      int currADIndex = 0;
      for (int row = 0; row < widthHeight; row++)
      {
         //Считаем сколько элементов на строке row
         int countElem = 0;
         if (row > LI.length)
         {
            countElem = 0;
         }
         else
         {
            countElem = LI.arr[row + 1] - LI.arr[row];
         }
         //Выводим строку
         for (int column = 0; column < widthHeight; column++)
         {
            //На главной диагонали
            if (row == column)
            {
               a[row][column] = AD.arr[row];
               continue;
            }
            //Значения над гл. диаг.
            if (row < column)
            {
               //Далее берем из массива AD по 1 элементу
               if (countElem > 0)
               {
                  if (column == LJ.arr[currADIndex])
                  {
                     a[row][column] = AU.arr[currADIndex];
                     countElem--;
                     currADIndex++;
                  }
                  else
                  {
                     a[row][column] = 0;
                  }
               }
               else
               {
                  a[row][column] = 0;
               }
            }
         }
      }
      //Заполняем нижний треугольник
      int currALUndex = 0;
      for (int row = 0; row < widthHeight; row++)
      {
         for (int column = 0; column < widthHeight; column++)
         {
            if (row > column)
            {
               if (a[column][row] != 0)
               {
                  a[row][column] = AL.arr[currALUndex];
               }
               else
               {
                  a[row][column] = 0;
               }
            }
         }
      }
      //Вывожу на экран для проверки что за *** получилась
      for (int i = 0; i < widthHeight; i++)
      {
         for (int j = 0; j < widthHeight; j++)
         {
            cout << a[i][j] << " ";
         }
         cout << endl;
      }
   }
   int calculate()
   {
      if (widthHeight == 0)
      {
         return 0;
      }
      //Не представляю как посчитать определитель в сжатом формате
      //Поэтому переведу сначала в обычный а потом посчитаю методом гауса
      double** a = new double* [widthHeight];
      for (int i = 0; i < widthHeight; i++)
      {
         a[i] = new double[widthHeight];
      }
      //Заполняю
      //Печать матрицы
      int currADIndex = 0;
      for (int row = 0; row < widthHeight; row++)
      {
         //Считаем сколько элементов на строке row
         int countElem = 0;
         if (row > LI.length)
         {
            countElem = 0;
         }
         else
         {
            countElem = LI.arr[row + 1] - LI.arr[row];
         }
         //Выводим строку
         for (int column = 0; column < widthHeight; column++)
         {
            //На главной диагонали
            if (row == column)
            {
               a[row][column] = AD.arr[row];
               continue;
            }
            //Значения над гл. диаг.
            if (row < column)
            {
               //Далее берем из массива AD по 1 элементу
               if (countElem > 0)
               {
                  if (column == LJ.arr[currADIndex])
                  {
                     a[row][column] = AU.arr[currADIndex];
                     countElem--;
                     currADIndex++;
                  }
                  else
                  {
                     a[row][column] = 0;
                  }
               }
               else
               {
                  a[row][column] = 0;
               }
            }
         }
      }
      //Заполняем нижний треугольник
      int currALUndex = 0;
      for (int row = 0; row < widthHeight; row++)
      {
         for (int column = 0; column < widthHeight; column++)
         {
            if (row > column)
            {
               if (a[column][row] != 0)
               {
                  a[row][column] = AL.arr[currALUndex];
               }
               else
               {
                  a[row][column] = 0;
               }
            }
         }
      }
      //Вывожу на экран для проверки что за *** получилась
      //for (int i = 0; i < widthHeight; i++)
      //{
      //   for (int j = 0; j < widthHeight; j++)
      //   {
      //      cout << a[i][j] << " ";
      //   }
      //   cout << endl;
      //}
      //Привожу матрицу к треугольному виду
      double tmp = 0, d = 1;
      for (int k = 0; k < widthHeight - 1; k++)
      {
         for (int i = k + 1; i < widthHeight; i++)
         {
            tmp = -a[i][k] / a[k][k];
            for (int j = 0; j < widthHeight; j++)
            {
               a[i][j] += a[k][j] * tmp;
            }
         }
      }
      //Вывожу на экран, для дебага
      cout << "\nTarget matrix (Gauss method):\n\n";
      cout.precision(2);
      for (int i = 0; i < widthHeight; i++)
      {
         for (int j = 0; j < widthHeight; j++)
         {
            cout.width(8);
            cout << fixed << a[i][j] << " ";
         }
         cout << "\n";
      }
      //Метод гауса, да
      for (int i = 0; i < widthHeight; i++)
      {
         d *= a[i][i];
      }
      //Очищаю память
      for (int i = 0; i < widthHeight; i++)
      {
         delete[] a[i];
      }
      delete[] a;
      //Возвращаю число
      return d;
   }
};

int main()
{
   SSSMatrix newMatrix;
   //Выбор чтения из консоли или файла
   int isConsoleInput = -1;
   cout << "Enter a 1 for console input or 0 for file input" << endl;
   while (true)
   {
      isConsoleInput = consoleInt();
      if (isConsoleInput != 0 && isConsoleInput != 1)
      {
         cout << "   1 for console and 0 for file!!!" << endl;
      }
      else
      {
         break;
      }
   }
   //Если из консоли
   if (isConsoleInput)
   {
      newMatrix.fillMatrixFromConsole();
   }
   //Из файла
   else
   {
      string fileName = "";
      cout << "Enter a file name: ";
      cin >> fileName;
      newMatrix.fillMatrixFromFile(fileName);
   }
   //Вывод полученного массива
   newMatrix.print();
   //Вывод полной матрицы
   newMatrix.printDecompressed();
   //Вывод детерминанта
   cout << "Determinant: " << newMatrix.calculate() << endl;

   return 0;
}