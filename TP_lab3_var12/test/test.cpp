#include<iostream>
#include<string>
#include<fstream>

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
         }
         catch (const std::exception& ex)
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
   return length;
}

class Tree
{
private:
   struct Item                //Структура звена
   {
      int data;               //Значения звена
      Item* left;             //Левое звено
      Item* right;            //Правое звено
   };
   Item* root;                //Корень дерева
   Tree::Item* create(const int);  //Создание нового звена
   Tree::Item* rAddElem(Item*, const int); //Добавление элемента
   Tree::Item* rAddElemWithChoise(Item*, const int); //Добавление элемента с выбором куда
   void rPrint(const string&, Item*, bool, bool);//Печать элементов поддеревьев
   bool rInclude(Item*, const int);    //Содержит ли число
   Tree::Item* rRemove(Item*, const int);    //Удаляет элемент по числу

   void rGetCountLeafs(Item*, int&);               //Посчитать количества листьев (сохраняя в переменную)
   const int rGetHeight(Item*);                    //Получение высоты поддеревьев (вовзращая число), типо и так и так умею
   void rGetMaxMinDelta(Item*, int&, int&, int&);  //Получение максимальной и минимальной высоты деревьев с заданным числом листьев
   void rPrintWithDelta(Item*, int&, int&, int&);  //Проход по всему дереву с печатью ветвей с макс мин
public:
   //Конструкторы
   /*
   * Первый создает пустое дерево
   * Второй создает дерево с корнем dt
   */
   Tree() : root(nullptr) {};
   Tree(const int dt) : root(create(dt)) {};
   //Методы использующие рекурсивные методы
   void addElem(const int);   //Добавление элемента в дерево
   void addElemWithChoise(const int); //Добавление элемента с выбором куда
   void print();              //Печать всего дерева
   bool include(const int);   //Содержит ли ключ
   void remove(const int);    //Удаляет элемент по ключу

   void printMaxMin(int&);    //Печать поддеревьев с макс и мин высотой дерева с заданным количеством листьев
};

int main()
{
   Tree newTree;
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
         newTree.addElemWithChoise(inInt);
      }
   }
   //Из файла
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
         newTree.addElem(simpleArray[i]);
      }
   }
   //Вывод полученного дерева
   newTree.print();
   //Применение функции по заданию
   cout << "Enter a leafs count to find: " << endl;
   int leafsCountFromUser = 0;
   while (true)
   {
      leafsCountFromUser = consoleInt();
      if (leafsCountFromUser < 1)
      {
         //Количество листьев должно быть больше 0. -1, -2 и т.д. понятно почему,
         //хз, стоит ли писать тут, для 0 не будет выведено ни одного поддерева
         //(функция не сломается если ввести 0 и даже если ввести отриц. число)
         //т.к. у листьев будет высота 1, а количество листьев тоже 1 (сам лист)!
         cout << "   should be more than 0 (> 0)" << endl;
      }
      else
      {
         break;
      }
   }
   newTree.printMaxMin(leafsCountFromUser);
   return 0;
}

Tree::Item* Tree::create(const int dt)
{
   Item* item = new Item;
   item->data = dt;
   item->left = nullptr;
   item->right = nullptr;
   return item;
}

Tree::Item* Tree::rAddElem(Item* rot, const int dt)
{
   if (rot)
   {
      if (rot->data >= dt)
      {
         rot->left = rAddElem(rot->left, dt);
      }
      else
      {
         rot->right = rAddElem(rot->right, dt);
      }
   }
   else
   {
      rot = create(dt);
   }
   return rot;
}

Tree::Item* Tree::rAddElemWithChoise(Item* rot, const int dt)
{
   if (rot)
   {
      cout << "0 - left branch, 1 - right branch" << endl;
      cout << "0" << ") ";
         if (rot->left)
         {
            cout << rot->left->data;
         }
         else
         {
            cout << "free";
         }
         cout << endl;
         cout << "1" << ") ";
         if (rot->right)
         {
            cout << rot->right->data;
         }
         else
         {
            cout << "free";
         }
         cout << endl;
      cout << "Choose branch:" << endl;
      int inputInt = consoleInt();
      while (inputInt < 0 || inputInt > 1)
      {
         cout << "There's only " << 2 << " branches. Try again!" << endl;
         inputInt = consoleInt();
      }
      if (inputInt == 0)
      {
         rot->left = rAddElemWithChoise(rot->left, dt);
      }
      else if (inputInt == 1)
      {
         rot->right = rAddElemWithChoise(rot->right, dt);
      }
      cout << dt << " going to " << inputInt << " branch" << endl;
   }
   else
   {
      rot = create(dt);
   }
   return rot;
}


void Tree::rPrint(const string& prefix, Item* rot, bool isLeft, bool notAlone)
{
   if (rot)
   {
      std::cout << prefix << ((isLeft && notAlone) ? "|--" : "`--") << rot->data << endl;
      rPrint(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->left, true, (rot->right && rot->left));
      rPrint(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->right, false, (rot->right && rot->left));
   }
}

