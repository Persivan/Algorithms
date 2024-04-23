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
   Tree::Item* create(const int);                     //Создание нового звена
   Tree::Item* rAddElem(Item*, const int);            //Автоматические добавление элемента
   Tree::Item* rAddElemWithChoise(Item*, const int);  //Добавление элемента с выбором куда
   void rPrint(const string&, Item*, bool, bool);     //Печать элементов поддеревьев
   const int rGetHeight(Item*);                       //Получение высоты поддеревьев

   void rFillLongestIncreasingBranch(Item*, Item*&, int&); //Создать самую длинную ветку с элементами по возрастанию
   int rGetIncreasedHeight(Item*);                    //Получить высоту ветки по возрастанию.
   void rDeleteNotIncreasingBranches(Item*&);         //Удаляет ветки не по возрастанию
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

   void printLongestIncreasingBranch(); //Печать самой длинной ветки с элементами по возрастанию.
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
   //Применение функции
   newTree.printLongestIncreasingBranch();
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

void Tree::rFillLongestIncreasingBranch(Item* rot, Item*& result, int& maxlength)
{
   if (rot != nullptr)
   {
      //Максимальная длина меньше, чем длина ветки которая поступила
      if (maxlength < rGetIncreasedHeight(rot))
      {
         result = rot;
         maxlength = rGetIncreasedHeight(rot);
         rDeleteNotIncreasingBranches(result);
      }
      rFillLongestIncreasingBranch(rot->left, result, maxlength);
      rFillLongestIncreasingBranch(rot->right, result, maxlength);
   }
}

int Tree::rGetIncreasedHeight(Item* rot)
{
   if (rot)
   {
      if (rot->left != nullptr && rot->left->data > rot->data)
      {
         return 1 + max(rGetIncreasedHeight(rot->right), rGetIncreasedHeight(rot->left));
      }
      if (rot->right != nullptr && rot->right->data > rot->data)
      {
         return 1 + max(rGetIncreasedHeight(rot->right), rGetIncreasedHeight(rot->left));
      }
      return 0;
   }
   else
   {
      return 0;
   }
}

void Tree::rDeleteNotIncreasingBranches(Item*& rot)
{
   if (rot)
   {
      int leftIncrdHeight = rGetIncreasedHeight(rot->left);
      int rightIncrdHeight = rGetIncreasedHeight(rot->right);
      //
      if (leftIncrdHeight > rightIncrdHeight)
      {
         //Удаляем правую если она не лист или меньше чем корень
         if (!rot->right->isLeaf() || (rot->right && rot->right->data < rot->data))
         {
            rot->right = nullptr;
         }
         rDeleteNotIncreasingBranches(rot->left);
      }
      //
      else if (leftIncrdHeight < rightIncrdHeight)
      {
         //Удаляем левую если она не лист или меньше чем корень
         if (!rot->left->isLeaf() || (rot->left && rot->left->data < rot->data))
         {
            rot->left = nullptr;
         }
         rDeleteNotIncreasingBranches(rot->right);
      }
      else if (leftIncrdHeight == rightIncrdHeight)
      {
         //Удаляем правую если она не лист или меньше чем корень
         if (!rot->right->isLeaf() || (rot->right && rot->right->data < rot->data))
         {
            rot->right = nullptr;
         }
         //Удаляем левую если она не лист или меньше чем корень
         if (!rot->left->isLeaf() || (rot->left && rot->left->data < rot->data))
         {
            rot->left = nullptr;
         }
      }
      else
      {
         rDeleteNotIncreasingBranches(rot->left);
         rDeleteNotIncreasingBranches(rot->right);
      }
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

void Tree::printLongestIncreasingBranch()
{
   if (root != nullptr)
   {
      Item* result = nullptr;
      int maxLength = 0;
      rFillLongestIncreasingBranch(root, result, maxLength);
      cout << "===Your Branch===" << endl;
      rPrint("", result, false, (root->right && root->left));
      cout << endl;
   }
   else
   {
      cout << "[printLongestIncreasingBranch] Your tree is empty" << endl;
   }
}

