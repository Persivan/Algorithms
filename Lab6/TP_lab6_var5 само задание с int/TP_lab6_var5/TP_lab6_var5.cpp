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

int main()
{
   vector<int> myVector;
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
         myVector.push_back(inInt);
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
         myVector.push_back(simpleArray[i]);
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
   //sort(myVector.begin(), myVector.end());
   bubbleSort(myVector, myVector.size());
   //Вывод сортированного массива
   cout << "Sorted array:";
   for (int i = 0; i < myVector.size(); i++)
   {
      cout << " " << myVector[i];
   }
   cout << endl;
   //Задание
   //Количество разных цифр
   int zeroCounter = 0, posCounter = 0, negCounter = 0;
   for (int i = 0; i < myVector.size(); i++)
   {
      if (myVector[i] == 0)
      {
         zeroCounter++;
         continue;
      }
      if (myVector[i] > 0)
      {
         posCounter++;
         continue;
      }
      if (myVector[i] < 0)
      {
         negCounter++;
      }
   }
   //Дебаг инфа
   cout << "Zero = " << zeroCounter << " posCounter = " << posCounter << " negCounter = " << negCounter << endl;
   //Случай когда есть 0
   if (zeroCounter != 0)
   {
      //Выходим в ситуации, когда в массиве есть 0,
      //но их количество не соответвует для того чтобы получить 0 в произведениях пар
      if (zeroCounter != myVector.size() / 2)
      {
         cout << "Should be N zero's" << endl;
         return 0;
      }
      for (int i = 0; i < myVector.size(); i++)
      {
         if (myVector[i] != 0)
         {
            cout << myVector[i] << " 0 = 0" << endl;
         }
      }
      return 0;
   }
   //Случай когда нет 0
   //Если количество положительных не равно количеству отрицательных
   if (posCounter != negCounter)
   {
      //Найдем индекс первого положительного числа
      int firstPosIndex = 0;
      while (firstPosIndex < myVector.size() && myVector[firstPosIndex] < 0)
      {
         firstPosIndex++;
      }
      cout << endl;
      //Выводим пары
      cout << "Pairs" << endl;
      //Отрицательные числа
      for (int i = 0; i < firstPosIndex / 2; i++)
      {
         cout << myVector[i] << " " << myVector[firstPosIndex - i - 1] << " = " 
            << myVector[i] * myVector[firstPosIndex - i - 1] << endl;
      }
      //Положительные числа
      for (int i = 0; i < (myVector.size() - firstPosIndex) / 2; i++)
      {
         cout << myVector[firstPosIndex + i] << " " << myVector[myVector.size() - i - 1] << " = " 
            << myVector[firstPosIndex + i] * myVector[myVector.size() - i - 1] << endl;
      }
      return 0;
   }
   //Если количество отриц и полож совпало
   if (posCounter == negCounter)
   {
      //Будем строить по 2 пары, то есть брать по 4 числа из массива
      for (int i = 0; i < myVector.size() / 4; i++)
      {
         //Получим 4 числа
         int maxPos = 0, minPos = INT_MAX, minNeg = 0, maxNeg = INT_MIN;
         for (int i = 0; i < myVector.size(); i++)
         {
            //Нули пропускаем, так как нулями заполнены ячейки массива которые уже выводили
            if (myVector[i] == 0)
            {
               continue;
            }
            if (maxPos < myVector[i] && myVector[i] > 0)
            {
               maxPos = myVector[i];
            }
            if (minPos > myVector[i] && myVector[i] > 0)
            {
               minPos = myVector[i];
            }
            if (minNeg > myVector[i] && myVector[i] < 0)
            {
               minNeg = myVector[i];
            }
            if (maxNeg < myVector[i] && myVector[i] < 0)
            {
               maxNeg = myVector[i];
            }
         }
         //Удалим их из массива, пропуская 0
         for (int i = 0; i < myVector.size(); i++)
         {
            if (myVector[i] != 0 && (myVector[i] == minPos ||
               myVector[i] == maxPos ||
               myVector[i] == minNeg ||
               myVector[i] == maxNeg))
            {
               myVector[i] = 0;
            }
         }
         //Узнаем положительные или отрицательные произведения мы можем получить
         //отрицательные числа 
         if (maxPos * maxNeg == minPos * minNeg)
         {
            cout << maxPos << " " << maxNeg << " = " << maxPos * maxNeg << endl;
            cout << minPos << " " << minNeg << " = " << minPos * minNeg << endl;
            continue;
         }
         //положительные числа
         if (maxPos * minPos == minNeg * maxNeg)
         {
            cout << maxPos << " " << minPos << " = " << maxPos * minPos << endl;
            cout << minNeg << " " << maxNeg << " = " << minNeg * maxNeg << endl;
         }
      }
   }
   return 0;
}
