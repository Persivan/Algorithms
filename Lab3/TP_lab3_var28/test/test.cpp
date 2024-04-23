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
      string color;           // Цвет звена
      Item* left;             //Левое звено
      Item* right;            //Правое звено
   };
   Item* root;                //Корень дерева
   Tree::Item* create(const int, string);                     //Создание нового звена
   Tree::Item* rAddElem(Item*, const int);            //Добавление элемента
   Tree::Item* rAddElemWithChoise(Item*, const int, string);  //Добавление элемента с выбором куда
   void rToString(const string&, Item*, bool, bool, string&);//Печать элементов поддеревьев
   bool isLeaf(Item* rot)           // Является ли листом
   {
      return rot && rot->left == nullptr && rot->right == nullptr;
   }

   void rGetHeight(Item*, string, int&);     // Получение высоты листьев указанного цвета
   void rGetFirstLeafHeight(Item*, string, int&);                // Получение высоты самого близкого к корню элемента указанного цвета
   void rDeleteLeafs(Item*, string, int, int);    // Удаляет листья на заданном уровне и указанного цвета
   bool rHasLeafsWithColor(Item*, string); //Есть ли в ветке листья указанного цвета
   void rDeleteLongestYellow();  // Удалить самые дальние желтые
public:
   //Конструкторы
   /*
   * Первый создает пустое дерево
   * Второй создает дерево с корнем dt
   */
   Tree() : root(nullptr) {};
   //Методы использующие рекурсивные методы
   void addElem(const int);   //Добавление элемента в дерево
   void addElemWithChoise(const int, string); //Добавление элемента с выбором куда
   string toString();                        //Печать всего дерева

   void magicFunc();             // Выполнение задания
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
         // Вводим цвет
         cout << "Enter a color (yelow, black, white)\n";
         string color = "";
         while (color != "yelow" && color != "black" && color != "white")
         {
            getline(cin, color);
         }
         newTree.addElemWithChoise(inInt, color);
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
   //Вывод по заданию
   newTree.magicFunc();
   cout << newTree.toString();
   return 0;
}

Tree::Item* Tree::create(const int dt, string color)
{
   Item* item = new Item;
   item->data = dt;
   item->color = color;
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
      // Выбираем случайныйй цвет
      string color = "";
      int random = rand() % 3;
      if (random == 0)
      {
         color = "yelow";
      }
      else if (random == 1)
      {
         color = "black";
      }
      else if (random == 2)
      {
         color = "white";
      }
      rot = create(dt, color);
   }
   return rot;
}

Tree::Item* Tree::rAddElemWithChoise(Item* rot, const int dt, string color)
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
         rot->left = rAddElemWithChoise(rot->left, dt, color);
      }
      else if (inputInt == 1)
      {
         rot->right = rAddElemWithChoise(rot->right, dt, color);
      }
      cout << dt << " going to " << inputInt << " branch" << endl;
   }
   else
   {
      rot = create(dt, color);
   }
   return rot;
}

void Tree::rToString(const string& prefix, Item* rot, bool isLeft, bool notAlone, string& strTree)
{
   if (rot)
   {
      strTree += prefix + ((isLeft && notAlone) ? "|--" : "`--") + to_string(rot->data) + " (" + rot->color + ")" + "\n";
      rToString(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->left, true, (rot->right && rot->left), strTree);
      rToString(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->right, false, (rot->right && rot->left), strTree);
   }
}

void Tree::rGetHeight(Item* rot, string color, int& counter)
{
   if (!rot)
   {
      return;
   }
   if (isLeaf(rot) && rot->color != color)
   {
      counter = 0;
      return;
   }
   int counterRight = counter + 1, counterLeft = counter + 1;
   rGetHeight(rot->left, color, counterLeft);
   rGetHeight(rot->right, color, counterRight);
   counter = max(counterLeft, counterRight);
}

void Tree::rGetFirstLeafHeight(Item* rot, string color, int& counter)
{
   if (!rot)
   {
      return;
   }
   if (isLeaf(rot) && rot->color != color)
   {
      counter = INT_MAX;
      return;
   }
   int counterRight = counter + 1, counterLeft = counter + 1;
   rGetHeight(rot->left, color, counterLeft);
   rGetHeight(rot->right, color, counterRight);
   counter = min(counterLeft, counterRight);
}

void Tree::rDeleteLeafs(Item* rot, string color, int height, int depth)
{
   if (!rot)
   {
      return;
   }
   if (isLeaf(rot->right) && rot->right->color == color && height == depth + 1)
   {
      cout << "Deleted leaf (yellow): " << rot->right->data << endl;
      rot->right->~Item();
      rot->right = nullptr;
   }
   if (isLeaf(rot->left) && rot->left->color == color && height == depth + 1)
   {
      cout << "Deleted leaf (yellow): " << rot->left->data << endl;
      rot->left->~Item();
      rot->left = nullptr;
   }
   depth++;
   rDeleteLeafs(rot->left, color, height, depth);
   rDeleteLeafs(rot->right, color, height, depth);
}

bool Tree::rHasLeafsWithColor(Item* rot, string color)
{
   if (!rot)
   {
      return false;
   }
   if (isLeaf(rot))
   {
      if (rot->color == color)
      {
         return true;
      }
      return false;
   }
   return rHasLeafsWithColor(rot->left, color) || rHasLeafsWithColor(rot->right, color);
   
}

void Tree::addElem(const int dt)
{
   if (root)
   {
      rAddElem(root, dt);
   }
   else
   {
      root = create(dt, "yelow");
   }
}

void Tree::addElemWithChoise(const int dt, string color)
{
   if (root)
   {
      rAddElemWithChoise(root, dt, color);
   }
   else
   {
      root = create(dt, color);
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

void Tree::magicFunc()
{
   if (rHasLeafsWithColor(root, "yelow"))
   {
      int height = 0;
      rGetHeight(root, "yelow", height);
      rDeleteLeafs(root, "yelow", height, 1);
      return;
   }
   if (rHasLeafsWithColor(root, "black"))
   {
      int height = 0;
      rGetFirstLeafHeight(root, "black", height);
      rDeleteLeafs(root, "black", height, 1);
      return;
   }
   if (rHasLeafsWithColor(root, "white"))
   {
      int height = 0;
      rGetFirstLeafHeight(root, "white", height);
      rDeleteLeafs(root, "white", height, 1);
      return;
   }
}
