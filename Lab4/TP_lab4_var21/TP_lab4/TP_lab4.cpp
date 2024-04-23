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
   int N;
   int data;
   string color;
   Item** children;   //Массив ветвей размера N
   Item(const int dt, const string col, const int numOfChilds)   //Конструктор элемента
   {
      children = new Item * [numOfChilds];
      N = numOfChilds;
      data = dt;
      color = col;
      for (auto i = 0; i < N; i++)
      {
         children[i] = nullptr;
      }
   }
   ~Item()              //Деструктор
   {
      for (int i = 0; i < N; i++)
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
   Item* root;                                                 //Корень дерева
   int N;                                                      //Количество сыновей в дереве
   //Рекурсивные функции для работы с деревом
   Item* rAddElem(Item*, const int&, unsigned int&, int, bool&, const string&); // Автоматическое добавление элемента в ветку
   Item* rAddElemWithChoice(Item*, const int&, const string);  //Добавление элемента
   const int rGetHeight(Item*);                                //Получение высоты поддеревьев
   void rPrint(const string&, Item*, bool, unsigned int, bool);//Печать элементов поддеревьев
   void rCountElemsOnLvls(Item*, int*, int);                   //Подсчет элементов поддеревьев
   void rGetCountElems(Item*, int& );                          //Посчитать количества элементов

   void rMagicFunc(Item*);                                     //Выполнить по заданию задание :]
   void rMagicFunc2(Item*, int&);
   const int rGetHeightYellow(Item*);
public:
   myNTree(const int numOfChilds) : root(nullptr), N(numOfChilds) {};
   //Деструктор
   ~myNTree() { delete root; };
   //Методы использующие рекурсивные методы
   void addElem(const int, const string);                      //Автоматическое добавление элемента в дерево
   void addElemWithChoice(const int, const string);            //Добавление элемента в дерево с интерфейсом выбора
   void print();                                               //Печать всего дерева
   const int getHeight();                                      //Получение высоты дерева
   const int getCountElems();                                  //Получение количества элементов дерева

   void magicFunc();                                           //Заменяет всем вершинам с нечетными номерами цвет
   void magicFunc2();                                          // Удаляет желтые вершины
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
         cout << "New Element value: " << endl;
         int inInt = 0;
         inInt = consoleInt();
         if (inInt == -1)
         {
            break;
         }
         string col = "";
         cout << "Choose color (1,2,3): " << endl;
         cout << "1) yellow " << endl;
         cout << "2) black " << endl;
         cout << "3) white " << endl;
         int inputIntCol = consoleInt();
         while (inputIntCol < 1 || inputIntCol > 3)
         {
            cout << " wrong number for color! 1 or 2 or 3" << endl;
            inputIntCol = consoleInt();
         }
         if (inputIntCol == 1)
         {
            col = "yellow";
         }
         else if (inputIntCol == 2)
         {
            col = "black";
         }
         else if (inputIntCol == 3)
         {
            col = "white";
         }
         newTree.addElemWithChoice(inInt, col);
      }
   }
   else
   {
      int* simpleArray = nullptr;
      int size = fileIntArray("input.txt", simpleArray);
      cout << "Array filled!" << endl;
      for (auto i = 0; i < size; i++)
      {
         newTree.addElem(simpleArray[i], "yellow");
      }
   }
   //Вывод полученого дерева
   newTree.print();
   cout << "Count of elems = " << newTree.getCountElems() << endl;
   //Вывод по заданию
   newTree.magicFunc2();
   newTree.print();
   return 0;
}


Item* myNTree::rAddElem(Item* rot, const int& dt, unsigned int& lvlToFill, int currLvl, bool& flag, const string& col)
{
   if (currLvl <= lvlToFill)
   {
      if (!flag)
      {
         if (rot)
         {
            for (auto i = 0; i < N; i++)
            {
               rot->children[i] = rAddElem(rot->children[i], dt, lvlToFill, ++currLvl, flag, col);
               currLvl--;
            }
         }
         else
         {
            rot = new Item(dt, col, N);
            flag = true;
         }
      }
   }
   return rot;
}

