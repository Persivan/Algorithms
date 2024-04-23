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
   return length;
}


struct Item
{
   int N;
   int data;
   Item** children;   //Массив ветвей размера N
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
   Item* rAddElem(Item*, const int&, unsigned int&, int, bool&);  //Добавление элемента
   Item* rAddElemWithChoice(Item*, const int&);                   //Добавление элемента
   const int rGetHeight(Item*);                                   //Получение высоты поддеревьев
   void rToString(const string&, Item*, bool, 
      unsigned int, bool, string&);                               //Печать элементов поддеревьев
   void rCountElemsOnLvls(Item*, int*, int);                      //Подсчет элементов поддеревьев
   
   void rFillLongestIncreasingBranch(Item*, Item*&, int&); //Создать самую длинную ветку с элементами по возрастанию
   int rGetIncreasedHeight(Item*);                    //Получить высоту ветки по возрастанию.
   void rDeleteNotIncreasingBranches(Item*&);         //Удаляет ветки не по возрастанию
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
   string toString();                        // Преобразовать в строку
   const int getHeight();                    //Получение высоты дерева

   void printLongestIncreasingBranch();      //Печать самой длинной ветки с элементами по возрастанию.
};


int main()
{
   cout << "Enter N for N-tree" << endl;
   int inputIntNTree = 0;
   while (true)
   {
      inputIntNTree = consoleInt();
      if (inputIntNTree < 1)
      {
         cout << "   N should be more than 1" << endl;
      }
      else
      {
         break;
      }
   }
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
   cout << newTree.toString();
   //Вывод по заданию
   newTree.printLongestIncreasingBranch();
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
      for (int i = 0; i < N; i++)
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

void myNTree::rToString(const string& prefix, Item* rot, bool isLast, 
   unsigned int childIndex, bool isAlone, string& myStrTree)
{
   if (rot)
   {
      myStrTree += prefix + (isLast ? "`--" : "|--") + to_string(rot->data) + "\n";
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
         rToString(prefix + (isLast? "    " : "|   "), rot->children[i], tIsLast, i, tIsAlone, myStrTree);
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

void myNTree::rFillLongestIncreasingBranch(Item* rot, Item*& result, int& maxlength)
{
   if (rot)
   {
      //Максимальная длина меньше, чем длина ветки которая поступила
      if (maxlength < rGetIncreasedHeight(rot))
      {
         result = rot;
         maxlength = rGetIncreasedHeight(rot);
         rDeleteNotIncreasingBranches(result);
      }
      // рекурсивный переход дальше
      for (auto i = 0; i < N; i++)
      {
         rFillLongestIncreasingBranch(rot->children[i], result, maxlength);
      }
      
   }
}
int myNTree::rGetIncreasedHeight(Item* rot)
{
   if (rot)
   {
      int rotMax = 0;
      for (int i = 0; i < N; i++)
      {
         if (rot->children[i] && rot->data < rot->children[i]->data)
         {
            rotMax = max(rotMax, rGetHeight(rot->children[i]));
         }
         else
         {
            return 0;
         }
      }
      return 1 + rotMax;
   }
   else
   {
      return 0;
   }
}
void myNTree::rDeleteNotIncreasingBranches(Item*& rot)
{
   if (rot)
   {
      // Находим максимальную длину
      int maxHeight = 0;
      for (int i = 0; i < N; i++)
      {
         if (maxHeight < this->rGetIncreasedHeight(rot->children[i]))
         {
            maxHeight = this->rGetIncreasedHeight(rot->children[i]);
         }
      }
      // Удаляем те ветки, котоорые не соответствуют максимому
      for (int i = 0; i < N; i++)
      {
         if (maxHeight != this->rGetIncreasedHeight(rot->children[i]))
         {
            rot->children[i]->~Item();
            rot->children[i] = nullptr;
         }
         if (rot->children[i] && rot->data > rot->children[i]->data)
         {
            rot->children[i]->~Item();
            rot->children[i] = nullptr;
         }
      }
      // Рекурсивный переход к следующему
      for (int i = 0; i < N; i++)
      {
         rDeleteNotIncreasingBranches(rot->children[i]);
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

string myNTree::toString()
{
   string myStrWithTree = "";
   if (root)
   {
      rToString("", root, true, 0, 0, myStrWithTree);
      cout << endl;
   }
   else
   {
      myStrWithTree = "Your tree is empty";
   }
   return myStrWithTree;
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

void myNTree::printLongestIncreasingBranch()
{
   if (root != nullptr)
   {
      Item* result = nullptr;
      int maxLength = 0;
      rFillLongestIncreasingBranch(root, result, maxLength);
      cout << "===Your Branch===" << endl;
      string myStrWithTree = "";
      rToString("", result, true, 0, 0, myStrWithTree);
      cout << myStrWithTree;
      cout << endl;
   }
   else
   {
      cout << "[printLongestIncreasingBranch] Your tree is empty" << endl;
   }
}
