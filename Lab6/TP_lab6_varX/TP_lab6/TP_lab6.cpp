#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
   cout << "Your rocks: ";
   for (int i = 0; i < arr.size(); i++)
   {
      cout << arr[i] << " ";
   }
   cout << endl;
}

int main()
{
   vector<int> rocks;
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
            rocks.push_back(inInt);
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
         rocks.push_back(simpleArray[i]);
      }
   }
   //Вывод полученного массива если он не пустой
   if (!rocks.empty())
   {
      print(rocks);
      //Функция по заданию
      //Сортируем массив
      bubbleSort(rocks, rocks.size());
      //Выводим
      print(rocks);
      //Собираем две кучи
      vector<int> kucha1, kucha2;
      int kucha1Sum = 0, kucha2Sum = 0;
      for (int i = 0; i < rocks.size(); i++)
      {
         //Если индекс нечетный то кладем в первую кучу. (i & 1) выдаст тру когда
         // число i в битовом представлении оканчивается на 1, 
         // а на 1 оно оканчивается если нечетнное
         if (i & 1)
         {
            kucha1.push_back(rocks[i]);
            kucha1Sum += rocks[i];
         }
         else
         {
            kucha2.push_back(rocks[i]);
            kucha2Sum += rocks[i];
         }
      }
      //Разница между размерами
      int delta = abs(kucha2Sum - kucha1Sum);
      //Если эта разница превосходит хотя бы одну из куч то значит что она больше другой кучи более чем в 2 раза
      if (delta > kucha1Sum || delta > kucha2Sum)
      {
         cout << "No, you can't" << endl;
         print(kucha1);
         print(kucha2);
      }
      else
      {
         cout << "Yes, you can do it" << endl;
         print(kucha1);
         print(kucha2);
      }
   }
   else
   {
      cout << "Empty vector";
   }

   return 0;
}