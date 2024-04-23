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
   Tree::Item* create(const int);  //Создание нового звена
   Tree::Item* rAddElem(Item*, const int); //Добавление элемента
   const int rGetHeight(Item*);              //Получение высоты поддеревьев
   void rPrint(const string&, Item*, bool, bool);//Печать элементов поддеревьев
   void rFillBranchesNum(Item*, Item*, int); //Заполнение нового дерева ветвями содержащими число
   int rInclude(Item*, int);  //Содержит ли ветвь число
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
   void addElem(const int);   //Добавление элемента в дерево
   void print();              //Печать всего дерева
   const int getHeight();     //Получение высоты дерева
   void printBranchesNum(const int);   //Печать ветвей с максимальной длиной
   bool include(const int);   //Содержит ли дерево число

};

int main()
{
   //Создание дин массива (чтение из файла)
   int* simpleArray = nullptr;
   int size = fileIntArray("input.txt", simpleArray);
   for (auto i = 0; i < size; i++)
   {
      cout << simpleArray[i] << " ";
   }
   cout << endl << "Array from tree completed!" << endl;
   //Создание дерева из дин массива
   Tree newTree(simpleArray, size);
   //Добавление элементов через консоль
   cout << "Fill elements (-1 to stop)" << endl;
   while (true)
   {
      int inInt = 0;
      inInt = consoleInt();
      if (inInt == -1)
      {
         break;
      }
      newTree.addElem(inInt);
   }
   //Вывод
   cout << "Tree Height: " << newTree.getHeight() << endl;
   newTree.print();
   //Вывод по заданию
   cout << "Enter a num to find" << endl;
   int inInt = consoleInt();
   newTree.printBranchesNum(inInt);
   //Проверка что исходное дерево не изменилось
   newTree.print();

   //
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


void Tree::rPrint(const string& prefix, Item* rot, bool isLeft, bool notAlone)
{
   if (rot)
   {
      std::cout << prefix << ((isLeft && notAlone) ? "|--" : "`--") << rot->data << endl;
      rPrint(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->left, true, (rot->right && rot->left));
      rPrint(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->right, false, (rot->right && rot->left));
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

const int Tree::getHeight()
{
   if (root)
   {
      return 1 + max(rGetHeight(root->right), rGetHeight(root->left));
   }
   else
   {
      return 0;
   }
}

void Tree::printBranchesNum(const int num)
{
   if (root != nullptr && include(num))
   {
      Item* result = create(root->data);
      rFillBranchesNum(root, result, num);
      //Вывод дерева
      cout << "===Your tree===" << endl;
      rPrint("", result, false, (result->right && result->left));
      cout << endl;

   }
   else
   {
      cout << "[printBranchesNum] Your tree is empty" << endl;
   }
}

bool Tree::include(const int num)
{
   return (rInclude(root, num) == 1);
}

void Tree::rFillBranchesNum(Tree::Item* rot, Tree::Item* item, int num)
{
   if (rot != nullptr)
   {
      if (rInclude(rot->left, num) == 1)
      {
         item->left = create(rot->left->data);
         rFillBranchesNum(rot->left, item->left, num);
      }
      if (rInclude(rot->right, num) == 1)
      {
         item->right = create(rot->right->data);
         rFillBranchesNum(rot->right, item->right, num);
      }
   }
   else
   {
      return;
   }
}

int Tree::rInclude(Item* rot, int num)
{
   if (rot == nullptr)
   {
      return -1;
   }
   if (rot->data == num)
   {
      return 1;
   }
   int result = -1;
   if (rot->right != nullptr)
   {
      result = rInclude(rot->right, num);
   }
   if (result < 0 && rot->left != nullptr)
   {
      result = rInclude(rot->left, num);
   }
   return result;
}