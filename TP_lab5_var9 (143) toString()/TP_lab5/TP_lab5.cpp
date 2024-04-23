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

//Дерево для обработки колизий
class Tree
{
private:
   struct Item                //Структура звена
   {
      string key;             //Ключ звена
      string data;            //Значения звена
      Item* left = nullptr;   //Левое звено
      Item* right = nullptr;  //Правое звено
   };
   Item* root;                //Корень дерева
   Tree::Item* create(const string, const string);          //Создание нового звена
   Tree::Item* rAddElem(Item*, const string, const string); //Добавление элемента
   void rToString(const string&, Item*, bool, bool, string&);//Печать элементов поддеревьев
   bool rInclude(Item*, const string, const string);        //Содержит ли такую пару ключ, значение
   Tree::Item* rRemove(Item*, const string, const string);  //Удаляет элемент по ключу и значению
   void rToArray(Item*, vector<string>&);                   //Заполняет вектор значениямии из дерева
   void rFindByKey(Item*, const string, string&);           //Находит значение по ключу
public:
   Tree() : root(nullptr) {};                   //Конструктор
   //Методы использующие рекурсивные методы
   void addElem(const string, const string);    //Добавление элемента в дерево
   string toString();                           // Печать всего дерева
   bool include(const string, const string);    //Поиск по ключу и значению
   void remove(const string, const string);     //Удаляет элемент по ключу и значению
   vector<string> toStringVector();             //Заполняет массива значениями элементов дерева 
   const string findByKey(const string);        //Возвращает строку по ключу или "not found"
};

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

class HashMap {
private:
   Tree** buckets;
   int bucketsCount;
   int hashCode(const string&);              //Генерирует хэш код из ключа (хэш функция)
public:
   //Конструкторы
   HashMap()
   {
      bucketsCount = 16;
      buckets = new Tree * [bucketsCount];
      for (auto i = 0; i < bucketsCount; i++)
      {
         buckets[i] = nullptr;
      }
   }
   bool include(const string&, const string&);//Содержит ли такую пару ключ, значение
   void put(const string&, const string&);   //Добавляет пару ключ, значение
   bool del(const string&, const string&);   //Удаляет элемент по ключу и значению, возвращает успешность операции
   string toString();                             //Печать всей таблицы
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
   //Считаем хэшкод
   int hashCode = intStr % bucketsCount;
   return hashCode;
}

void HashMap::put(const string& key, const string& dt)
{
   int hashCode = this->hashCode(key);
   if (buckets[hashCode] == nullptr)
   {
      buckets[hashCode] = new Tree;
   }
   buckets[hashCode]->addElem(key, dt);
}

bool HashMap::include(const string& key, const string& dt)
{
   bool flag = false;
   int hashCode = this->hashCode(key);
   if (buckets[hashCode])
   {
      flag = buckets[hashCode]->include(key, dt);
   }
   return flag;
}



void HashMap::rehash(const int& N)
{
   int InitValue = 1;
   while (InitValue < N)
   {
      InitValue *= 2;
   }
   if (InitValue == N)
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
      buckets = new Tree * [bucketsCount];
      //Инициализируем
      for (int i = 0; i < bucketsCount; ++i)
      {
         buckets[i] = nullptr;
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
   else
   {
      cout << "Rehash only to 2^n!";
   }
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
      else
      {
         result += "\n";
      }
   }
   return result;
}

Tree::Item* Tree::create(const string key, const string dt)
{
   Item* item = new Item;
   item->key = key; 
   item->data = dt;
   return item;
}

Tree::Item* Tree::rAddElem(Item* rot, const string key, const string dt)
{
   if (rot)
   {
      if (rot->key > key)
      {
         rot->left = rAddElem(rot->left, key, dt);
      }
      else if (rot->key < key)
      {
         rot->right = rAddElem(rot->right, key, dt);
      }
      else
      {
         rot->data = dt;
         rot->key = key;
      }
   }
   else
   {
      rot = create(key, dt);
   }
   return rot;
}

