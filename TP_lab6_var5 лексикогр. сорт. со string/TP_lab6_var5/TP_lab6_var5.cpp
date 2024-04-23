#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>

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
      cout << "File not found, closing" << endl;
      return 0;
   }
   return length;
}

//Сортировка пузырьковая
void bubbleSort(vector<int>& arr, int n)
{
   for (int i = 0; i < n - 1; i++)
   {
      //Последниий i элементы уже на своих местах
      for (int j = 0; j < n - i - 1; j++)
      {
         if (arr[j] > arr[j + 1])
         {
            swap(arr[j], arr[j + 1]);
         }
      }
   }
}

bool lexicMore(string first, string second)
{
   //Пока первый символы совпадают 
   int index = 0;
   while (index < first.size() &&
      index < second.size() &&
      first[index] == second[index])
   {
      index++;
   }
   cout << first[index] << second[index] << endl;
   //Защита, если одна строка короче второй
   if (index == first.size() || index == second.size())
   {
      index--;
   }
   return first[index] > second[index];
}

void lexicSort(vector<string>& arr, int n)
{
   for (int i = 0; i < n - 1; i++)
   {
      //Последниий i элементы уже на своих местах
      for (int j = 0; j < n - i - 1; j++)
      {
         if (lexicMore(arr[j], arr[j + 1]))
         {
            swap(arr[j], arr[j + 1]);
         }
      }
   }
}


int main()
{
   vector<string> myVector;
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
         myVector.push_back(to_string(inInt));
      }
   }
   else
   {
      int* simpleArray = nullptr;
      int size = fileIntArray("input.txt", simpleArray);
      cout << "Array filled!" << endl;
      //Проверка если из файла было получено недостаточно чисел
      if (size % 2 != 0)
      {
         cout << "Numbers should be 2N" << endl;
         return 0;
      }
      for (auto i = 0; i < size; i++)
      {
         myVector.push_back(to_string(simpleArray[i]));
      }
   }
   //Вывод исходного массива
   cout << "Unsorted array:";
   for (int i = 0; i < myVector.size(); i++)
   {
      cout << " " << myVector[i];
   }
   cout << endl;
   //Соритврока
   lexicSort(myVector, myVector.size());
   //Вывод сортированного массива
   cout << "Sorted array:";
   for (int i = 0; i < myVector.size(); i++)
   {
      cout << " " << myVector[i];
   }
   cout << endl;
   //Задание
   //Переводить числа обратно в int?
   //Потом сортировать в нормальном порядке?
   //Очень странное задание...
   return 0;
}
