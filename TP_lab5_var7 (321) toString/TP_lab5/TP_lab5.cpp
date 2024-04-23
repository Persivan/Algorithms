#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
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
            //Если length (количество слов) совпало с длиной массива
            if (length == arrLength)
            {
               int* tArr = new int[length];
               for (unsigned int i = 0; i < length; i++)
               {
                  tArr[i] = arr[i];
               }
               delete[]arr;
               arrLength += 10;
               arr = new int[arrLength];
               for (unsigned int i = 0; i < length; i++)
               {
                  arr[i] = tArr[i];
               }
               delete[]tArr;
            }
            arr[length++] = result;
         }
         catch (const exception& ex)
         {
            cout << "Error: " << ex.what() << endl;
         }
      }
   }
   else
   {
      cout << "File read error!" << endl;
   }
   return length;
}

template<typename T>             //Будем использовать шаблонный класс
class List                       //Создаем тип данных Список
{
private:
   struct Item                   //Структура, являющаяся звеном списка
   {
      T key;                     //Ключ
      T data;                    //Значение
      Item* next;                //Указатель на следующее звено списка
      Item* prev;                //Указатель на предыдущее звено списка
   };
   Item* head;                   //Указатель на начало списка
   Item* tail;                   //Указатель на конец списка
   List::Item* create(const T&, const T&); //Создание нового звена
public:
   //Конструкторы
   List() : head(nullptr), tail(nullptr) {};
   List(const T&);
   ~List();                      //Деструктор
   void removeHead();            //Удаление головы списка
   void put(const T&, const T&);//Добавление элемента
   string toString();               //Вывод списка на экран
   bool isEmpty() { return head; };//Является ли список пустым
   vector<T> toStringVector();   //Перевод всего списка в вектор
   string findByKey(const T&);      //Вернуть значение по ключу
   bool include(const T&, const T&);//Проверяет есть ли такое ключ значение
   bool remove(const T&, const T&);//Удаляет элемент с ключом занчением
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

template<typename T>
void List<T>::removeHead()
{
   if (head)
   {
      Item* temp = head->next;
      T data = head->data;
      delete head;
      head = temp;
   }
}

template<typename T>
void List<T>::put(const T& key, const T& data)
{
   if (tail && head)
   {
      Item* curr = head;
      bool flag = false;
      while (curr)
      {
         if (curr->key == key)
         {
            curr->data = data;
            flag = true;
         }
         curr = curr->next;
      }
      if (!flag)
      {
         tail->next = create(key, data);
         tail->next->prev = tail;
         tail = tail->next;
      }
   }
   else
   {
      head = create(key, data);
      tail = head;
   }
}

template <typename T>
typename List<T>::Item* List<T>::create(const T& key, const T& vl)
{
   Item* item = new Item;
   item->data = vl;
   item->key = key;
   item->next = nullptr;
   item->prev = nullptr;
   return item;
}

template<typename T>
string List<T>::toString()
{
   string result = "";
   Item* current = head;
   while (current)
   {

      result += "[" + current->key + "]=[" + current->data + "]  ";
     
      current = current->next;
   }
   result += "\n";
   return result;
}

template<typename T>
vector<T> List<T>::toStringVector()
{
   vector<T> allValues;
   Item* current = head;
   while (current)
   {
      allValues.push_back(current->key);
      allValues.push_back(current->data);
      current = current->next;
   }
   return allValues;
}

template<typename T>
string List<T>::findByKey(const T& key)
{
   Item* curr = head;
   string result = "not found";
   while (curr)
   {
      if (curr->key == key)
      {
         result = curr->data;
         break;
      }
      curr = curr->next;
   }
   return result;
}

template<typename T>
bool List<T>::include(const T& key, const T& data)
{
   bool result = false;
   Item* curr = head;
   while (curr)
   {
      if (curr->key == key && curr->data == data)
      {
         result = true;
         break;
      }
      curr = curr->next;
   }
   return result;
}

template<typename T>
bool List<T>::remove(const T& key, const T& data)
{
   bool flag = false;
   if (head->key == key && head->data == data)
   {
      removeHead();
      flag = true;
   }
   else
   {
      Item* curr = head;
      while (curr)
      {
         if (curr->next)
         {
            if (curr->next->data == data, curr->next->key == key)
            {
               Item* toDel = curr->next;
               curr->next = curr->next->next;
               delete toDel;
               flag = true;
               break;
            }
         }
         curr = curr->next;
      }
   }
   return flag;
}

class HashMap {
private:
   List<string>** buckets;
   int bucketsCount;
   int hashCode(const string&);              //Генерирует хэш код из ключа (хэш функция)
public:
   //Конструкторы
   HashMap()
   {
      bucketsCount = 16;
      buckets = new List<string>* [bucketsCount];
      for (auto i = 0; i < bucketsCount; i++)
      {
         buckets[i] = new List<string>;
      }
   }
   bool include(const string&, const string&);//Входит ли такая пара ключ значение
   const string get(const string&);          //Поиск по ключу
   void put(const string&, const string&);   //Добавляет пару ключ, значение
   bool del(const string&, const string&);   //Удаляет элемент по ключу и значению, возвращает успешность операции
   string toString();                        //Печать всей таблицы
   void rehash(const int&);                  //Пересоздает таблицу под N корзин
};

int main()
{
   HashMap hashmap;
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
      cout << "Use menu for add" << endl;
   }
   //Если из файла
   else
   {
      ifstream fin("input.txt");
      if (fin.is_open())
      {
         string inStrKey = "";
         string inStrDt = "";
         while (!fin.eof())
         {
            //Мы берем по 2 строчки из файла
            //Данная защита будет выводить сообещние если в файле нечетное количество строк
            if (!getline(fin, inStrKey))
            {
               cout << "Bad string in file!" << endl;
            };
            if (!getline(fin, inStrDt))
            {
               cout << "Bad string in file!" << endl;
            };
            //Проверка на пустые строки (в файле часто пустые строки, но никто их не замечает почему-то)
            if (inStrKey != "" && inStrDt != "")
            {
               hashmap.put(inStrKey, inStrDt);
            }
            else
            {
               cout << "Empty key or value (do not add)" << endl;
            }
         }
      }
      else
      {
         cout << "File read error!" << endl;
      }
   }
   //Интерфейс
   while (true)
   {
      cout << endl;
      cout << "===Menu===" << endl;
      cout << " 1) Print" << endl;
      cout << " 2) Find" << endl;
      cout << " 3) Put" << endl;
      cout << " 4) Delete" << endl;
      cout << " 5) Rehash" << endl;
      cout << " 6) Exit" << endl;
      cout << "Enter a number (1-6): " << endl;
      cout << endl;
      int subMenu = consoleInt();
      if (subMenu < 1 || subMenu > 6)
      {
         cout << " Enter a number from 1 to 6! just type '1'!" << endl;
         continue;
      }
      else
      {
         if (subMenu == 1)
         {
            //Вывод на экран
            cout << hashmap.toString();
         }
         else if (subMenu == 2)
         {
            //Поиск по ключу
            cout << "Enter a key to find?" << endl;
            string toFindKey = "";
            getline(cin, toFindKey);
            cout << "Key-Value = '" << toFindKey << "'-" << "'" << hashmap.get(toFindKey) << "'" << endl;
         }
         else if (subMenu == 3)
         {
            cout << "Fill elements (-1 to stop)" << endl;
            while (true)
            {
               string inStrKey = "";
               string inStrDt = "";
               cout << "Key to add:" << endl;
               getline(cin, inStrKey);
               if (inStrKey == "-1")
               {
                  break;
               }
               cout << "Value to [" << inStrKey << "] add:" << endl;
               getline(cin, inStrDt);
               if (inStrDt == "-1")
               {
                  break;
               }
               hashmap.put(inStrKey, inStrDt);
            }
         }
         else if (subMenu == 4)
         {
            //Удалим элемент
            cout << "What u want to delete?" << endl;
            string inStrKeyToDel = "";
            string inStrDtToDel = "";
            cout << "Enter a key" << endl;
            getline(cin, inStrKeyToDel);
            cout << "Enter a value" << endl;
            getline(cin, inStrDtToDel);
            cout << "Deleted '" << inStrKeyToDel << "'-'" << inStrDtToDel << "'"
               << (hashmap.del(inStrKeyToDel, inStrDtToDel) ? " Success delete" : "Not found") << endl;
         }
         else if (subMenu == 5)
         {
            //Рехешируем
            cout << "Enter new buckets count:" << endl;
            int rehashTo = consoleInt();
            hashmap.rehash(rehashTo);
         }
         else if (subMenu == 6)
         {
            cout << "Bye" << endl;
            break;
         }
      }
      system("pause");
   }
   return 0;
}

