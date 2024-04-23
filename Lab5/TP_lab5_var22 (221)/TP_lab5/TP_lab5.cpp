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
   return length;
}

//Интерфейс для вектора
class myIntVector1d {
private:
   unsigned int elemCounter;
   unsigned int currentArrSize;
public:
   myIntVector1d(int*& arr)
   {
      elemCounter = 0;
      currentArrSize = 10;
      arr = new int[currentArrSize];
   }
   ~myIntVector1d() {
   }

   bool include(int*& arr, int value)
   {
      bool flag = 0;
      for (auto i = 0; i < elemCounter; i++)
      {
         if (arr[i] == value)
         {
            flag = true;
            break;
         }
      }
      return flag;
   }

   void push(int*& arr, int value)
   {
      if (elemCounter < currentArrSize)
      {
         arr[elemCounter] = value;
         elemCounter++;
      }
      else
      {
         increaseArray(arr, 10);
         push(arr, value);
      }
   }

   void print(int*& arr)
   {
      cout << "Your array[";
      if (this->elemCounter != 0)
      {
         int arrAlmostLastIndex = (this->elemCounter - 1);
         for (int i = 0; i < arrAlmostLastIndex; ++i)
         {
            cout << arr[i] << ", ";
         }
         cout << arr[arrAlmostLastIndex] << "]" << endl;
      }
      else
      {
         cout << "empty]" << endl;
      }
   }

   unsigned int length(int*& arr)
   {
      return elemCounter;
   }

private:
   void increaseArray(int*& arr, unsigned int value)
   {
      if (currentArrSize)
      {
         int* tempArr = new int[currentArrSize];
         for (unsigned int i = 0; i < currentArrSize; i++)
         {
            tempArr[i] = arr[i];
         }
         delete[]arr;
         arr = new int[currentArrSize + value];
         for (unsigned int i = 0; i < currentArrSize; i++)
         {
            arr[i] = tempArr[i];
         }
         currentArrSize += value;
      }
      else
      {
         arr = new int[value];
         currentArrSize += value;
      }
   }
};

class myHashMap {
private:
   struct Item {
      int key;
      int data;
      Item* next;

      Item(const int& k, const int& dt) : key(k), data(dt), next(nullptr) {}
      void deleteList() {
         erase(this);
      }
   private:
      void erase(Item* n)
      {
         if (n)
         {
            erase(n->next);
            delete(n);
         }
      }

   };
   Item** buckets;
   int bucketsCount;
   myHashMap::Item* create(const int&); //Создает новый элемент
   int hashCode(const int&);  //Генерирует хэш код
public:
   //Конструкторы
   myHashMap()
   {
      bucketsCount = 16;
      buckets = new Item * [bucketsCount];
      for (auto i = 0; i < bucketsCount; i++)
      {
         buckets[i] = nullptr;
      }
   }
   bool include(const int&);  //Возвращает наличие элемента
   void put(const int&);      //Добавляет значение
   bool del(const int&);      //Удаляет элемент, возвращает успешность операции
   void print();               //Печать всей таблицы
   void rehash(const int&);   //Пересоздает таблицу под N корзин
};

int main()
{
   //Человек который читает! Тут 1 огромный косяк и 1 ну типо окей.
   //Огромный - ключом в лабе обозван хэшкод, а значением ключ) 
   //То есть тот ключ который сейчас есть это значение, а значение которое должно было бы храниться вообще нет в лабе
   //Второй ну типо окей косяк - корзины (buckets) должна были быть отдельной структурой данных
   //Я из-за своей неопытности обьединил хэш таблицу и список. Список должен быть отдельной структурой данных!
   myHashMap hashmap;
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
         hashmap.put(inInt);
      }
   }
   else
   {
      int* simpleArray = nullptr;
      int size = fileIntArray("input.txt", simpleArray);
      cout << "Array filled!" << endl;
      for (auto i = 0; i < size; i++)
      {
         hashmap.put(simpleArray[i]);
      }
   }
   //Вывод на экран
   hashmap.print();
   //Удалим элемент
   cout << "Deleted element 772 - " << (hashmap.del(772) ? "Success delete" : "Not found") << endl;
   //Вывод на экран
   hashmap.print();
   //Рехешируем на 20 элементов
   hashmap.rehash(20);
   cout << "Rehashed hashmap to 20 buckets!" << endl;
   //Вывод на экран
   hashmap.print();
   //Поиск
   cout << "Data to find" << endl;
   int toFind = consoleInt();
   cout << (hashmap.include(toFind) ? "This hashmap include " : "This hashmap doesnt include ") << toFind << endl;
   return 0;

}


