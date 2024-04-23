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
   else
   {
      cout << "File not found!" << endl;
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
      const bool isLeaf()
      {
         if (this)
         {
            bool flag = false;
            if (left)
            {
               flag = true;
            }
            if (right)
            {
               flag = true;
            }
            return !flag;
         }
      }
   };
   Item* root;                //Корень дерева
   Tree::Item* create(const int);               //Создание нового звена
   Tree::Item* rAddElem(Item*, const int);      //Автоматическое добавление элемента
   Tree::Item* rAddElemWithChoise(Item*, const int); //Добавление элемента с выбором куда
   void rPrint(const string&, Item*, bool, bool);//Печать элементов поддеревьев
   bool rInclude(Item*, const int);             //Содержит ли число
   Tree::Item* rRemove(Item*, const int);       //Удаляет элемент по числу

   void rHasOddAndEvenNodes(Item*, bool&, bool&, int);//Есть ли у ветки и четные и нечетные вершины
   void rPrintBranches(Item*, const int);       //Печать веток по заданию
public:
   //Конструкторы
   /*
   * Первый создает пустое дерево
   * Второй создает дерево с корнем dt
   */
   Tree() : root(nullptr)
   {
   };
   Tree(const int dt) : root(create(dt))
   {
   };
   //Методы использующие рекурсивные методы
   void addElem(const int);   //Добавление элемента в дерево
   void addElemWithChoise(const int); //Добавление элемента с выбором куда
   void print();              //Печать всего дерева
   bool include(const int);   //Содержит ли ключ
   void remove(const int);    //Удаляет элемент по ключу

   void printBranches();      //Выводит поддеревья вершины которых имеют номера: нечётные/четные
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
   newTree.printBranches();
   //Вывод исходное, чтобы показать что оно не изменилось
   newTree.print();
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
   //Если звено на данном моменте рекурсии не пустое
   if (rot)
   {
      //Вывод куда что 
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
      //Выбор ветки
      cout << "Choose branch:" << endl;
      int inputInt = consoleInt();
      while (inputInt < 0 || inputInt > 1)
      {
         cout << "There's only " << 2 << " branches. Try again!" << endl;
         inputInt = consoleInt();
      }
      //Переход к след моменту рекурсии
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
   //Если пустое то создаем там элемент
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
   if (root == nullptr)
   {
      //если дерево пустое
      return root;
   }
   //поиск удаляемого узла
   Item* p, * v;
   if (value < root->data)
   {
      root->left = rRemove(root->left, value);
   }
   else if (value > root->data)
   {
      root->right = rRemove(root->right, value);
   }
   else
   {
      //если один потомок
      if (root->right == nullptr)
      {
         root = root->left;
      }
      else if (root->left == nullptr)
      {
         root = root->right;
      }
      else
      {
         //у левого узла 2 потомка
         v = root->left;
         //у левого узла есть правый потомок
         if (v->right)
         {
            //поиск самого большого
            while (v->right->right)
            {
               v = v->right;
            }
            //меняем найденное значение с корнем
            root->data = v->right->data;
            p = v->right; //удаляемый элемент
            v->right = v->right->left;
         }
         else
         {
            //у левого потомка нет правого поддерева
            root->data = v->data;
            p = v;
            root->left = root->left->left;
         }
      }
   }
   return root;
}

void Tree::rPrintBranches(Item* rot, const int depth)
{
   // Если пустой элемент или лист
   if (!rot || rot->isLeaf())
   {
      return;
   }
   // Проверка является ли подходящим для нас,
   // создадим переменные
   bool hasOdd = false;
   bool hadEven = false;
   // заполним для текущей ветки
   rHasOddAndEvenNodes(rot, hasOdd, hadEven, 0);
   // если все вершины нечетные - выводим
   if (hasOdd && !hadEven)
   {
      cout << "=Good branch (only odd leafs)=" << endl;
      rPrint("", rot, false, (rot->right && rot->left));
   }
   // если все вершины четные - выводим
   if (!hasOdd && hadEven)
   {
      cout << "=Good branch (only even leafs)=" << endl;
      rPrint("", rot, false, (rot->right && rot->left));
   }
   // Вызов у потомков
   rPrintBranches(rot->left, depth + 1);
   rPrintBranches(rot->right, depth + 1);
}

void Tree::rHasOddAndEvenNodes(Item* rot, bool& hasOddLeaf, bool& hasEvenLeaf, int depth)
{
   // Если пустой элемент
   // Если при обращение к текущему элементу уже были и четные и нечетные вершины
   if (!rot || (hasOddLeaf && hasEvenLeaf))
   {
      return;
   }
   // Если это лист, то узнаем на какой он глубине
   // если он на четной глубине то делаем тру для hasOddleaf, аналогично для нечетной
   if (rot->isLeaf())
   {
      if (depth % 2 == 1)
      {
         hasOddLeaf = true;
      }
      else
      {
         hasEvenLeaf = true;
      }
   }
   rHasOddAndEvenNodes(rot->left, hasOddLeaf, hasEvenLeaf, depth + 1);
   rHasOddAndEvenNodes(rot->right, hasOddLeaf, hasEvenLeaf, depth + 1);
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
   if (root)
   {
      root = rRemove(root, value);
   }
}

void Tree::printBranches()
{
   if (root)
   {
      cout << "===Branches===" << endl;
      rPrintBranches(root, 0);
   }
   else
   {
      cout << "[printBranches] Your tree is empty" << endl;
   }
}

