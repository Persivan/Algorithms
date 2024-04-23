/*
   Дано бинарное дерево. Найти все поддеревья, структура которых совпадает с заданной.
*/
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

template <typename T>
class Tree
{
   struct Node
   {
      T value;                          // значение, которое передает пользователь
      int count;                        // количество этих значений
      Node* left;                       // указатель на левое поддерево
      Node* right;                      // указатель на правое поддерево
   };

private:
   int size_ = 0;                       // размер дерева
   T* list;                         // дерево в виде списка, который формируется при dfs
   int countNodes = 0;
   string dfs_str;                      // dfs строка, которая поможет для поиска поддеревьев
   Node* root;                          // указатель на корень дерева
   Node* subRoot;                       // указатель на корень поддерева, которая поможет при выводе поддеревьев

   Node* addNode(Node* root, T value)   // метод добавления нового значения
   {
      if (root == nullptr)
      {
         root = new Node;
         root->value = value;
         root->count = 1;
         root->left = root->right = nullptr;
         size_++;
      }
      else if (root->value > value)     // если наше значения меньше значения в корне
      {                                 // смотрим в левом поддереве
         root->left = addNode(root->left, value);
      }
      else if (root->value < value)     // если наше значения больше значения в корне
      {                                 // смотрим в правом поддереве
         root->right = addNode(root->right, value);
      }
      else
         ++root->count;

      return root;
   }

   void printTree(Node* root, int level) // функция вывода дерева (повернутое боком)
   {
      if (root == nullptr)
         return;

      printTree(root->right, level + 1);

      for (int i = 0; i < level; i++)
         cout << "---";
      cout << root->value << '\n';

      printTree(root->left, level + 1);
   }

   void delTree(Node* root)             // метод удаления/очистки дерева
   {
      if (root == nullptr)
         return;
      
      delTree(root->left);
      delTree(root->right);
      delete root;
   }

   void searchNode(Node* root, T value) // метод поиска элемента по значению
   {
      if (root == nullptr)
         return;

      searchNode(root->left, value);
      if (root->value == value)
         subRoot = root;                // сохраняем значение в корень поддерева subRoot
      searchNode(root->right, value);
   }

   void dfsTree(Node* root_, int num)   // Метод, который выполняет поиск в глубину, и заполняет dfs строку и dfs list
   {
      if (root_ == nullptr)
         return;
      if (root_ != root->left)
      {
         if (num == 1) dfs_str += "1";  // если dfs шел в левое поддерево
         if (num == 2) dfs_str += "2";  // если dfs шел в правое поддерево
         list[countNodes++] = root_->value;
      }
      dfsTree(root_->left, 1);

      dfsTree(root_->right, 2);
      if (root_ != root->left)
      {
         if (num == 1) dfs_str += "3";  // если dfs выходил из левого поддерева
         if (num == 2) dfs_str += "4";  // если dfs выходил из правого поддерева
         list[countNodes++] = root_->value;
      }
   }

   void dfs()                           // метод-обертка dfsTree
   {
      list = new T[size_ * 2];

      Node* newRoot = nullptr;
      newRoot = addNode(newRoot, 0);

      newRoot->left = root;
      root = newRoot;                   // создаем новый корень с одним указателем на основной корень

      dfs_str = "";
      dfsTree(root, 1);              // производим поиск в глубину
      dfs_str = dfs_str.substr(1, dfs_str.size() - 2); // удаляем первый и последний символ

      root = root->left;                // возвращаем наш корень на место
      size_--;
   }

public:
   Tree() : root(nullptr) {}
   ~Tree()
   {
      clear();
   }

   void add(T value)                    // метод-обертка добавления элемента
   {
      root = addNode(root, value);
   }

   void print()                         // метод-обертка вывода дерева
   {
      printTree(root, 0);
   }

   void clear()                         // метод-обертка удаления дерева
   {
      if (root)
      {
         delTree(root);
         root = nullptr;
      }
      size_ = 0;
   }

