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

   Tree::Item* create(const int);                     //Создание нового звена
   Tree::Item* rAddElem(Item*, const int);            //Добавление элемента
   Tree::Item* rAddElemWithChoise(Item*, const int);  //Добавление элемента с выбором куда
   void rPrint(const string&, Item*, bool, bool);     //Печать элементов поддеревьев
   bool rInclude(Item*, const int);                   //Содержит ли число
   Tree::Item* rRemove(Item*, const int);             //Удаляет элемент по числу
   void rPiramida(Item*, bool&);
   void rPrintPiramida(Item*);

   void rPrintTemplates(Item*, Item*);
   void isTemplate(Item*, Item*, bool&);

   const bool rIsBinary(Item*);
   const int rGetHeightEven(Item*);                //Получение высоты поддеревьев c четными вершинами (вовзращая число), типо и так и так умею
   const int rMaxWayEven(Item*);                   //Получение макс высоты
   const int rGetSumOdd(Item*, int);               //Найти сумму элементов на нечетных уровнях
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
   bool include(const int);   //Содержит ли ключ
   void remove(const int);    //Удаляет элемент по ключу
   bool isLeaf(Item*);

   void piramida();


   void printTemplate();
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
   cout << "Include 10? " << newTree.include(10) << endl;
   newTree.remove(4);
   //Вывод полученного дерева
   newTree.print();
   newTree.printTemplate();
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

void Tree::rPiramida(Item* rot, bool& is_piramida)
{
   if (rot == nullptr || is_piramida == false)
   {
      return;
   }
   if ((rot->left && rot->right) || isLeaf(rot))
   {
      is_piramida = true;
      rPiramida(rot->left, is_piramida);
      rPiramida(rot->right, is_piramida);
   }
   else
   {
      is_piramida = false;
      return;
   }
}

void Tree::rPrintPiramida(Item* rot)
{
   // Если текущий узел пустой
   if (rot == nullptr)
   {
      return;
   }
   // Алгоритм
   bool isPyramida = true;
   rPiramida(rot, isPyramida);
   if (isPyramida)
   {
      cout << "Piramida" << endl;
      rPrint("", rot, false, (rot->right && rot->left));
   }
   // Рекурсивный вызов
   rPrintPiramida(rot->left);
   rPrintPiramida(rot->right);
}

void Tree::rPrintTemplates(Item* rot, Item* templ)
{
   // Проверка пустой ли узел
   if (rot == nullptr)
   {
      return;
   }
   // Вызов рекурсивной функции проверки текущий узел удовлетворяет шаблону?
   bool isTempl = true;
   isTemplate(rot, templ, isTempl);
   // Вывод дерева если да
   if (isTempl)
   {
      cout << "Template tree" << endl;
      rPrint("", rot, false, (rot->right && rot->left));
   }
   // Рекурсивный вызов у потомков
   rPrintTemplates(rot->left, templ);
   rPrintTemplates(rot->right, templ);
}

void Tree::isTemplate(Item* rot, Item* templ, bool& isTempl)
{
   // Если узел шаблона пустой
   if (templ == nullptr || isTempl == false)
   {
      return;
   }
   // проверка текущий узел удовлетворяет шаблону?
   if (rot && templ)
   {
      // Рекурсивный вызов для проверки дальнейших потомков удовлетворяют ли они шаблону
      isTemplate(rot->left, templ->left, isTempl);
      isTemplate(rot->right, templ->right, isTempl);
   }
   else
   {
      isTempl = false;
      return;
   }
}

const bool Tree::rIsBinary(Item* rot)
{
   // Выход из рекурсии на пустом узле
   if (rot == nullptr)
   {
      return true;
   }
   // Если существует левый потомок
   if (rot->left)
   {
      // Проверяем а вдруг он больше чем текущий узел (мы считаем что в 
      // бинарном дереве поиска в левый потомок идут меньшие или равные числа
      // а в правый потомок большиее числа )
      if (rot->data < rot->left->data)
      {
         // 
         return false;
      }
   }
   // Аналогично для правого потомка
   // Если существует правый потомок
   if (rot->right)
   {
      // Проверяем
      if (rot->data >= rot->right->data)
      {
         return false;
      }
   }
   // Рекурсивный вызов у потомокв
   // true здесь это типо проверка текущего узла, так как
   // мы дошли до этого участка кода
   // то мы уверены что текущий узел и его 2 
   // потомка построены по алгоритмам бинарных деревьев
   return (true and rIsBinary(rot->left) and rIsBinary(rot->right));
}

// Функция подсчета длины пути четных вершин для заданного узла
const int Tree::rGetHeightEven(Item* rot)
{
   // Если текущий узел пустой ИЛИ если текущий узел нечетная вершина
   if (rot == nullptr || rot->data % 2 == 1)
   {
      return 0;
   }
   // Рекурсивный вызов (единичка это сама вершина, а она четная т.к. прошлый if не сработал) 
   //плюс максимальное среди двух веток (max встроенная функция)
   return 1 + max(rGetHeightEven(rot->left), rGetHeightEven(rot->right));
}


const int Tree::rMaxWayEven(Item* rot)
{
   // Куда будем записывать максимум
   int maxCount = 0;
   // Если текущий узел пустой (дошли до конца дерева)
   if (rot == nullptr)
   {
      return maxCount;
   }
   // Считаем для правой и левой ветки длину четных вершин
   int maxLeft = rGetHeightEven(rot->left);
   int maxRight = rGetHeightEven(rot->right);
   // Записываем в maxCount макс среди двух (без использования встроенной функции max)
   if (maxCount < maxLeft || maxCount < maxRight)
   {
      if (maxLeft < maxRight)
      {
         maxCount = maxRight;
      }
      else
      {
         maxCount = maxLeft;
      }
   }
   // Рекурсивный вызов у потомков (да, мы игнорируем то, что он возвращает)
   rMaxWayEven(rot->left);
   rMaxWayEven(rot->right);
   return 1 + maxCount;
}



const int Tree::rGetSumOdd(Item* rot, int depth)
{
   // Выход из рекурсии
   if (rot == nullptr)
   {
      return 0;
   }
   // Текущий уровень +1 (начинался с нуля), ЗАМЕЧУ, ЭТО НЕ ГЛОБАЛЬНАЯ ПЕРЕМЕННАЯ, 
   // каждый вызов функции будет создаваться копия прошлого значения, увеличиваться на один
   // и в 503, 509 передаваться это значение, а дальше функция создаст копию и так по кругу.
   depth++;
   int sum = 0;
   // Если текущий уровень четный добавляем число и рекурсивно вызываем у потомком
   if (depth % 2 == 1)
   {
      // В рекурсивном вызове передается текущий уровень, 
      sum = rot->data + rGetSumOdd(rot->left, depth) + rGetSumOdd(rot->right, depth);
   }
   // Иначе добавляем ноль
   else
   {
      sum = 0 + rGetSumOdd(rot->left, depth) + rGetSumOdd(rot->right, depth);
   }
   return sum;
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

bool Tree::isLeaf(Item* rot)
{
   return rot->left == nullptr && rot->right == nullptr;
}

void Tree::piramida()
{
   if (root == nullptr)
   {
      cout << "no tree";
      return;
   }
   cout << "piramida:" << endl;
   rPrintPiramida(root);
}

void Tree::printTemplate()
{
   // Проверка существует ли дерево
   if (root == nullptr)
   {
      cout << "No tree";
      return;
   }
   //Создание второго дерева с защитами
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
   newTree.print();
   //Вызов рекурсивной функции
   rPrintTemplates(root, newTree.root);
}