bool Tree::rInclude(Item* rot, const int val)
{
   if (rot)
   {
      if (rot->data == val)
      {
         return true;
      }
      if (rInclude(rot->left, val))
      {
         return true;
      }
      if (rInclude(rot->right, val))
      {
         return true;
      }
   }
   else
   {
      return false;
   }
}

Tree::Item* Tree::rRemove(Item* root, const int value)
{
   if (root == nullptr) {
      //если дерево пустое
      return root;
   }
   //поиск удаляемого узла
   Item* p, * v;
   if (value < root->data) {
      root->left = rRemove(root->left, value);
   }
   else if (value > root->data) {
      root->right = rRemove(root->right, value);
   }
   else {
      //если один потомок
      if (root->right == nullptr) {
         root = root->left;
      }
      else if (root->left == nullptr) {
         root = root->right;
      }
      else {
         //у левого узла 2 потомка
         v = root->left;
         //у левого узла есть правый потомок
         if (v->right) {
            //поиск самого большого
            while (v->right->right) {
               v = v->right;
            }
            //меняем найденное значение с корнем
            root->data = v->right->data;
            p = v->right; //удаляемый элемент
            v->right = v->right->left;
         }
         else {
            //у левого потомка нет правого поддерева
            root->data = v->data;
            p = v;
            root->left = root->left->left;
         }
      }
   }
   return root;
}

void Tree::rGetCountLeafs(Item* rot, int& sum)
{
   if (rot == nullptr)
   {
      return;
   }
   //Проверка является ли листом
   if (rot->left == nullptr && rot->right == nullptr)
   {
      sum++;
   }
   //Рекурсивный вызов у потомков
   rGetCountLeafs(rot->left, sum);
   rGetCountLeafs(rot->right, sum);
}

const int Tree::rGetHeight(Item* rot)
{
   if (rot == nullptr)
   {
      return 0;
   }
   return 1 + max(rGetHeight(rot->left), rGetHeight(rot->right));
}

void Tree::rGetMaxMinDelta(Item* rot, int& resultMax, int& resultMin, int& leafsCount)
{
   //Текущий элемент пустой
   if (rot == nullptr)
   {
      return;
   }
   //Получение высоты и количества листьев
   int leafsCt = 0;
   rGetCountLeafs(rot, leafsCt);
   int height = rGetHeight(rot);
   //Если в этом поддереве нужное количество листьев
   if (leafsCt == leafsCount)
   {
      //И высота этого поддерева больше чем которая у нас, то запоминаем новый максимум
      if (height > resultMax)
      {
         resultMax = height;
      }
      //Соответсвенно, новый минимум
      if (height < resultMin)
      {
         resultMin = height;
      }
   }
   //Рекурсивный вызов той же функции у потомков
   rGetMaxMinDelta(rot->left, resultMax, resultMin, leafsCount);
   rGetMaxMinDelta(rot->right, resultMax, resultMin, leafsCount);
}

void Tree::rPrintWithDelta(Item* rot, int& max, int& min, int& leafsCount)
{
   //Текущий элемент не пустой
   if (rot == nullptr)
   {
      return;
   }
   //Получение высоты и количества листьев
   int leafsCt = 0;
   rGetCountLeafs(rot, leafsCt);
   int height = rGetHeight(rot);

   //Вывод соответствующих
   if (leafsCt == leafsCount)
   {
      if (height == max)
      {
         cout << "Max Branch" << endl;
         rPrint("", rot, true, 0);
      }
      if (height == min)
      {
         cout << "Min Branch" << endl;
         rPrint("", rot, true, 0);
      }
   }
   //Рекурсивный вызов для потомков
   rPrintWithDelta(rot->left, max, min, leafsCount);
   rPrintWithDelta(rot->right, max, min, leafsCount);
}

void Tree::addElem(const int dt)
{
   if (root)
   {
      rAddElem(root, dt);
   }
   else
   {
      root = create(dt);
   }
}

void Tree::addElemWithChoise(const int dt)
{
   if (root)
   {
      rAddElemWithChoise(root, dt);
   }
   else
   {
      root = create(dt);
   }
}

void Tree::print()
{
   if (root)
   {
      cout << "===Your tree===" << endl;
      rPrint("", root, false, (root->right && root->left));
      cout << endl;
   }
   else
   {
      cout << "[print] Your tree is empty" << endl;
   }
}

bool Tree::include(const int val)
{
   if (root)
   {
      return rInclude(root, val);
   }
   else
   {
      return false;
   }
}

void Tree::remove(const int value)
{
   if (root) {
      root = rRemove(root, value);
   }
}

void Tree::printMaxMin(int& leafsCount)
{
   cout << "===Max/Min trees===" << endl;
   if (root)
   {
      //Создаем две переменные, где будем хранить макс и мин высоту
      int max = 0;
      int min = INT_MAX;
      //Проходимся рекурсивно по дереву получая эти числа
      rGetMaxMinDelta(root, max, min, leafsCount);
      //Проходимся второй раз рекусрвино для вывода деревьей
      rPrintWithDelta(root, max, min, leafsCount);
      cout << endl;
   }
   else
   {
      cout << "Your tree is empty" << endl;
   }
}

