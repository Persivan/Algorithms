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
int fileInt(const string filename, int*& arr)
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


//Получение строки после перевода n в 2-ую систему исчисления из 10-ой
string intToBinaryString(unsigned int n)
{
   string buffer; // символы ответа в обратном порядке
   // выделим память заранее по максимуму
   buffer.reserve(numeric_limits<unsigned int>::digits);
   do
   {
      buffer += char('0' + n % 2); // добавляем в конец
      n = n / 2;
   } while (n > 0);
   return string(buffer.crbegin(), buffer.crend()); // разворачиваем результат
}

//Получение числа после перевода instr в 10-ую систему исчисления из 2-ой
int binaryStringToNormal(string instr)
{
   bitset<64U> bs(instr);
   return bs.to_ullong();
}

// Структура элемента
struct Node
{
    Node(const string k, const string v)
    {
        key = k;
        value = v;
    }

    void clear()
    {
        key.clear();
        value.clear();
    }

    string key;         // ключ
    string value;       // значение
};


class HashMap {
private:
   Node** buckets;
   int bucketsCount;
   int hashCode(const string&);              //Генерирует хэш код из ключа (хэш функция)
public:
   //Конструкторы
   HashMap()
   {
      bucketsCount = 16;
      buckets = new Node*[bucketsCount];
      for (auto i = 0; i < bucketsCount; i++)
      {
         buckets[i] = nullptr;
      }
   }
   bool include(const string&, const string&);//Содержит ли такую пару ключ, значение
   void put(const string&, const string&);   //Добавляет пару ключ, значение
   bool del(const string&, const string&);   //Удаляет элемент по ключу и значению, возвращает успешность операции
   string toString();                        //Всю таблицу в строку
   void rehash(const int&);                  //Пересоздает таблицу под N корзин
   const string get(const string&);          //Поиск по ключу
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
   else
   {
      ifstream fin("input.txt");
      if (fin.is_open())
      {
         string inStrKey = "";
         string inStrDt = "";
         while (!fin.eof())
         {
            getline(fin, inStrKey);
            getline(fin, inStrDt);
            hashmap.put(inStrKey, inStrDt);
         }
      }
      else
      {
         cout << "File read error!" << endl;
      }
   }
   //Вывод на экран
   cout << hashmap.toString();
   //Рехешируем на 32 элементов
   hashmap.rehash(32);
   cout << "Rehashed hashmap to 32 buckets!" << endl;
   //Вывод на экран
   cout << hashmap.toString();




   //Поиск по ключу
   cout << "Enter a key to find?" << endl;
   string toFindKey = "";
   getline(cin, toFindKey);
   cout << "Key-Value = '" << toFindKey << "'-" << "'" << hashmap.get(toFindKey) << "'" << endl;
   //Есть ли ключ-значение
   cout << "Is it contain?" << endl;
   string includeKey = "";
   string includeData = "";
   cout << "Enter a key: " << endl;
   getline(cin, includeKey);
   cout << "Enter a value [" << includeKey << "]" << endl;
   getline(cin, includeData);
   cout << (hashmap.include(includeKey, includeData) ? "This hashmap include " : "This hashmap doesnt include ")
      << "'" << includeKey << "'-'" << includeData << "'" << endl;
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
   //Вывод на экран после удаления
   cout << hashmap.toString();
   system("pause");
   //Рехешируем на 32 элементов
   hashmap.rehash(32);
   cout << "Rehashed hashmap to 32 buckets!" << endl;
   //Вывод на экран
   cout << hashmap.toString();
   return 0;
}

