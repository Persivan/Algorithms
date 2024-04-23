#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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

class Item
{
public:
   char data;
   Item* next;
   Item* prev;
   Item(int dt)
   {
      data = dt;
      next = nullptr;
      prev = nullptr;
   }
};

class List
{
private:
   Item* head;
   Item* tail;
   int size;

public:
   //Конструкторы
   List()
   {
      size = 0;
      head = nullptr;
      tail = nullptr;
   }
   //Методы
   void addHead(char);
   void addTail(char);
   int popHead();
   int popTail();
   int getSize();
   void magicFunc();

   void print();
   void printPairs();
};

int main()
{
   List list01;
   cout << "Enter 1 for console input or 2 for file input: ";
   int inputChoose = 0;
   while (true)
   {
      inputChoose = consoleInt();
      if (inputChoose == 1 || inputChoose == 2)
      {
         break;
      }
      else
      {
         cout << "Should be 1 or 2" << endl;
      }
   }
   //Чтение из консоли
   if (inputChoose == 1)
   {
      cout << "Enter a values ('stop' to stop)" << endl;
      string strFromConsole = "";
      while (true)
      {
         getline(cin, strFromConsole);
         if (strFromConsole == "stop")
         {
            break;
         }
         else
         {
            for (int i = 0; i < strFromConsole.size(); i++)
            {
               list01.addHead(strFromConsole[i]);
            }
         }
      }
   }
   //Чтение из файла
   else
   {
      string strFromFile = "";
      string allFile = "";
      fstream fs("input_test.txt"); // input_good, input_bad, input_cycle
      if (fs.is_open())
      {
         while (getline(fs, strFromFile))
         {
            allFile += strFromFile;
         }
         for (int i = 0; i < allFile.size(); i++)
         {
            list01.addTail(allFile[i]);
         }
      }
      else
      {
         cout << "File doesnt exist" << endl;
      }
   }
   //Вывод результа
   list01.printPairs();
   //Функция по заданию
   list01.magicFunc();
   return 0;
}

void List::addHead(char dt)
{
   if (head == nullptr && tail == nullptr)
   {
      head = new Item(dt);
      tail = head;
      size++;
   }
   else
   {
      Item* temp = head;
      head = new Item(dt);
      head->next = temp;
      temp->prev = head;
      size++;
   }
}

void List::addTail(char dt)
{
   if (head == nullptr && tail == nullptr)
   {
      head = new Item(dt);
      tail = head;
      size++;
   }
   else
   {
      Item* temp = tail;
      tail = new Item(dt);
      tail->prev = temp;
      temp->next = tail;
      size++;
   }
}

int List::popHead()
{
   int value = 0;
   if (head)
   {
      value = head->data;
      Item* temp = head;
      head = head->next;
      delete temp;
      size--;
   }
   else
   {
      cout << "Your list is empty!" << endl;
   }
   return value;
}

int List::popTail()
{
   int value = 0;
   if (tail)
   {
      value = tail->data;
      Item* temp = tail;
      tail = tail->prev;
      delete temp;
      size--;
   }
   else
   {
      cout << "Your list is empty!" << endl;
   }
   return value;
}

int List::getSize()
{
   return size;
}

void List::magicFunc()
{
   // Защита что список не пустой
   if (!head)
   {
      cout << "The list is empty!" << endl;
      return;
   }
   // Случай когда в списке 2 элемента
   if (head->next && !head->next->next)
   {
      // Если они повторяются, то это противоречивая (здесь можно было использовать тернарный оператор)
      if (head->data == head->next->data)
      {
         cout << "b) Wrong: ";
      }
      else
      {
         cout << "a) Full: ";
      }
      
   }

   // Будем изменять эту переменную как результат проверки (1 - полная, 2 - противоречивая (зацикленная), 3 - имеет ошибки)
   int resultMode = 0; 
   // Указатель текущего элемента (начинаем не с головы а со следующего т.к. нам не важно какая голова)
   Item* currentItem = head->next;
   bool isSecond = true;
   while (true)
   {
      // Выход из алгоритма
      if (!currentItem->next)
      {
         break;
      }
      // Проверка совпадает ли текущий элемент со следующим
      if (isSecond && currentItem->data != currentItem->next->data)
      {
         resultMode = 3;
         break;
      }
      else
      {
         resultMode = 1;
      }
      // Переход к следующему
      isSecond = !isSecond;
      currentItem = currentItem->next;
   }
   // Проверим на зацикленность в случае если цепочка полная
   if (resultMode == 1 && size > 3)
   {
      Item* moveableItem = head;
      currentItem = head;
      int i = 0, j = 0;
      // Проход по списку дввумя указателями
      while (currentItem)
      {
         while (moveableItem)
         {
            if (currentItem != moveableItem &&
               currentItem->data == moveableItem->data &&
               ((abs(i - j) == 1 && i % 2 == 0 && j % 2 == 1) ||
                  (i % 2 == 1 && j % 2 == 0) ||
                  (i % 2 == 0 && j % 2 == 1)))
            {
               resultMode = 2;
               break;
            }
            // Переход к следующему
            j++;
            moveableItem = moveableItem->next;
         }
         if (resultMode == 2)
         {
            break;
         }
         // Переход к следующему
         i++;
         currentItem = currentItem->next;
      }
   }
   
   
   // Вывод результата
   if (resultMode == 1)
   {
      cout << "a) Full: ";
   }
   else if (resultMode == 2)
   {
      cout << "b) Wrong: ";
   }
   else if (resultMode == 3)
   {
      cout << "Your list is not completed: ";
   }
   // Если алгоритм не сработал и почему-то не изменил переменную
   else
   {
      cout << "Something went wrong" << endl;
      exit(-1);
   }
   // вывод на экран
   print();
}

void List::print()
{
   Item* currentItem = head;
   while (currentItem)
   {
      cout << currentItem->data << " ";
      currentItem = currentItem->next;
   }
   cout << endl;
}

void List::printPairs()
{
   Item* currentItem = head;
   if (!(size & 1))
   {
      while (currentItem)
      {
         cout << currentItem->data << " > ";
         currentItem = currentItem->next;
         cout << currentItem->data << endl;
         currentItem = currentItem->next;
      }
      cout << endl;
   }
}
