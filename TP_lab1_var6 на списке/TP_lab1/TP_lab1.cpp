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

using namespace std;

template<typename T>             // Будем использовать шаблонный класс
class List                       // Создаем тип данных Список
{
private:

public:
   // ДОЛЖНО БЫТЬ В ПРИВАТЕ, НО У НАС КОСТЫЛЬ
   struct Item                   //Структура, являющаяся звеном списка
   {
      T data;                    //Значение
      Item* next;                //Указатель на следующее звено списка
      Item* prev;                //Указатель на предыдущее звено списка
   };
   Item* head;                   //Указатель на начало списка
   Item* tail;                   //Указатель на конец списка
   List::Item* create(const T&); //Создание нового звена
   // ДОЛЖНО БЫТЬ В ПРИВАТЕ, НО У НАС КОСТЫЛЬ

   //Конструкторы
   List() : head(nullptr), tail(nullptr)
   {
   };
   List(int);
   ~List();                      //Деструктор
   void addTail(const T&);       //Добавление звена в хвост списка
   void addHead(const T&);       //Добавление звена в голову списка
   T removeHead();               //Удаление головного звена
   T removeTail();               //Удаление хвостового звена
   void print();                 //Вывод списка на экран
};


template<typename T>
List<T>::List(int size)
{
   head = nullptr;
   tail = nullptr;
   for (int i = 0; i < size; i++)
   {
      addTail((T)false);
   }
}

template<typename T>
List<T>::~List()
{
   while (head)
   {
      removeHead();
   }
}

template <typename T>
typename List<T>::Item* List<T>::create(const T& vl)
{
   Item* item = new Item;
   item->data = vl;
   item->next = nullptr;
   item->prev = nullptr;
   return item;
}

template<typename T>
void List<T>::addTail(const T& vl)
{
   if (tail && head)
   {
      tail->next = create(vl);
      tail->next->prev = tail;
      tail = tail->next;
   }
   else
   {
      head = create(vl);
      tail = head;
   }
}

template<typename T>
void List<T>::addHead(const T& vl)
{
   if (tail && head)
   {
      Item* temp = create(vl);
      temp->next = head;
      head = temp;
   }
   else
   {
      head = create(vl);
      tail = head;
   }
}

template<typename T>
T List<T>::removeHead()
{
   if (head)
   {
      Item* temp = head->next;
      T data = head->data;
      delete head;
      head = temp;
      return data;
   }
   else
   {
      return (T)0;
   }
}

template<typename T>
T List<T>::removeTail()
{
   if (tail)
   {
      Item* temp = tail->prev;
      T data = tail->data;
      delete tail;
      tail = temp;
      tail->next = nullptr;
      return data;
   }
   else
   {
      return (T)0;
   }
}


template<typename T>
void List<T>::print()
{
   Item* current = head;
   while (current)
   {
      cout << current->data << " ";
      current = current->next;
   }
   cout << endl;
}

int m, // начальное количество решек
n, // начальное количество гербов
s, // положение поворота
k, // количество ходов
l; // финальное число гербов

List<bool>* coins; // true - орел, false - решка
List<bool>* init_coins;

// Инициализация
void create_coins()
{
   coins = new List<bool>(m + n);
   init_coins = new List<bool>(m + n);
}

int test_and_print_print_coins(bool print)
{
   // Считаем количество coats (забыл как на русcком)
   int coats = 0;
   List<bool>::Item* currItem = coins->head; // Т.к. задача была максимально использовать старый код,
                                             // то все функции остались неизменны, а в случаях,
                                             // где нужно пройтись по списку, используются указатели
                                             // head или tail, которые по правильному должны остаться в привате
   while (currItem)
   {
      coats += currItem->data ? 1 : 0;
      if (print)
      {
         cout << currItem->data;
      }
      currItem = currItem->next;
   }
   if (print)
   {
      cout << " coats: " << coats << endl;
   }
   return coats;
}

void fill_coins()
{
   List<bool>::Item* currItem = init_coins->head;
   for (int i = 0; i < n; i++)
   {
      currItem->data = false;
      currItem = currItem->next;
   }
   for (int i = n; i < m + n; i++)
   {
      currItem->data = true;
      currItem = currItem->next;
   }
}

