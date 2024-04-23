#include <iostream>	
#include <string>
#include <fstream>

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


template<typename T>             //Будем использовать шаблонный класс
class List                       //Создаем тип данных Список
{
private:
   struct Item                   //Структура, являющаяся звеном списка
   {
      T data;                    //Значение
      Item* next;                //Указатель на следующее звено списка
   };
   Item* head;                   //Указатель на начало списка
   Item* tail;                   //Указатель на конец списка
   List::Item* create(const T&); //Создание нового звена
public:
   //Конструкторы
   List() : head(nullptr), tail(nullptr)
   {
   };
   List(const T&);
   ~List();                      //Деструктор
   void addTail(const T&);       //Добавление звена в хвост списка
   void addHead(const T&);       //Добавление звена в голову списка
   T removeHead();               //Удаление головного звена
   T removeTail();               //Удаление хвостового звена
   void print();                 //Вывод списка на экран
   bool isEmpty()                //Является ли список пустым
   {
      return !head;
   };
};


template<typename T>
List<T>::List(const T& vl)
{
   head = create(vl);
   tail = head;
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
   return item;
}

template<typename T>
void List<T>::addTail(const T& vl)
{
   if (tail && head)
   {
      tail->next = create(vl);
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
   if (tail && head)
   {
      Item* temp = head;
      while (temp->next && temp->next->next)
      {
         temp = temp->next;
      }
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
   // Если голова пустая, значит список пустой и выводить нечего
   if (!head)
   {
      cout << "empty";
   }
   // Выводим все элемента пока они есть
   Item* current = head;
   while (current)
   {
      cout << current->data << " ";
      current = current->next;
   }
   // Вывод завершаем переходом на след строку
   cout << endl;
}


int main()
{
   // Создаем 4 стэка
   List<int> list1, list2, list3, list4;
   // Выбор чтения из консоли или файла
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
   // Если из консоли
   if (isConsoleInput)
   {
      string inStr = "";
      // Заполняем первый стэк
      cout << "Enter a values for the first list (-1 to stop)" << endl;
      while (true)
      {
         getline(cin, inStr);
         if (inStr == "-1")
         {
            break;
         }
         try
         {
            list1.addTail(stoi(inStr));
         }
         catch (const std::exception&)
         {
            cout << "not a number, try again" << endl;
         }
      }
      // Заполняем второй стэк
      cout << "Enter a values for the second list (-1 to stop)" << endl;
      while (true)
      {
         getline(cin, inStr);
         if (inStr == "-1")
         {
            break;
         }
         try
         {
            list2.addTail(stoi(inStr));
         }
         catch (const std::exception&)
         {
            cout << "not a number, try again" << endl;
         }
      }
   }
   // Ввод из файла (файл будет один, первая половина значений пойдет в 1 стэк, а вторая во 2)
   else
   {
      int* simpleArray = nullptr;
      int size = fileIntArray("input.txt", simpleArray);
      for (auto i = 0; i < size; i++)
      {
         cout << simpleArray[i] << " ";
      }
      cout << endl << "Array from file completed!" << endl;
      // Заполнение первого стэка половиной чисел из файла
      for (int i = 0; i < size / 2; i++)
      {
         list1.addTail(simpleArray[i]);
      }
      // Заполнение первого стэка другой половиной чисел из файла
      for (int i = size / 2; i < size; i++)
      {
         list2.addTail(simpleArray[i]);
      }
   }
   //Вывод полученных списков
   list1.print();
   list2.print();
   list3.print();
   list4.print();
   //По заданию
   // В 3 стэк запихнем четные числа, в 4 стэк нечетные
   // Получаем цифры из 1 стэка
   while (!list1.isEmpty())
   {
      int value = list1.removeHead();
      // Если число их стэка нечетное, добавляем в 4 стэк
      if (value & 1)
      {
         list4.addHead(value);
      }
      else
      {
         list3.addHead(value);
      }
   }
   // Получаем цифры из 2 стэка
   while (!list2.isEmpty())
   {
      int value = list2.removeHead();
      // Если число их стэка нечетное, добавляем в 4 стэк
      if (value & 1)
      {
         list4.addHead(value);
      }
      else
      {
         list3.addHead(value);
      }
   }
   // Выводим на экран, чтобы проверить
   cout << "After first sort" << endl;
   list1.print();
   list2.print();
   list3.print();
   list4.print();
   // Сортируем
   int min, tek;
   while (!list3.isEmpty())
   {
      min = list3.removeHead();
      list1.addHead(min);
      while (!list3.isEmpty())
      {
         tek = list3.removeHead();
         if (tek < min)min = tek;
         list1.addHead(tek);
      }
      while (!list1.isEmpty())
      {
         tek = list1.removeHead();
         if (tek == min)list2.addHead(tek);
         else list3.addHead(tek);
      }
   }
   cout << "=====After sort=====" << endl;
   list1.print();
   list2.print();
   list3.print();
   list4.print();
   // Сортируем 
   while (!list4.isEmpty())
   {
      min = list4.removeHead();
      list1.addHead(min);
      while (!list4.isEmpty())
      {
         tek = list4.removeHead();
         if (tek < min)min = tek;
         list1.addHead(tek);
      }
      while (!list1.isEmpty())
      {
         tek = list1.removeHead();
         if (tek == min)list3.addHead(tek);
         else list4.addHead(tek);
      }
   }
   cout << "=====After sort=====" << endl;
   list1.print();
   list2.print();
   list3.print();
   list4.print();
   return 0;
}