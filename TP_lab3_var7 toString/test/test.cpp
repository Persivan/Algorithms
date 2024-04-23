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
   void rToString(const string&, Item*, bool, bool, string&);//Печать элементов поддеревьев
   bool rInclude(Item*, const int);    //Содержит ли число
   Tree::Item* rRemove(Item*, const int);    //Удаляет элемент по числу
   const int rGetHeight(Item*);                    //Получение высоты поддеревьев (вовзращая число)

   const int rGetHeightToFirstLeaf(Item*);       // Получение высоты поддеревьев (вовзращая число)
   void rPrintMiddle(Item*, int);      //Построить дерево из веток с максимальным количеством ветвлений

   int printRange(Item* root, int index, int N1, int N2);

   void printLevelCounts(Item* root, int level);
   void printNodeCounts(Item* root);

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
   string toString();         // Печать всего дерева
   bool include(const int);   // Содержит ли ключ
   void remove(const int);    // Удаляет элемент по ключу

   void printMiddle();                       //Печать элементов равноудаленных от корня и листов

   void printDiff(int, int); // Запускающая функция

   void printNodeCountsWrapper();
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
   cout << newTree.toString();
   // Функция по заданию
   newTree.printNodeCountsWrapper();
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

void Tree::rToString(const string& prefix, Item* rot, bool isLeft, bool notAlone, string& strTree)
{
   if (rot)
   {
      strTree += prefix + ((isLeft && notAlone) ? "|--" : "`--") + to_string(rot->data) + "\n";
      rToString(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->left, true, (rot->right && rot->left), strTree);
      rToString(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->right, false, (rot->right && rot->left), strTree);
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

const int Tree::rGetHeight(Item* rot)
{
   if (rot == nullptr)
   {
      return 0;
   }
   return 1 + max(rGetHeight(rot->left), rGetHeight(rot->right));
}

const int Tree::rGetHeightToFirstLeaf(Item* rot)
{
    if (rot == nullptr)
    {
        return 0;
    }
    return 1 + min(rGetHeight(rot->left), rGetHeight(rot->right));
}

void Tree::rPrintMiddle(Item* rot, int depth)
{

        if (rot != nullptr)
        {
            if (rGetHeightToFirstLeaf(rot) == depth)
            {
                cout << rot->data << " ";
            }
            depth++;
            rPrintMiddle(rot->left, depth);
            rPrintMiddle(rot->right, depth);
        }
    
}

int Tree::printRange(Item* root, int index, int N1, int N2)
{
    // Base case: if the root is null, return the current index
    if (root == nullptr) return index;

    // If the current index is within the range N1 to N2, print the value
    // of the root
    if (index >= N1 && index <= N2) cout << root->data << " ";

    // Recursive case: update the index and check the left and right subtrees
    int leftIndex = printRange(root->left, index + 1, N1, N2);
    int rightIndex = printRange(root->right, leftIndex, N1, N2);

    // Return the rightmost index after traversing both subtrees
    return rightIndex;
}

void Tree::printLevelCounts(Item* root, int level)
{
    // Base case: if the root is null, do nothing
    if (root == nullptr) return;

    // If the level is 1, print the value of the root
    if (level == 1) cout << root->data << " ";

    // Recursive case: check the left and right subtrees at the next level
    // (level - 1)
    printLevelCounts(root->left, level - 1);
    printLevelCounts(root->right, level - 1);
}

void Tree::printNodeCounts(Item* root)
{
    // Get the height of the tree
    int height = rGetHeight(root);

    // Iterate over the levels of the tree, from the top level down
    for (int i = height; i >= 1; i--) {
        // Print the number of nodes at each level
        cout << "Level " << i << ": ";
        printLevelCounts(root, i);
        cout << endl;
    }
}

void Tree::printDiff(int N1, int N2)
{
    // Защита что дерево не пустое, выход если пустое (return)
    if (!root)
    {
        cout << "Your tree is epmty!";
        return;
    }
    int index = 0;
    printRange(root, 0, N1, N2);
}

void Tree::printNodeCountsWrapper()
{
    printNodeCounts(root);
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

void Tree::printMiddle()
{
    cout << "===Your elements===" << endl;
    if (root != nullptr)
    {
        rPrintMiddle(root, 1);
        cout << endl;
    }
    else
    {
        cout << "[printMiddle] Your tree is empty" << endl;
    }
}