bool turn_coins()
{
   List<bool>::Item* currItem = coins->head;
   if (currItem->data == false)
   {
      return false;
   }
   for (int i = 1, j; i <= k; i++)
   {
      j = (s * i) % (m + n);
      List<bool>::Item* currItem = coins->head;
      for (int i = 0; i < j; i++)
      {
         currItem = currItem->next;
      }
      currItem->data = !currItem->data;
   }
   return true;
}

// Самодельный метод next_permutation для списка
bool list_next_permutation(List<bool>*& list)
{
   List<bool>::Item* currItem = list->head->next;
   bool flag = 0;
   // Проходимся по списку
   while (currItem)
   {
      // Проверяем, что каждый текущий элемент больше или равен следующему
      if (currItem->prev->data >= currItem->data)
      {
         flag = 1;
      }
      else
      {
         flag = 0;
         break;
      }
      currItem = currItem->next;
   }
   // Если цепочка уже лексикографически по возрастанию, то переделываем ее в минимально возможную
   if (flag)
   {
      while (list->head->data)
      {
         list->removeHead();
         list->addTail(1);
      }
      return false;
   }
   else
   {
      // Сохраняем количество 1
      currItem = list->tail;
      int count = 0;
      while (currItem)
      {
         if (currItem->data)
         {
            count++;
         }
         currItem = currItem->prev;
      }

      // Получение след последовательнности
      int currCount = 0;
      while (count != currCount)
      {
         // Алгоритмом будем увеличивать число в двоичной системе на 1
         currItem = list->tail;
         while (currItem)
         {
            // Все встречающиеся 1 заменяем на 0
            if (currItem->data)
            {
               currItem->data = 0;
            }
            else
            {
               currItem->data = 1;
               break;
            }
            currItem = currItem->prev;
         }
         // Считаем количество 1, в полученной последовательности
         currCount = 0;
         currItem = list->tail;
         while (currItem)
         {
            if (currItem->data)
            {
               currCount++;
            }
            currItem = currItem->prev;
         }
      }
      return true;
   }
}

void list_copy(List<bool>*& lista, List<bool>*& listb)
{
   List<bool>::Item* currItemCoins = lista->head;
   List<bool>::Item* currItemInitCoins = listb->head;
   while (currItemCoins)
   {
      currItemCoins->data = currItemInitCoins->data;
      currItemCoins = currItemCoins->next;
      currItemInitCoins = currItemInitCoins->next;
   }
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
   // Ввод из указанного потока
   if (isConsoleInput)
   {
      string inStr = "";
      cout << "Enter a values:" << endl;
      cout << "m(initial coats)=" << endl;
      m = consoleInt();
      cout << "n(tails)=" << endl;
      n = consoleInt();
      cout << "s(position)=" << endl;
      s = consoleInt();
      cout << "k(turns)=" << endl;
      k = consoleInt();
      cout << "l(final coats)=" << endl;
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
      // Заполняем m,n,s...
      if (size >= 5)
      {
         m = simpleArray[0];
         n = simpleArray[1];
         s = simpleArray[2];
         k = simpleArray[3];
         l = simpleArray[4];
      }
      else
      {
         cout << "File is not completed!";
         exit(-1);
      }
   }

   // Алгоритм по заданию
   cout << "Finally=====" << endl;
   cout << "m(initial coats)=" << m << endl;
   cout << "n(tails)=" << n << endl;
   cout << "s(position)=" << s << endl;
   cout << "k(turns)=" << k << endl;
   cout << "l(final coats)=" << l << endl;
   cout << endl;

   // Инициализируем монеты
   create_coins();
   fill_coins();

   do
   {
      // Копируем из init_coins
      list_copy(coins, init_coins);

      // Проверяем возможно ли перевернуть монеты в текущей последовательности
      if (turn_coins() == false)
      {
         continue;
      }

      // Если количество coats равно заданному, то выходим из цикла
      if (l == test_and_print_print_coins(true))
      {
         // Копируем из init_coins
         list_copy(coins, init_coins);
         cout << endl;
         break;
      }
      // Следующая попытка (другая последовательность)
   } while (result = list_next_permutation(init_coins));

   // Если последовательность существует выведем на экран
   if (result)
   {
      test_and_print_print_coins(true);
      turn_coins();
      test_and_print_print_coins(true);
   }
   else
   {
      cout << "there is no decision" << endl;
   }

   return 0;
}