int HashMap::hashCode(const string& key)
{
   //Преобразуем строку в число путем сложения кодов каждого символа
   int intStr = 0;
   for (auto i = 0; i < key.size(); i++)
   {
      intStr += key[i];
   }
   intStr *= intStr;
   //Переведем числа в битовый формат
   string binaryIntStr = intToBinaryString(intStr);
   string binaryBucketsCount = intToBinaryString(bucketsCount);
   //Получу их длины
   int binaryIntStrSize = binaryIntStr.size();
   int binaryBucketsCountSize = binaryBucketsCount.size() - 1; //-1 т.к. корзины считаются с 0. а количество корзин с 1 (ну макс индекс массива и количество элементов массива бла бла бла)
   //Если число полученное из строки меньше чем количество корзин
   if (binaryIntStrSize <= binaryBucketsCountSize)
   {
      int hashCode = binaryStringToNormal(binaryIntStr);
      return hashCode;
   }
   else
   {
      //Получим середину
      int middle = binaryIntStrSize / 2;
      //Получим точку для начала считывания
      int startMiddle = middle - (binaryBucketsCountSize / 2) + 1;
      //Считаем нужные биты
      string hashCodeStr = "";
      for (auto i = 0; i < binaryBucketsCountSize; i++)
      {
         int index = startMiddle + i;
         hashCodeStr += binaryIntStr[index];
      }
      int hashCode = binaryStringToNormal(hashCodeStr);
      return hashCode;
   }
}

void HashMap::put(const string& key, const string& dt)
{
   int hashCode = this->hashCode(key);
   int attemptCount = 1;
   while (true)
   {
      if (buckets[hashCode] == nullptr)
      {
         buckets[hashCode] = new Node(key, dt);
         break;
      }
      else if (buckets[hashCode]->key == key)
      {
         buckets[hashCode]->value = dt;
         break;
      }
      else
      {
         hashCode = hashCode + pow(attemptCount, 2);
         attemptCount++;
      }
      if (hashCode > bucketsCount)
      {
         this->rehash(bucketsCount * 2);          // увеличиваем в 2 раза
      }
   }
}

bool HashMap::include(const string& key, const string& dt)
{
   int hashCode = this->hashCode(key);
   string value = "";
   int attempt = 1;
   while (buckets[hashCode]
      && buckets[hashCode]->key != key
      && buckets[hashCode]->key != "")
   {
      hashCode = pow(attempt, 2) + hashCode;
      attempt++;
   }
   if (buckets[hashCode] && buckets[hashCode]->value == dt)
   {
      return true;
   }
   else
   {
      return false;
   }
}

string HashMap::toString()
{
   string result = "";
   result += "===Your hashmap===\n";
   for (auto i = 0; i < bucketsCount; i++)
   {
      result += "Bucket[" + to_string(i) + "]: ";
      if (buckets[i] != nullptr)
      {
         result += "'" + buckets[i]->key + "' - '" + buckets[i]->value + "'\n";
      }
      else
      {
         result += "\n";
      }
   }
   return result;
}

void HashMap::rehash(const int& N)
{
    //Все значения сохраняю в вектор
    vector<Node*> allHashMapValues;
    for (int i = 0; i < bucketsCount; i++)
    {
        if (buckets[i])
        {
            allHashMapValues.push_back(buckets[i]);
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
    buckets = new Node * [bucketsCount];
    //Инициализируем
    for (int i = 0; i < N; i++)
    {
        buckets[i] = nullptr;
    }
    //Заполняем
    for (int i = 0; i < allHashMapValues.size(); ++i)
    {
        this->put(allHashMapValues[i]->key, allHashMapValues[i]->value);
    }
}

const string HashMap::get(const string& key)
{
   int hashCode = this->hashCode(key);
   string value = "";
   int attempt = 1;
   while (buckets[hashCode]
      && buckets[hashCode]->key != key 
      && buckets[hashCode]->key != "")
   {
      hashCode = pow(attempt,2) + hashCode;
      attempt++;
   }
   if (buckets[hashCode])
   {
      return buckets[hashCode]->value;
   }
   else
   {
      return "not found";
   }
}

bool HashMap::del(const string& key, const string& dt)
{
   if (this->include(key, dt))
   {
      int hashCode = this->hashCode(key);
      string value = "";
      int attempt = 1;
      while (buckets[hashCode]
         && buckets[hashCode]->key != key
         && buckets[hashCode]->key != "")
      {
         hashCode = pow(attempt, 2) + hashCode;
         attempt++;
      }
      if (buckets[hashCode])
      {
         buckets[hashCode]->clear();
         buckets[hashCode] = nullptr;
         return true;
      }
      else
      {
         return false;
      }
   }
   else
   {
      return false;
   }
}