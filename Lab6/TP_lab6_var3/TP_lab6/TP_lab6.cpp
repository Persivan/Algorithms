#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//Ввод из консоли с проверкой каждого числа на int
int consoleInt()
{
   string instr = "";
   int res = 0;
   while (true)
   {
      cout << "Enter a number: ";
      getline(cin, instr);
      try
      {
         res = stoi(instr);
         break;
      }
      catch (const std::exception& ex)
      {
         cout << "Error: " << ex.what() << endl;
      }
   }
   return res;
};

//Ввод из файла
int fileIntArray(const string filename, int*& arr)
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
      cout << "Eror while reading" << endl;
   }
   return length;
}

//Сортировка пузырьковая
void bubbleSort(vector<int>& arr, int n)
{
   int i, j;
   for (i = 0; i < n - 1; i++)
   {
      //Последниий i элементы уже на своих местах
      for (j = 0; j < n - i - 1; j++)
      {
         if (arr[j] > arr[j + 1])
         {
            swap(arr[j], arr[j + 1]);
         }
      }
   }
}


//Функция смены двух значений
void swap(int& xp, int& yp)
{
   int temp = xp;
   xp = yp;
   yp = temp;
}

// Печать 
void print(vector<int>& arr)
{
   cout << "Your array: ";
   for (int i = 0; i < arr.size(); i++)
   {
      cout << arr[i] << " ";
   }
   cout << endl;
}

int main()
{
   vector<int> arr;
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
      cout << "Fill elements (-1 to stop)" << endl;
      while (true)
      {
         int inInt = 0;
         inInt = consoleInt();
         if (inInt == -1)
         {
            break;
         }
         else
         {
            arr.push_back(inInt);
         }
      }
   }
   else
   {
      int* simpleArray = nullptr;
      int size = fileIntArray("input.txt", simpleArray);
      cout << "Array filled!" << endl;
      for (int i = 0; i < size; i++)
      {
         arr.push_back(simpleArray[i]);
      }
   }
   //Если он пустой, то выходим из программы
   if (arr.empty())
   {
      cout << "Your array is empty!" << endl;
      return 0;
   }
   // Разобьем массив на массив A и B
   vector<int> A, B;
   for (int i = 0; i < 10; i++)
   {
      if (i >= arr.size())
      {
         A.push_back(0);
         continue;
      }
      A.push_back(arr[i]);
   }
   for (int i = 10; i < 25; i++)
   {
      if (i >= arr.size())
      {
         B.push_back(0);
         continue;
      }
      B.push_back(arr[i]);
   }
   // Выводим полученные массивы
   cout << "Array A:";
   print(A);
   cout << "Array B:";
   print(B);
   // Сортируем массивы, Удаляем повторяющиеся значения
   bubbleSort(A, A.size());
   A.erase(unique(A.begin(), A.end()), A.end());
   bubbleSort(B, B.size());
   B.erase(unique(B.begin(), B.end()), B.end());
   // Выводим полученные массивы
   cout << "Array A:";
   print(A);
   cout << "Array B:";
   print(B);
   // Находим первый индекс меньшего массива в большом массиве
   if (A.size() > B.size())
   {
      int i = 0;
      for (i = 0; i < A.size(); i++)
      {
         if (A[i] == B[0])
         {
            break;
         }
      }
      bool flag = 0;
      if (i == A.size())
      {
         flag = 1;
      }
      for (int j = 0; i < A.size() && j < B.size(); i++, j++)
      {
         if (A[i] != B[j])
         {
            flag = 1;
            break;
         }
      }
      if (flag)
      {
         cout << "No, this B is not included in A" << endl;
         return 0;
      }
   }
   else
   {
      int i = 0;
      for (i = 0; i < B.size(); i++)
      {
         if (B[i] == A[0])
         {
            break;
         }
      }
      bool flag = 0;
      if (i == B.size())
      {
         flag = 1;
      }
      for (int j = 0; i < B.size() && j < A.size(); i++, j++)
      {
         if (B[i] != A[j])
         {
            flag = 1;
            break;
         }
      }
      if (flag)
      {
         cout << "No, this A is not included in B" << endl;
         return 0;
      }
   }
   cout << "Yes! Some of the array is included in other!" << endl;
   return 0;
}