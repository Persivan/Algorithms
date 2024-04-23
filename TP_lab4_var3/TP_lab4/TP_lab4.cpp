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
         catch (const std::exception& ex)
         {
            cout << "Error: " << ex.what() << endl;
         }
      }
   }
   else
   {
      cout << "File not found" << endl;
   }
   return length;
}


struct Item
{
   int N;               //Количество потомков
   int data;            //Значение звена
   Item** children;     //Массив ветвей размера N
   Item(const int dt, int numOfChilds)   //Конструктор элемента
   {
      children = new Item * [numOfChilds];
      N = numOfChilds;
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
   bool isLeaf()        //Является ли листом элемент
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
   Item* root;                               //Корень дерева
   int N;                                    //Количество сыновей в дереве
   //Рекурсивные функции для работы с деревом
   Item* rAddElem(Item*, const int&, unsigned int&, int, bool&);  //Автоматическое добавление элемента
   Item* rAddElemWithChoice(Item*, const int&);                   //Добавление элемента
   void rPrint(const string&, Item*, bool, unsigned int, bool);   //Печать элементов поддеревьев
   void rCountElemsOnLvls(Item*, int*, int);                      //Подсчет элементов поддеревьев

   void rGetCountLeafs(Item*, int&);               //Посчитать количества листьев
   const int rGetHeight(Item*);                    //Получение высоты поддеревьев
   void rGetMaxMinDelta(Item*, float&, float&);    //Получение максимального и минимального соотношения высота/число
   void rPrintWithDelta(Item*, float&, float&);    //Проход по всему дереву с печатью ветвей с макс мин соотношением.
public:
   //Конструкторы
   /*
   * 1. Создает пустое дерево
   * 2. С корнем dt
   */
   myNTree(const int numOfChilds) : root(nullptr), N(numOfChilds) {};
   myNTree(const int dt, const int numOfChilds) : root(new Item(dt, numOfChilds)), N(numOfChilds) {};
   //Деструктор
   ~myNTree() { delete root; };
   //Методы использующие рекурсивные методы
   void addElem(const int);                  //Добавление элемента в дерево
   void addElemWithChoice(const int);        //Добавление элемента в дерево с интерфейсом выбора
   void print();                             //Печать всего дерева
   const int getHeight();                    //Получение высоты дерева
   const int getCountElems();                //Получение количества элементов дерева
   
   void printMaxMin();                       //Печать поддеревьев с макс и мин соотношением высота/число листьев
};


int main()
{
   cout << "Enter N for N-tree" << endl;
   int inputIntNTree = consoleInt();
   myNTree newTree(inputIntNTree);
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
         newTree.addElemWithChoice(inInt);
      }
   }
   else
   {
      int* simpleArray = nullptr;
      int size = fileIntArray("input.txt", simpleArray);
      cout << "Array filled!" << endl;
      for (auto i = 0; i < size; i++)
      {
         newTree.addElem(simpleArray[i]);
      }
   }
   //Вывод полученого дерева
   newTree.print();
   cout << "Count of elems = " << newTree.getCountElems() << endl;
   //Вывод по заданию
   newTree.printMaxMin();
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
            rot = new Item(dt, N);
            flag = true;
         }
      }
   }
   return rot;
}

Item* myNTree::rAddElemWithChoice(Item* rot, const int& dt)
{
   if (rot)
   {
      for (int i = 0; i < N; i++)
      {
         cout << i << ") ";
         if (rot->children[i])
         {
            cout << rot->children[i]->data;
         }
         else
         {
            cout << "free";
         }
         cout << endl;
      }
      cout << "Choose branch:" << endl;
      int inputInt = consoleInt();
      while (inputInt < 0 || inputInt > N - 1)
      {
         cout << "There's only " << N << " branches. Try again!" << endl;
         inputInt = consoleInt();
      }
      rot->children[inputInt] = rAddElemWithChoice(rot->children[inputInt], dt);
      cout << dt << " going to " << inputInt << " branch" << endl;
   }
   else
   {
      rot = new Item(dt, N);
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

void myNTree::rGetMaxMinDelta(Item* rot, float& resultMax, float& resultMin)
{
   if (rot && (!rot->isLeaf()))
   {
      //Получение высоты и количества листьев
      int leafsCount = 0;
      rGetCountLeafs(rot, leafsCount);
      int height = rGetHeight(rot);
      //Перевод этих чисел в float
      float leafsCountF = static_cast<float>(leafsCount);
      float heightF = static_cast<float>(height);
      //Высчитываем соотноешние
      float magicNum = heightF/leafsCountF;
      if (magicNum > resultMax)
      {
         resultMax = magicNum;
      }
      if (magicNum < resultMin)
      {
         resultMin = magicNum;
      }
      //Рекурсивный вызов той же функции у потомков
      for (int i = 0; i < N; i++)
      {
         rGetMaxMinDelta(rot->children[i], resultMax, resultMin);
      }
   }
}

void myNTree::rPrintWithDelta(Item* rot, float& max, float& min)
{
   if (rot && (!rot->isLeaf()))
   {
      //Получение высоты и количества листьев
      int leafsCount = 0;
      rGetCountLeafs(rot, leafsCount);
      int height = rGetHeight(rot);
      //Перевод этих чисел в float
      float leafsCountF = static_cast<float>(leafsCount);
      float heightF = static_cast<float>(height);
      //Высчитываем соотношение
      float magicNum = heightF / leafsCountF;

      //Вывод соответствующих
      if (magicNum == max)
      {
         cout << "Max Branch" << endl;
         rPrint("", rot, true, 0, 0);
      }
      if (magicNum == min)
      {
         cout << "Min Branch" << endl;
         rPrint("", rot, true, 0, 0);
      }
      //Рекурсивный вызов для потомков
      for (int i = 0; i < N; i++)
      {
         rPrintWithDelta(rot->children[i], max, min);
      }
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

void myNTree::rGetCountLeafs(Item* rot, int& sum)
{
   if (rot)
   {
      if (rot->isLeaf())
      {
         sum++;
      }
      //Рекурсивный вызов у потомков
      for (auto i = 0; i < N; i++)
      {
         rGetCountLeafs(rot->children[i], sum);
      }
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
      root = new Item(dt, N);
   }
}

void myNTree::addElemWithChoice(const int dt)
{
   if (root)
   {
      rAddElemWithChoice(root, dt);
   }
   else
   {
      root = new Item(dt, N);
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

const int myNTree::getCountElems()
{
   if (root)
   {
      int result = 0;
      rGetCountLeafs(root, result);
      return result;
   }
   else
   {
      return 0;
   }
}

void myNTree::printMaxMin()
{
   cout << "===Max/Min trees===" << endl;
   if (root)
   {
      float max = 0;
      float min = FLT_MAX;
      rGetMaxMinDelta(root, max, min);
      rPrintWithDelta(root, max, min);
      cout << endl;
   }
   else
   {
      cout << "Your tree is empty" << endl;
   }
}
