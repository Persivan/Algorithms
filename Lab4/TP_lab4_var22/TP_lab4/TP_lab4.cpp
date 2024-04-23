#include<iostream>
#include<string>
#include<fstream>

#define N 5 //N дерево

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


struct Item
{
   int data;
   Item* children[N];   //Массив ветвей размера N
   Item(const int dt)   //Конструктор элемента
   {
      data = dt;
      for (auto i = 0; i < N; i++)
      {
         children[i] = nullptr;
      }
   }
   ~Item()              //Деструктор
   {
      for (int i = 0; i < N && children[i]; i++)
      {
         delete children[i];
      }
   }
   bool isLeaf()
   {
      bool flag = true;
      for (auto i = 0; i < N; i++)
      {
         if (children[i])
         {
            flag = false;
         }
      }
      return flag;
   }
};


class myNTree
{
private:
   Item* root;                               //Начало дерева
   //Рекурсивные функции для работы с деревом
   Item* rAddElem(Item*, const int&, unsigned int&, int, bool&); //Добавление элемента
   const int rGetHeight(Item*);              //Получение высоты поддеревьев
   void rPrint(const string&, Item*, bool, unsigned int, bool);  //Печать элементов поддеревьев
   void rCountElemsOnLvls(Item*, int*, int); //Подсчет элементов поддеревьев
   void rCountParentsData(Item*, int);       //Посчитать сумму значений предков
public:
   //Конструкторы
   /*
   * 1. Создает пустое дерево
   * 2. С корнем dt
   * 3. С веткой rot
   * 4. Из динам. массива и количества элементов массива
   */
   myNTree() : root(nullptr) {};
   myNTree(const int dt) : root(new Item(dt)) {};
   myNTree(Item* rot) : root(rot) {};
   myNTree(int*, const unsigned int);
   //Методы использующие рекурсивные методы
   void addElem(const int);                  //Добавление элемента в дерево
   void print();                             //Печать всего дерева
   const int getHeight();                    //Получение высоты дерева
   void printCountElemsOnLvls();             //Печать количества элементов на каждом уровне
   void CountParentsData();                  //Перестраивает дерево таким образом, что каждый лист хранит сумму его предков
};


int main()
{
   //-----------------------------------
   //-----Создадим пустое дерево--------
   //-----------------------------------
   myNTree nTreeExample1;
   nTreeExample1.print();
   cout << "Height = " << nTreeExample1.getHeight() << endl;
   nTreeExample1.printCountElemsOnLvls();
   cout << endl << endl;

   //-----------------------------------
   //-----оздадим дерево с корнем-------
   //-----------------------------------
   myNTree nTreeExample2(10);
   nTreeExample2.print();
   cout << "Height = " << nTreeExample2.getHeight() << endl;
   nTreeExample2.printCountElemsOnLvls();
   cout << endl << endl;

   //-----------------------------------
   //------Создадим дерево из ветки-----
   //-----------------------------------
   Item* vetka = new Item(3);
   vetka->children[0] = new Item(5);
   vetka->children[1] = new Item(1);
   vetka->children[2] = new Item(2);
   vetka->children[3] = new Item(10);
   vetka->children[4] = new Item(4);
   vetka->children[4]->children[3] = new Item(3);
   myNTree nTreeExample3(vetka);
   nTreeExample3.print();
   cout << "Height = " << nTreeExample3.getHeight() << endl;
   nTreeExample3.printCountElemsOnLvls();
   nTreeExample3.CountParentsData();
   nTreeExample3.print();
   cout << endl << endl;


   //-----------------------------------
   //-----Создадим дерево из массива----
   //-----------------------------------
   int* simpleArray = nullptr;
   int size = Tools::fileIntArray("war_peace.txt", simpleArray, 0, 0);
   cout << "Array filled!" << endl;
   myNTree nTreeExample4(simpleArray, size);
   nTreeExample4.print();
   cout << "Height = " << nTreeExample4.getHeight() << endl;
   nTreeExample4.printCountElemsOnLvls();
   cout << endl << endl;


   //Проверка методов
   nTreeExample4.addElem(10);
   cout << "May be add some element? (-1 to stop)" << endl;
   while (true)
   {
      int inInt = Tools::consoleInt();
      if (inInt == -1)
      {
         break;
      }
      nTreeExample4.addElem(inInt);
   }
   nTreeExample4.addElem(Tools::consoleInt());
   cout << "I want to add from input.txt first number (" << Tools::fileInt("input.txt", 0) << ")" << endl;
   nTreeExample4.addElem(Tools::fileInt("input.txt", 1));
   cout << "Added some elements" << endl;
   nTreeExample4.print();
   cout << "Height = " << nTreeExample4.getHeight() << endl;
   nTreeExample4.printCountElemsOnLvls();
   cout << "Made my super function" << endl;
   nTreeExample4.CountParentsData();
   nTreeExample4.print();
   cout << endl << endl;

   return 0;
}