int HashMap::hashCode(const string& key)
{
    int intStr = 0;
    for (auto i = 0; i < key.size(); i++)
    {
        intStr += key[i];
    }
    intStr *= intStr;
    //Если число полученное из строки меньше чем количество корзин
    float magicConst = 0.6180339887;
    float step1 = intStr * magicConst;
    float step2 = fmod(step1, 1);
    float step3 = step2 * bucketsCount;
    int hashCode = floor(step3);
    return hashCode;
}

void HashMap::put(const string& key, const string& dt)
{
   int hashCode = this->hashCode(key);
   buckets[hashCode]->put(key, dt);
}

string HashMap::toString()
{
   string result = "";
   result = "===Your hashmap===\n";
   for (auto i = 0; i < bucketsCount; i++)
   {
      result += "Bucket[" + to_string(i) + "]: ";
      if (buckets[i] != nullptr)
      {
         result += buckets[i]->toString();
      }
   }
   return result;
}

void HashMap::rehash(const int& N)
{
   //Все значения сохраняю в вектор
   vector<string> allHashMapValues;
   for (int i = 0; i < bucketsCount; i++)
   {
      if (buckets[i])
      {
         vector<string> vectorFromTree = buckets[i]->toStringVector();
         for (int i = 0; i < vectorFromTree.size(); i++)
         {
            allHashMapValues.push_back(vectorFromTree[i]);
         }
      }
   }
   //Удаляем существующие корзины
   for (int i = 0; i < bucketsCount; ++i)
   {
      buckets = nullptr;
   }
   delete[] buckets;
   //Создаем новые
   bucketsCount = N;
   buckets = new List<string> * [bucketsCount];
   //Инициализируем
   for (int i = 0; i < bucketsCount; ++i)
   {
      buckets[i] = new List<string>;
   }
   //Заполняем
   for (int i = 0; i < allHashMapValues.size(); i++)
   {
      string key = allHashMapValues[i];
      i++;
      string data = allHashMapValues[i];
      put(key, data);
   }
}

bool HashMap::include(const string& key, const string& data)
{
   bool result = false;
   int hashcode = hashCode(key);
   result = buckets[hashcode]->include(key, data);
   return result;
}

const string HashMap::get(const string& key)
{
   int hashCode = this->hashCode(key);
   return buckets[hashCode]->findByKey(key);
}

bool HashMap::del(const string& key, const string& dt)
{
   if (this->include(key, dt))
   {
      int hashCode = this->hashCode(key);
      buckets[hashCode]->remove(key, dt);
      return true;
   }
   else
   {
      return false;
   }
}