void Tree::rToString(const string& prefix, Item* rot, bool isLeft, bool notAlone, string& strTree)
{
   if (rot)
   {
      strTree += prefix + ((isLeft && notAlone) ? "|--" : "`--") + "[" + rot->key + "] - [" + rot->data + "]" + "\n";
      rToString(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->left, true, (rot->right && rot->left), strTree);
      rToString(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->right, false, (rot->right && rot->left), strTree);
   }
}

bool Tree::rInclude(Item* rot, const string key, const string dt)
{
   if (rot)
   {
      if (rot->key == key && rot->data == dt)
      {
         return true;
      }
      if (rInclude(rot->left, key, dt))
      {
         return true;
      }
      if (rInclude(rot->right, key, dt))
      {
         return true;
      }
      return false;
   }
   else
   {
      return false;
   }
}

Tree::Item* Tree::rRemove(Item* rot, const string key, const string dt)
{
   if (rot == nullptr) {
      //если дерево пустое
      return rot;
   }
   //поиск удаляемого узла
   Item* p, * v;
   if (key < rot->key) {
      rot->left = rRemove(rot->left, key, dt);
   }
   else if (key > rot->key) {
      rot->right = rRemove(rot->right, key, dt);
   }
   else {
      if (dt != rot->data)
      {
         if (rot->right && rot->right->key == key)
         {
            rot->right = rRemove(rot->right, key, dt);
         }
         if (rot->left && rot->left->key == key)
         {
            rot->left = rRemove(rot->left, key, dt);
         }
      }
      else
      {
         //если один потомок
         if (rot->right == nullptr) {
            rot = rot->left;
         }
         else if (rot->left == nullptr) {
            rot = rot->right;
         }
         else {
            //у левого узла 2 потомка
            v = rot->left;
            //у левого узла есть правый потомок
            if (v->right) {
               //поиск самого большого
               while (v->right->right) {
                  v = v->right;
               }
               //меняем найденное значение с корнем
               rot->key = v->right->key;
               rot->data = v->right->data;
               p = v->right; //удаляемый элемент
               v->right = v->right->left;
            }
            else {
               //у левого потомка нет правого поддерева
               rot->data = v->data;
               rot->key = v->key;
               p = v;
               rot->left = rot->left->left;
            }
         }
      }
      
   }
   return rot;
}

void Tree::rToArray(Item* rot, vector<string>& arr)
{
   if (rot)
   {
      arr.push_back(rot->key);
      arr.push_back(rot->data);
      rToArray(rot->left, arr);
      rToArray(rot->right, arr);
   }
}

void Tree::rFindByKey(Item* rot, const string key, string& value)
{
   if (rot)
   {
      if (rot->key == key)
      {
         value = rot->data;
      }
      else
      {
         rFindByKey(rot->right, key, value);
         rFindByKey(rot->left, key, value);
      }
   }
}

void Tree::addElem(const string key, const string dt)
{
   if (root)
   {
      rAddElem(root, key, dt);
   }
   else
   {
      root = create(key, dt);
   }
}

string Tree::toString()
{
   string strTree = "\n";
   if (root)
   {
      rToString("", root, false, (root->right && root->left), strTree);
   }
   else
   {
      strTree += "[toString] Your tree is empty\n";
   }
   return strTree;
}

bool Tree::include(const string key, const string dt)
{
   if (root)
   {
      return rInclude(root, key, dt);
   }
   else
   {
      return false;
   }
}

void Tree::remove(const string key, const string dt)
{
   if (root) {
      root = rRemove(root, key, dt);
   }
}

vector<string> Tree::toStringVector()
{
   vector<string> arr;
   if (root)
   {
      rToArray(root, arr);
   }
   return arr;
}

const string Tree::findByKey(const string key)
{
   if (root)
   {
      string value = "not found";
      rFindByKey(root, key, value);
      return value;
   }
   else
   {
      return "this tree is empty";
   }
}