Item* myNTree::rAddElem(Item* rot, const int& dt, unsigned int& lvlToFill, int currLvl, bool& flag)
{
   if (currLvl <= lvlToFill)
   {
      if (!flag)
      {
         if (rot)
         {
            for (auto i = 0; i < N; i++)
            {
               rot->children[i] = rAddElem(rot->children[i], dt, lvlToFill, ++currLvl, flag);
               currLvl--;
            }
         }
         else
         {
            rot = new Item(dt);
            flag = true;
         }
      }
   }
   return rot;
}

const int myNTree::rGetHeight(Item* rot)
{
   if (rot)
   {
      int rotMax = 0;
      for (auto i = 0; i < N; i++)
      {
         rotMax = max(rotMax, rGetHeight(rot->children[i]));
      }
      return 1 + rotMax;
   }
   else
   {
      return 0;
   }
}

void myNTree::rPrint(const string& prefix, Item* rot, bool isLast, unsigned int childIndex, bool isAlone)
{
   if (rot)
   {
      std::cout << prefix << (isLast ? "`--" : "|--") << rot->data << endl;
      for (auto i = 0; i < N ; i++)
      {
         char tIsAloneCount = 0;
         for (auto i = 0; i < N; i++)
         {
            if (rot->children[i])
            {
               tIsAloneCount++;
            }
         }
         bool tIsAlone = true;
         if (tIsAloneCount > 1)
         {
            tIsAlone = false;
         }
         bool tIsLast = true;
         for (auto j = i; j < N - 1; j++)
         {
            if (rot->children[j + 1])
            {
               tIsLast = false;
               break;
            }
         }
         rPrint(prefix + (isLast? "    " : "|   "), rot->children[i], tIsLast, i, tIsAlone);
      }
   }
}

void myNTree::rCountElemsOnLvls(Item* rot, int* lvls, int currLvl)
{
   if (rot)
   {
      lvls[currLvl]++;
      currLvl++;
      for (auto i = 0; i < N; i++)
      {
         rCountElemsOnLvls(rot->children[i], lvls, currLvl);
      }
   }
}

void myNTree::rCountParentsData(Item* rot, int sum)
{
   if (!rot->isLeaf())
   {
      for (auto i = 0; i < N; i++)
      {
         if (rot->children[i])
         {
            rot->data = sum;
            rCountParentsData(rot->children[i], rot->data + rot->children[i]->data);
         }
      }
   }
   else
   {
      rot->data = sum;
   }
}


myNTree::myNTree(int* arr, const unsigned int length)
{
   root = nullptr;
   int count = 0;
   while (count < length)
   {
      addElem(arr[count++]);
   }
}

void myNTree::addElem(const int dt)
{
   if (root)
   {
      int* lvls = new int[getHeight()];
      for (auto i = 0; i < getHeight(); i++)
      {
         lvls[i] = 0;
      }
      rCountElemsOnLvls(root, lvls, 0);
      int* lvlsIsFull = new int[getHeight()];
      for (auto i = 0; i < getHeight(); i++)
      {
         if (lvls[i] == pow(N, i))
         {
            lvlsIsFull[i] = true;
         }
         else
         {
            lvlsIsFull[i] = false;
         }
      }
      unsigned int lvlToFill = 0;
      while (lvlsIsFull[lvlToFill])
      {
         lvlToFill++;
      }
      bool flag = 0;
      rAddElem(root, dt, lvlToFill, 0, flag);
   }
   else
   {
      root = new Item(dt);
   }
}

void myNTree::print()
{
   cout << "===Your tree===" << endl;
   if (root)
   {
      rPrint("", root, true, 0, 0);
      cout << endl;
   }
   else
   {
      cout << "Your tree is empty" << endl;
   }
}

const int myNTree::getHeight()
{
   if (root)
   {
      return rGetHeight(root);
   }
   else
   {
      return 0;
   }
}

void myNTree::printCountElemsOnLvls()
{
   cout << "===Elements on every lvl===" << endl;
   if (root)
   {
      int* lvls = new int[getHeight()];
      for (auto i = 0; i < getHeight(); i++)
      {
         lvls[i] = 0;
      }
      rCountElemsOnLvls(root, lvls, 0);
      for (auto i = 0; i < getHeight(); i++)
      {
         cout << "Lvl " << i << ": " << lvls[i] << endl;
      }
      delete[] lvls;
   }
   else
   {
      cout << "Your tree is empty" << endl;
   }
}

void myNTree::CountParentsData()
{
   if (root)
   {
      rCountParentsData(root, root->data);
   }
}

