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
            cout << "Error: " << ex.what() << endl;
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
   Tree::Item* create(const int);            //Создание нового звена
   Tree::Item* rAddElem(Item*, const int);   //Добавление элемента
   Tree::Item* rAddElemWithChoise(Item*, const int); //Добавление элемента с выбором куда
   const int rGetHeight(Item*);              //Получение высоты поддеревьев
   void rToString(const string&, Item*, bool, bool, string&);//Печать элементов поддеревьев
   
   int rCount(Item*, int);                   // Возвращает количество указанного значения
   void rFindMin(Item*, int&, int);          // Находит мин значение в дереве больше указанного
   void rPrintCounts(Item*, int, bool);      // Выводит на экран количество каждого значения в дереве 
                                             // третьим параметром идет выбор показывать ли элементы количество которых равно 1
public:
   //Конструкторы
   /*
   * Первый создает пустое дерево
   * Второй создает дерево с корнем dt
   * Третий создает дерево из динам. массива чисел и размера массива
   */
   Tree() : root(nullptr) {};
   Tree(const int dt) : root(create(dt)) {};
   Tree(int*, const unsigned int);
   //Методы использующие рекурсивные методы
   void addElem(const int);                  //Добавление элемента в дерево
   void addElemWithChoise(const int);        //Добавление элемента с выбором куда
   string toString();                        //Печать всего дерева
   int getHeight();                          // Высота дерева

   void printCounts(bool);                   //Печать количества каждого значения
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
   // Вывод
   cout << newTree.toString();
   newTree.getHeight();
   //Вывод по заданию (в скобках 0 - не выводить элементы количество которых равно 1, 1 - выводить)
   newTree.printCounts(0);
   //Проверка что исходное дерево не изменилось
   cout << newTree.toString();
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

const int Tree::rGetHeight(Item* rot)
{
   if (rot)
   {
      return 1 + max(rGetHeight(rot->right), rGetHeight(rot->left));
   }
   else
   {
      return 0;
   }
}


void Tree::rToString(const string& prefix, Item* rot, bool isLeft, bool notAlone, string& strTree)
{
   if (rot)
   {
      strTree += prefix + ((isLeft && notAlone) ? "|--" : "`--") + to_string(rot->data) + "\n";
      rToString(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->left, true, (rot->right && rot->left), strTree);
      rToString(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->right, false, (rot->right && rot->left), strTree);
   }
}

Tree::Tree(int* arr, const unsigned int length)
{
   root = nullptr;
   int count = 0;
   while (count < length)
   {
      addElem(arr[count++]);
   }
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

string Tree::toString()
{
   string strTree = "";
   if (root)
   {
     
      strTree += "===Your tree===\n";
      rToString("", root, false, (root->right && root->left), strTree);
   }
   else
   {
      strTree += "[print] Your tree is empty";
   }
   return strTree;
}

int Tree::getHeight()
{
   return rGetHeight(root);
}


void Tree::printCounts(bool showOnes)
{
   if (!root)
   {
      cout << "[printCounts] Your tree is empty" << endl;
      return;
   }
   // Находим минимальное значение в дереве
   int min = INT_MAX;
   rFindMin(root, min, INT_MIN);
   rFindMin(root, min, INT_MIN);
   // Начинаем выводить количество
   rPrintCounts(root, min, showOnes);
}

int Tree::rCount(Item* rot, int num)
{
   // Если тек. эл. пустой
   if (!rot)
   {
      return 0;
   }
   // Если текущий элемент равен указанному, то в рекурсивном вызове добавляем 1
   if (rot->data == num)
   {
      return 1 + rCount(rot->left, num) + rCount(rot->right, num);
   }
   else
   {
      return 0 + rCount(rot->left, num) + rCount(rot->right, num);
   }
}

void Tree::rFindMin(Item* rot, int& min, int bottomEdge)
{
   // Выходим если эл. пустой
   if (!rot)
   {
      return;
   }
   // Сравниваем значение тек элемента с мин
   if (rot->data < min && rot->data > bottomEdge)
   {
      min = rot->data;
   }
   // Переход к след рекурсии
   rFindMin(rot->left, min, bottomEdge);
   rFindMin(rot->right, min, bottomEdge);
}

void Tree::rPrintCounts(Item* rot, int currValue, bool showOne)
{
   // Если тек. эл. пустой или пытаемся вывести в currValue попал INT_MAX (значит больше нет элементов)
   if (!rot || currValue == INT_MAX)
   {
      return;
   }
   // Находим количество значений
   if (rCount(rot, currValue) != 1 || showOne)
   {
      cout << "[" << currValue << "]=" << rCount(rot, currValue) << endl;
   }
   // След рекурсия
   int nextValue = INT_MAX;
   rFindMin(rot, nextValue, currValue);
   rPrintCounts(rot, nextValue, showOne);
}