myHashMap::Item* myHashMap::create(const int& dt)
{
   Item* item = new Item(hashCode(dt), dt);
   return item;
}

int myHashMap::hashCode(const int& val)
{
   float magicConst = 0.6180339887;
   float step1 = val * magicConst;
   float step2 = fmod(step1, 1);
   float step3 = step2 * bucketsCount;
   int hashCode = floor(step3);
   return hashCode;
}

void myHashMap::put(const int& dt)
{
   int hashCode = this->hashCode(dt);
   //Следующие 5 строчек под вопросом, время 3 ночи, я не понима. почему 6 строка не делает тоже самое
   if (buckets[hashCode] == nullptr)
   {
      buckets[hashCode] = create(dt);
      return;
   }
   Item* current = buckets[hashCode];
   while (current)
   {
      if (current->next == nullptr)
      {
         current->next = create(dt);
         return;
      }
      current = current->next;
   }
}

bool myHashMap::include(const int& dt)
{
   bool flag = false;
   int hashCode = this->hashCode(dt);
   if (buckets[hashCode])
   {
      Item* currentItem = buckets[hashCode];
      while (currentItem)
      {
         if (currentItem->data == dt)
         {
            flag = true;
            break;
         }
         currentItem = currentItem->next;
      }
   }
   return flag;
}

void myHashMap::print()
{
   cout << "===Your hashmap===" << endl;
   for (auto i = 0; i < bucketsCount; i++)
   {
      cout << "Bucket[" << i << "] = ";
      Item* current = buckets[i];
      while (current)
      {
         cout << "[" << current->key << "-" << current->data << "] ";
         current = current->next;
      }
      cout << endl;
   }
}

void myHashMap::rehash(const int& N)
{
   //Все значения сохраняю в массив
   int* simpleArray = nullptr;
   myIntVector1d simpleArrayInterface01(simpleArray);
   for (int i = 0; i < bucketsCount; ++i)
   {
      if (buckets[i])
      {
         Item* currentItem = buckets[i];
         while (currentItem)
         {
            simpleArrayInterface01.push(simpleArray, currentItem->data);
            currentItem = currentItem->next;
         }
      }
   }
   //Удаляем существующие корзины @todo понять почему корзины не очищаются
   for (int i = 0; i < bucketsCount; ++i)
   {
      buckets[i]->deleteList();
   }
   delete[] buckets;
   //Следующей строчкой можно проверить что в корзине до сих пор что-то лежит.
   // (buckets[0]) cout << "!!!!!";
   //Создаем новые
   bucketsCount = N;
   buckets = new Item * [bucketsCount];
   //Следующие 4 строчки зануляют корзины (Вопрос это инициализация или переопределение,
   // я все еще сомневаюсь что именно происходит 8-ю строчками выше)
   for (int i = 0; i < bucketsCount; ++i)
   {
      buckets[i] = nullptr;
   }
   //Заполняем
   for (int i = 0; i < simpleArrayInterface01.length(simpleArray); ++i)
   {
      put(simpleArray[i]);
   }
   //Очищаем память от массива
   delete[] simpleArray;
}

bool myHashMap::del(const int& dt)
{
   if (this->include(dt))
   {
      int hashCode = this->hashCode(dt);
      //Ситуация когда первый элемент тот который удалить
      if (buckets[hashCode] && buckets[hashCode]->data == dt)
      {
         Item* newFirst = buckets[hashCode]->next;
         delete buckets[hashCode];
         buckets[hashCode] = newFirst;
         return true;
      }
      //Со второго и т.д.
      Item* currentItem = buckets[hashCode];
      while (currentItem)
      {
         if (currentItem->next && currentItem->next->data == dt)
         {
            Item* newCurrItemNext = currentItem->next->next;
            delete currentItem->next;
            currentItem->next = newCurrItemNext;
            return true;
         }
         else
         {
            currentItem = currentItem->next;
         }
      }
   }
   return false;
}


