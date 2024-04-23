#include <stdlib.h>
#include <iostream>     
#include <algorithm> 
#include <fstream>
#include <string>

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
      catch (const exception& ex)
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
         }
         catch (const exception& ex)
         {
            //cout << "Error: " << ex.what() << endl;
         }
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
   }
   else
   {
      cout << "File not found!" << endl;
   }
   return length;
}

int m, // начальное количество решек
n, // начальное количество гербов
s, // положение поворота
k, // количество ходов
l; // финальное число гербов

bool* coins; // true - орел, false - решка
bool* init_coins;

void create_coins()
{
   coins = new bool[m + n];
   init_coins = new bool[m + n];
}

void delete_coins()
{
   delete coins;
   delete init_coins;
}

int test_and_print_print_coins(bool print)
{
   // печать
   cout << "========" << endl;
   for (int i = 0; i < m + n; i++)
   {
      cout << " " << coins[i];
   }
   cout << endl;
   for (int i = 0; i < m + n; i++)
   {
      cout << " " << init_coins[i];
   }
   cout << "\n=======" << endl;

   int coats = 0;
   for (int i = 0; i < m + n; coats += coins[i] ? 1 : 0, i++)
      if (print) cout << coins[i];
   if (print) cout << " coats: " << coats << endl;
   return coats;
}

void fill_coins()
{
   for (int i = 0; i < n; i++) init_coins[i] = false;
   for (int i = n; i < m + n; i++) init_coins[i] = true;
}

bool turn_coins()
{
   if (coins[0] == false) return false;

   for (int i = 1, j; i <= k; i++)
   {
      j = (s * i) % (m + n);
      coins[j] = coins[j] ? false : true;
   }
   return true;
}

int main()
{
   bool result = true;

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
   if (isConsoleInput)
   {
      string inStr = "";
      cout << "Enter a values:" << endl;
      cout << "m(initial coats)=" << endl;
      m = consoleInt();
      cout << "n(tails)=" << endl;
      n = consoleInt();
      cout << "s(position)="  << endl;
      s = consoleInt();
      cout << "k(turns)="  << endl;
      k = consoleInt();
      cout << "l(final coats)="  << endl;
      l = consoleInt();
   }
   else
   {
      int* simpleArray = nullptr;
      int size = fileIntArray("input.txt", simpleArray);
      for (auto i = 0; i < size; i++)
      {
         cout << simpleArray[i] << " ";
      }
      cout << endl << "Array from file completed!" << endl;
      for (int i = 0; i < size; i++)
      {
         try
         {
            m = simpleArray[0];
            n = simpleArray[1];
            s = simpleArray[2];
            k = simpleArray[3];
            l = simpleArray[4];
         }
         catch (const std::exception&)
         {
            cout << "File is not completed!";
            exit(-1);
         }
      }
   }

   cout << "Finally=====" << endl;
   cout << "m(initial coats)=" << m << endl;
   cout << "n(tails)=" << n << endl;
   cout << "s(position)=" << s << endl;
   cout << "k(turns)=" << k << endl;
   cout << "l(final coats)=" << l << endl;
   cout << endl;

   create_coins();
   fill_coins();





   sort(init_coins, init_coins + m + n);



   do
   {

      copy(init_coins, init_coins + n + m, coins);
      if (turn_coins() == false)
      {
         continue;
      }

      // печать
      cout << "========test1==================================================" << endl;
      for (int i = 0; i < m + n; i++)
      {
         cout << " " << coins[i];
      }
      cout << endl;
      for (int i = 0; i < m + n; i++)
      {
         cout << " " << init_coins[i];
      }
      cout << "\n==========================" << endl;

      if (l == test_and_print_print_coins(true))
      {
         copy(init_coins, init_coins + n + m, coins);
         cout << endl;
         break;
      }
   } while (result = next_permutation(init_coins, init_coins + m + n));

   if (result)
   {
      test_and_print_print_coins(true);
      turn_coins();
      test_and_print_print_coins(true);
   }
   else cout << "there is no decision" << endl;

   delete_coins();

   system("pause");
   return 0;
}