Item* myNTree::rAddElemWithChoice(Item* rot, const int& dt, const string col)
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
      rot->children[inputInt] = rAddElemWithChoice(rot->children[inputInt], dt, col);
      cout << dt << " going to " << inputInt << " branch" << endl;
   }
   else
   {
      rot = new Item(dt, col, N);
   }
   return rot;
}

// Высота дерева (длина дерева я хз)
const int myNTree::rGetHeight(Item* rot)
{
   // Если текущий узел пустой, возвращаем 0 выходим из рекурсии
   if (rot == nullptr)
   {
      return 0;
   }
   // Ищем среди веток самую длинную
   int rotMax = 0;
   for (auto i = 0; i < N; i++)
   {
      rotMax = max(rotMax, rGetHeight(rot->children[i]));
   }
   // Возвращаем длину ветки (1 это сам узел а rotMax это длина макс длинной ветки)
   return 1 + rotMax;
}

void myNTree::rPrint(const string& prefix, Item* rot, bool isLast, unsigned int childIndex, bool isAlone)
{
   if (rot)
   {
      std::cout << prefix << (isLast ? "`--" : "|--") << rot->data << " (" << rot->color << ")" << endl;
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

void myNTree::rGetCountElems(Item* rot, int& sum)
{
   if (rot)
   {
      sum++;
      for (auto i = 0; i < N; i++)
      {
         rGetCountElems(rot->children[i], sum);
      }
   }
}

void myNTree::rMagicFunc(Item* rot)
{
   if (rot)
   {
      //Меняем цвета при условии что вершина нечетная
      if (rot->data & 1) 
      {
         if (rot->color == "yellow")
         {
            rot->color = "black";
         }
         else if (rot->color == "black")
         {
            rot->color = "white";
         }
         else if (rot->color == "white")
         {
            rot->color = "yellow";
         }
      }
      //рекурсивн переходим к след уровню
      for (int i = 0; i < N; i++)
      {
         rMagicFunc(rot->children[i]);
      }
   }
}

// Магическая херь удаляющая желтые вершины
void myNTree::rMagicFunc2(Item* rot, int& depth)
{
   // Если текущщий узел пустой, выходим из рекурсии
   if (rot == nullptr)
   {
      return;
   }
   depth++;
   // Алгоритм поиска и удаления
   // Если высота дерева совпала (замечу, тут root, а не rot) с текущей глубиной рекурсии
   // И цвет узла желтый
   if (rGetHeight(root) == depth && rot->color == "yellow")
   {
      // Удаляем
      rot->color = "deleted";
      return;
   }
   //рекурсивн переходим к след уровню
   for (int i = 0; i < N; i++)
   {
      rMagicFunc2(rot->children[i], depth);
      depth--;
   }
}

const int myNTree::rGetHeightYellow(Item* rot)
{
   // Если текущий узел пустой, возвращаем 0 выходим из рекурсии
   if (rot == nullptr || rot->color != "yellow")
   {
      return 0;
   }
   // Ищем среди веток самую длинную
   int rotMax = 0;
   for (auto i = 0; i < N; i++)
   {
      rotMax = max(rotMax, rGetHeightYellow(rot->children[i]));
   }
   // Возвращаем длину ветки (1 это сам узел а rotMax это длина макс длинной ветки)
   return 1 + rotMax;
}


void myNTree::addElem(const int dt, const string col)
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
      rAddElem(root, dt, lvlToFill, 0, flag, col);
   }
   else
   {
      root = new Item(dt, col, N);
   }
}

void myNTree::addElemWithChoice(const int dt, const string col)
{
   if (root)
   {
      rAddElemWithChoice(root, dt, col);
   }
   else
   {
      root = new Item(dt, col, N);
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
      rGetCountElems(root, result);
      return result;
   }
   else
   {
      return 0;
   }
}

void myNTree::magicFunc()
{
   if (root)
   {
      rMagicFunc(root);
   }
}

// Функция запускающая рекурсию
void myNTree::magicFunc2()
{
   // Если есть корень (существует ли дерево)
   if (root)
   {
      // Создаем функцию, она будет помогать определять глубину
      int depth = 0;
      // Запускаем рекурсию для корня
      rMagicFunc2(root, depth);
   }
}

