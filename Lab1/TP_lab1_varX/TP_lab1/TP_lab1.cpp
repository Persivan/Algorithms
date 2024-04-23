#include <iostream>	
#include <string>
#include <fstream>

using namespace std;

template<typename T>             //Будем использовать шаблонный класс
class List                       //Создаем тип данных Список
{
private:
   struct Item                   //Структура, являющаяся звеном списка
   {
      T data;                    //Значение
      Item* next;                //Указатель на следующее звено списка
      Item* prev;                //Указатель на предыдущее звено списка
   };
   Item* head;                   //Указатель на начало списка
   Item* tail;                   //Указатель на конец списка
   List::Item* create(const T&); //Создание нового звена
public:
   //Конструкторы
   List() : head(nullptr), tail(nullptr) {};
   List(const T&);
   ~List();                      //Деструктор
   void addTail(const T&);       //Добавление звена в хвост списка
   void addHead(const T&);       //Добавление звена в голову списка
   T removeHead();               //Удаление головного звена
   T removeTail();               //Удаление хвостового звена
   void print();                 //Вывод списка на экран
   bool isEmpty() { return head; };//Является ли список пустым
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



int main()
{
   List<char> list;
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
      cout << "Enter a values (-1 to stop)" << endl;
      while (true)
      {
         getline(cin, inStr);
         if (inStr == "-1")
         {
            break;
         }
         for (int i = 0; i < inStr.size(); i++)
         {
            list.addTail(inStr[i]);
         }
      }
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
         list.addTail(simpleArray[i]);
      }
   }
   //Вывод полученного списка
   list.print();
   //По заданию
   string strToModify = "";
   cout << "Enter a string to delete all chars from list:" << endl;
   getline(cin, strToModify);
   while (!list.isEmpty())
   {
      //Метод removeHead удалит из головы элемент и вернет его значение
      char ch = list.removeHead();
      //Методов erase удаляем этот элемент. 
      strToModify.erase(strToModify.find(ch), 1);
   }
   list.print();
   return 0;
}