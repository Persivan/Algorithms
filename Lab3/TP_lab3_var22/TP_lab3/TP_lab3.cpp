#include<iostream>
#include<string>
#include<fstream>

using namespace std;

class Tools
{
public:
   //Ввод из консоли с проверкой каждого числа на int
   static const int consoleInt()
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
            cout << "Wrong console input! Error: " << ex.what() << endl;
         }
      }
      return res;
   };
   //Ввод из файла filename (первой строки) с проверкой на int
   /*
   * Принимает название файла
   * Принимает цифру 0-2 количество информации меньше число меньше информации
   */
   static const int fileInt(const string filename, const int debugMode)
   {
      int res = 0;
      ifstream fin(filename);
      if (fin.is_open())
      {
         if (!fin.eof())
         {
            string instr = "";
            getline(fin, instr);
            try
            {
               res = stoi(instr);
            }
            catch (const std::exception& ex)
            {
               if (debugMode > 1)
               {
                  cout << "Wrong file input! Error: " << ex.what() << endl;
               }
            }
         }
      }
      else
      {
         if (debugMode > 0)
         {
            cout << "File read error" << endl;
         }
      }

      return res;
   }
   //Ввод из файла filename массива чисел с проверкой на int
   /*
   * Возвращает размер полученного массива
   * Принимает название файла
   * Принимат указатель куда записать массив чисел из файла
   * Принимает bool заменять ли необработанное на 0
   * Принимает цифру 0-2 количество информации меньше число меньше информации
   * p.s. функция специально сделана самостоятеьной
   */
   static const int fileIntArray(const string filename, int*& arr, const bool isNeedZero, const int debugMode)
   {
      unsigned int length = 0;
      if (arr)
      {
         if (debugMode > 0)
         {
            cout << "Warning, arr not empty! Return 0!" << endl;
         }
         return 0;
      }
      ifstream fin(filename);
      if (fin.is_open())
      {
         string instr = "";
         unsigned int arrLength = 10;
         arr = new int[arrLength];
         while (getline(fin, instr))
         {
            string oneWord = "";
            int intFromWord = 0;
            for (auto i = 0; i < instr.size(); i++)
            {
               bool flag = false;
               oneWord = "";
               intFromWord = 0;
               while (instr[i] == ' ' && i < instr.size())
               {
                  i++;
               }
               while (instr[i] != ' ' && i < instr.size())
               {
                  oneWord += instr[i++];
               }
               try
               {
                  intFromWord = stoi(oneWord);
                  flag = true;
               }
               catch (const std::exception& ex)
               {
                  if (isNeedZero)
                  {
                     flag = true;
                  }
                  if (debugMode > 1)
                  {
                     cout << "Wrong file input! Error: " << ex.what() << endl;
                  }
               }
               //Если length (количество слов) совпало с длиной массива
               if (flag)
               {
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
                  arr[length++] = intFromWord;
               }
            }
         }
      }
      else
      {
         if (debugMode > 0)
         {
            cout << "File read error" << endl;
         }
      }
      return length;
   }

};
class myTree
{
private:
   struct Item                //Структура, представляющая собой лист
   {
      int data;               //Значения листа
      Item* left;             //Левый лист
      Item* right;            //Правый лист
   };
   Item* root;                //Начало дерева
   myTree::Item* create(const int);  //Создание нового звена
   //Рекурсивные функции для работы с деревом
   myTree::Item* rAddElem(Item*, const int); //Добавление элемента
   const int rGetHeight(Item*);              //Получение высоты поддеревьев
   void rPrint(const string&, Item*, bool, bool);//Печать элементов поддеревьев
   void rCountElemsOnLvls(Item*, int*, int); //Подсчет элементов поддеревьев
public:
   //Конструкторы
   /*
   * Первый создает пустое дерево
   * Второй создает дерево с корнем dt
   * Третий создает дерево из динам. массива чисел и размера массива
   */
   myTree() : root(nullptr) {};
   myTree(const int dt) : root(create(dt)) {};
   myTree(int*, const unsigned int);
   //Методы использующие рекурсивные методы
   void addElem(const int);   //Добавление элемента в дерево
   void print();              //Печать всего дерева
   const int getHeight();     //Получение высоты дерева
   void printCountOnLvls();   //Печать количества элементов на каждом уровне
};


int main()
{
   int* simpleArray = nullptr;
   int size = Tools::fileIntArray("input.txt", simpleArray, 0, 1);
   for (auto i = 0; i < size; i++)
   {
      cout << simpleArray[i] << " ";
   }
   cout << endl << "Array from tree completed!" << endl;
   myTree newTree(simpleArray, size);
   cout << "Fill elements (-1 to stop)" << endl;
   while (true)
   {
      int inInt = 0;
      inInt = Tools::consoleInt();
      if (inInt == -1)
      {
         break;
      }
      newTree.addElem(inInt);
   }
   cout << "Tree Height: " << newTree.getHeight() << endl;
   newTree.print();
   newTree.printCountOnLvls();
   return 0;
}

myTree::Item* myTree::create(const int dt)
{
   Item* item = new Item;
   item->data = dt;
   item->left = nullptr;
   item->right = nullptr;
   return item;
}

myTree::Item* myTree::rAddElem(Item* rot, const int dt)
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

const int myTree::rGetHeight(Item* rot)
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


void myTree::rPrint(const string& prefix, Item* rot, bool isLeft, bool notAlone)
{
   if (rot)
   {
      std::cout << prefix << ((isLeft && notAlone) ? "|--" : "`--") << rot->data << endl;
      rPrint(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->left, true, (rot->right && rot->left));
      rPrint(prefix + ((isLeft && notAlone) ? "|   " : "    "), rot->right, false, (rot->right && rot->left));
   }
}

void myTree::rCountElemsOnLvls(Item* rot, int* lvls, int currLvl)
{
   if (rot)
   {
      lvls[currLvl]++;
      currLvl++;
      rCountElemsOnLvls(rot->left, lvls, currLvl);
      rCountElemsOnLvls(rot->right, lvls, currLvl);
   }
}

myTree::myTree(int* arr, const unsigned int length)
{
   root = nullptr;
   int count = 0;
   while (count < length)
   {
      addElem(arr[count++]);
   }
}

void myTree::addElem(const int dt)
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

void myTree::print()
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


const int myTree::getHeight()
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

void myTree::printCountOnLvls()
{
   if (root) 
   {
      int* lvls = new int[getHeight()];
      for (auto i = 0; i < getHeight(); i++)
      {
         lvls[i] = 0;
      }
      rCountElemsOnLvls(root, lvls, 0);
      cout << "===Elements on every lvl===" << endl;
      for (auto i = 0; i < getHeight(); i++)
      {
         cout << "Lvl " << i << ": " << lvls[i] << endl;
      }
      delete[] lvls;
   }
   else
   {
      cout << "[printCountElemsOnLvls] Your tree is empty" << endl;
   }
}