   void search(Tree<T>& subTree)        // метод, выполняющий поиск поддеревьев, структура которых совпадает с заданным
   {
      dfs();
      subTree.dfs();                    // выполняем для каждого дерева поиск в глубину

      bool isPrint = false;
      string dfs_substr_ = subTree.dfs_str; // сохраняем наши dfs строки
      string dfs_str_ = dfs_str;
      int n = 0;                            // переменная с индексом в строке dfs

      while (n >= 0)
      {
         n = dfs_str_.find(dfs_substr_);    // ищем вхождение dfs строки поддерева в dfs строке дерева
         if (n > 0)                         // если не 0, значит он начинается не с корня
         {
            T value = list[n];          // получается значение по индексу в dfs списке
            searchNode(root, value);        // ищем элемент по этому значению, записываем в subRoot

            printTree(subRoot, 0);             // выполняем вывод этого поддерева
            cout << '\n';
            isPrint = true;
            dfs_str_.replace(n, 1, "a");  // заменяем один элемент на букву, чтобы не было вхождений в это же место
         }
         else if (n == 0)                    // если 0 - значит корень
         {
            if (size_ == subTree.size_)      // если совпадают размеры деревьев - выводим
            {
               printTree(root, 0);
               isPrint = true;
            }
            else
            {
               cout << "Таких поддеревьев нет!\n";
               isPrint = true;
            }

            dfs_str_.replace(n, 1, "a");
         }

         if (isPrint == false)
            cout << "Таких поддеревьев нет!\n";
      }

      delete list;       // очистка памяти
      list = nullptr;
   }

   int size() const                     // метод, возвращающий размер дерева
   {
      return size_;
   }
};

ifstream fin("input.txt");

int input(int a, int b)
{
   while (true)                                            // цикл продолжается до тех пор, пока пользователь не введет корректное значение
   {
      int n;

      if (a == 0)
         cin >> n;
      else
         fin >> n;

      if (a == 1 && fin.fail())
      {
         cout << "\nОшибка в файле!\n";
         exit(1);
      }

      if (b == 0)                                          // для случаев ввода размера дерева
      {
         if (n > 0)
            return n;

         cin.clear();                                   // то возвращаем cin в обычный режим работы
         cin.ignore(INT_MAX, '\n');       // и удаляем значения предыдущего ввода из входного буфера
         cout << "Вы ввели символ или число <= 0! Повторите ввод: \n";
      }
      else                                                 // для ввода элементов дерева
      {
         if (cin.fail())                                   // если извлечение оказалось неудачным
         {
            cin.clear();                                   // то возвращаем cin в обычный режим работы
            cin.ignore(INT_MAX, '\n');       // и удаляем значения предыдущего ввода из входного буфера
            cout << "Вы ввели символ! Повторите ввод: \n";
         }
         else
            return n;
      }
   }
}

int main()
{
   setlocale(LC_ALL, "Rus");

   int k = 0;
   cout << "Как будем осуществлять ввод?\n 0 (или любой символ). Клавиатура.\n 1. Файл.\n";
   cin >> k;

   cin.clear();
   cin.ignore(INT_MAX, '\n');

   Tree<int> tree;                      // переменные для наших деревьев
   Tree<int> subTree;
   // осуществляем ввод наших деревьев
   cout << "Введите количество элементов в дереве: \n";
   int n = input(k, 0);
   if (k == 1)
   {
      cout << n << '\n';
   }

   cout << "Введите элементы дерева: \n";
   for (int i = 0; i < n; i++)
   {
      int num = input(k, 1);
      tree.add(num);

      if (k == 1)
      {
         cout << num << " ";
      }
   }

   cout << '\n';
   cout << "Введите количество элементов в поддереве: \n";
   n = input(k, 0);
   if (k == 1)
   {
      cout << n << '\n';
   }

   cout << "Введите элементы поддерева: \n";
   for (int i = 0; i < n; i++)
   {
      int num = input(k, 1);
      subTree.add(num);

      if (k == 1)
      {
         cout << num << " ";
      }
   }
   cout << '\n';
   fin.close();

   cout << "\nДерево: \n";
   tree.print();
   cout << "\nПоддерево: \n";
   subTree.print();
   cout << '\n';

   cout << "\nПоддеревья, структура которых совпадает с заданной: \n";
   tree.search(subTree);             // производим поиск поддеревьев, структура которых совпадает с заданной

}

/*
5
5 3 4 6 7
3
3 1 2

5
5 3 4 6 7
4
1 3 4 5

4
1 2 3 4
2
1 2

9
10 1 7 2 3 8 9 12 15
5
5 3 4 6 7

8
7 1 6 2 5 10 8 9
3
3 1 2

14
11 9 10 7 8 1 4 3 12 20 21 15 17 16
5
6 7 1 4 3

11
12 6 15 5 10 3 7 4 9 13 14
3
3 1 2
*/
