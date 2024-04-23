#include <iostream>
#include <string>
#include <fstream>

//Самодельный вектор
class myIntVector1d
{
private:
   int* arr;
   unsigned int elemCounter;
   unsigned int currentArrSize;
public:
   myIntVector1d()
   {
      elemCounter = 0;
      currentArrSize = 10;
      this->arr = new int[currentArrSize];
   }
   myIntVector1d(unsigned int size)
   {
      currentArrSize = size;
      elemCounter = 0;
      this->arr = new int[size];
   }
   ~myIntVector1d()
   {
      delete[] arr;
   }
   bool include(int value)
   {
      bool flag = 0;
      for (auto i = 0; i < elemCounter; i++)
      {
         if (arr[i] == value)
         {
            flag = true;
            break;
         }
      }
      return flag;
   }
   void push(int value)
   {
      if (elemCounter < currentArrSize)
      {
         arr[elemCounter] = value;
         elemCounter++;
      }
      else
      {
         increaseArray(10);
         push(value);
      }
   }
   void print()
   {
      std::cout << "Your array[";
      for (unsigned int i = 0; i <= elemCounter - 2; i++)
      {
         std::cout << arr[i] << ", ";
      }
      std::cout << arr[elemCounter - 1] << "]" << std::endl;
   }
   int* get()
   {
      return this->arr;
   }
   unsigned int length()
   {
      return elemCounter;
   }
private:
   void increaseArray(unsigned int value)
   {
      if (currentArrSize)
      {
         int* tempArr = new int[currentArrSize];
         for (unsigned int i = 0; i < currentArrSize; i++)
         {
            tempArr[i] = this->arr[i];
         }
         delete[]this->arr;
         this->arr = new int[currentArrSize + value];
         for (unsigned int i = 0; i < currentArrSize; i++)
         {
            this->arr[i] = tempArr[i];
         }
         currentArrSize += value;
      }
      else
      {
         this->arr = new int[value];
         currentArrSize += value;
      }
   }
};

//Структура, представляющая собой элемент матрицы с координатами
template<typename T>
struct elemMatrix
{
   T value;
   int x;
   int y;
};


template<typename T>       //Будем использовать шаблонный класс
class List                 //Создаем тип данных Список
{
private:
   struct Item             //Структура, являющаяся звеном списка
   {
      elemMatrix<T> data;  //Звено
      Item* next;          //Указатель на следующее звено списка
      Item* prev;          //Указатель на предыдущее звено списка
   };
   Item* head;             //Указатель на начало списка
   Item* tail;             //Указатель на конец списка
   List::Item* create(const T&, const int, const int);   //Создание нового звена
   unsigned int matrixSize;
public:
   //Конструкторы
   List() :head(nullptr), tail(nullptr), matrixSize(0) {};
   List(myIntVector1d&, myIntVector1d&, myIntVector1d&);
   void addTail(const T&, const int, const int);   //Добавление звена в хвост списка
   void transparent();                    //Транспонирование матрицы
   void rotate();                         //Поворот матрицы на 90 градусов против часовой стрелке
   void rotateV2();                       //Поворот матрицы на 90 градусов по часовой стрелке
   void show();                           //Вывод списка на экран
   bool isFree(int, int);                 //Проверяет свободен ли элемент под x, y;
   T getElem(int, int);                   //Возвращает элемент под x, y если есть.
   void getA(myIntVector1d& newA);        //Возвращает массив элементов
   void getLI(myIntVector1d& newLI);      //Возвращает массив строк
   void getLJ(myIntVector1d& newLJ);      //Возвращает массив столбцов
   unsigned int getElemCount();           //Возвращает количество элементов
};

template <typename T>
typename List<T>::Item* List<T>::create(const T& vl, const int x, const int y)
{
   Item* item = new Item;
   item->data.value = vl;
   item->data.x = x;
   item->data.y = y;
   item->next = nullptr;
   item->prev = nullptr;
   return item;
}

template<typename T>
List<T>::List(myIntVector1d& tA, myIntVector1d& tLJ, myIntVector1d& tLI)
{
   head = nullptr;
   tail = nullptr;
   matrixSize = 0;
   for (auto i = 0; i < tLJ.length(); i++)
   {
      if (matrixSize < tLJ.get()[i])
      {
         matrixSize = tLJ.get()[i];
      }
      if (matrixSize < tLI.get()[i])
      {
         matrixSize = tLI.get()[i];
      }
   }
   matrixSize++;
   for (auto i = 0; i < tA.length(); i++)
   {
      addTail(tA.get()[i], tLJ.get()[i], tLI.get()[i]);
   }
}

template<typename T>
void List<T>::addTail(const T& vl, const int x, const int y)
{
   if (tail && head)
   {
      tail->next = create(vl, x, y);
      tail->next->prev = tail;
      tail = tail->next;
   }
   else
   {
      head = create(vl, x, y);
      tail = head;
   }
}

template<typename T>
void List<T>::transparent()
{
   Item* current = head;
   while (current)
   {
      int temp = current->data.x;
      current->data.x = current->data.y;
      current->data.y = temp;
      current = current->next;
   }
}

template<typename T>
void List<T>::rotate()
{
   Item* current = head;
   while (current)
   {
      int temp = current->data.x;
      current->data.x = current->data.y;
      current->data.y = matrixSize - temp - 1;
      current = current->next;
   }
}

template<typename T>
void List<T>::rotateV2()
{
   Item* current = head;
   while (current)
   {
      int temp = current->data.y;
      current->data.x = current->data.x;
      current->data.y = matrixSize - temp - 1;
      current = current->next;
   }
}

template<typename T>
void List<T>::show()
{
   if (matrixSize == 0)
   {
      std::cout << "Matrix size 0!" << std::endl;
      return;
   }
   std::cout << "=====Your matrix=====" << std::endl;
   Item* current = head;

   int** aOut = new int* [matrixSize]; //Строки в массиве
   for (auto i = 0; i < matrixSize; i++)
   {
      aOut[i] = new int[matrixSize]; //столбцы
   }
   //Инициализация
   for (auto i = 0; i < matrixSize; i++)
   {
      for (auto j = 0; j < matrixSize; j++)
      {
         aOut[i][j] = 0;
      }
   }
   //Заполнение
   while (current)
   {
      aOut[current->data.y][current->data.x] = current->data.value;
      current = current->next;
   }
   //Вывод
   for (int i = 0; i < matrixSize; i++)
   {
      for (auto j = 0; j < matrixSize; j++)
      {
         std::cout << aOut[i][j] << ", ";
      }
      std::cout << std::endl;
   }
   //Удаление
   for (int i = 0; i < matrixSize; i++)
   {
      delete[]aOut[i];
   }
}

template<typename T>
bool List<T>::isFree(int x, int y)
{
   bool flag = 1;
   Item* current = head;
   while (current)
   {
      if (current->data.x == x && current->data.y == y)
      {
         flag = 0;
         break;
      }
      current = current->next;
   }
   return flag;
}

template<typename T>
T List<T>::getElem(int x, int y)
{
   Item* current = head;
   if (!isFree(x, y))
   {
      while (current)
      {
         if (current->data.x == x && current->data.y == y)
         {
            return current->data.value;
         }
         current = current->next;
      }
   }
   return T();
}

template<typename T>
void List<T>::getA(myIntVector1d& newA)
{
   Item* current = head;
   while (current)
   {
      try
      {
         newA.push(current->data.value);
      }
      catch (const std::exception& ex)
      {
         std::cout << "error while getA, " << ex.what() << std::endl;
      }
      current = current->next;
   }
}

template<typename T>
void List<T>::getLI(myIntVector1d& newLI)
{
   Item* current = head;
   while (current)
   {
      try
      {
         newLI.push(current->data.x);
      }
      catch (const std::exception& ex)
      {
         std::cout << "error while getA, " << ex.what() << std::endl;
      }
      current = current->next;
   }
}

template<typename T>
void List<T>::getLJ(myIntVector1d& newLJ)
{
   Item* current = head;
   while (current)
   {
      try
      {
         newLJ.push(current->data.y);
      }
      catch (const std::exception& ex)
      {
         std::cout << "error while getA, " << ex.what() << std::endl;
      }
      current = current->next;
   }
}

template<typename T>
unsigned int List<T>::getElemCount()
{
   Item* current = head;
   int count = 0;
   while (current)
   {
      current = current->next;
      count++;
   }
   return count;
}

bool is_number(const std::string& s)
{
   std::string::const_iterator it = s.begin();
   while (it != s.end() && std::isdigit(*it))
   {
      ++it;
   }
   return !s.empty() && it == s.end();
}

//Вернет первую цифру из первой строки файла filename
int good_file_input(const std::string filename)
{
   int fs = 0;
   std::ifstream fin(filename);
   if (fin.is_open())
   {
      if (!fin.eof())
      {
         std::string instr;
         std::getline(fin, instr);
         try
         {
            fs = std::stoi(instr);
         }
         catch (const std::exception& ex)
         {
            std::cout << "Wrong file input! Error: " << ex.what() << std::endl;
         }
      }
   }
   else
   {
      std::cout << "File does not exist" << std::endl;
   }
   return fs;
}

//Заполняет двумерный массив из файла
/*Первый параметр название файла, второй параметр массив который заполняем
* Файл должен в первой строчке должен хранить размер матрицы
* Далее матрица, элементы одной строки отделены пробелами
*/
void fillMatrixFromFile(const std::string filename, int**& arr)
{
   int fs = 0;
   std::ifstream fin(filename);
   if (fin.is_open())
   {
      std::string instr = "";
      int currPosY = 0;
      getline(fin, instr);
      while (getline(fin, instr))
      {
         //Разбиваем строку на подстроки
         std::string oneWord = "";
         int currPosX = 0;
         for (auto i = 0; i < instr.size(); i++)
         {
            oneWord = "";
            while (instr[i] != ' ' && i < instr.size())
            {
               oneWord += instr[i];
               i++;
            }
            try
            {
               arr[currPosX][currPosY] = std::stoi(oneWord);
               currPosX++;
            }
            catch (const std::exception& ex)
            {
               std::cout << "Wrong file input! Error: " << ex.what() << " will return 0" << std::endl;
               arr[currPosX][currPosY] = 0;
            }
         }
         currPosY++;
      }
   }
   else
   {
      std::cout << "File does not exist" << std::endl;
   }
}

const int good_console_input()
{
   std::string instr = "";
   int res = 0;
   while (true)
   {
      std::cout << "Enter the number: ";
      std::cin >> instr;
      try
      {
         res = std::stoi(instr);
         break;
      }
      catch (const std::exception& ex)
      {
         std::cout << "Wrong console input! Error: " << ex.what() << std::endl;
      }
   }
   return res;
}

void myIntVector2d()
{
   //need create
   //2d динамический массив
   //Ввод
   std::cout << "Enter rows: ";
   unsigned int matrix_rows = 0;
   std::cin >> matrix_rows;
   std::cout << "Enter columns: ";
   unsigned int matrix_columns = 0;
   std::cin >> matrix_columns;
   //Создание
   int** arr = new int* [matrix_rows];    //Строки в массиве
   for (auto i = 0; i < matrix_rows; i++)
   {
      arr[i] = new int[matrix_columns];   //столбцы
   }
   //Заполнение
   for (auto i = 0; i < matrix_rows; i++)
   {
      for (auto j = 0; j < matrix_columns; j++)
      {
         arr[i][j] = i * 10 + j;
      }
   }
   //Вывод
   for (int i = 0; i < matrix_rows; i++)
   {
      for (auto j = 0; j < matrix_columns; j++)
      {
         std::cout << arr[i][j] << ", ";
      }
      std::cout << std::endl;
   }
   //Удаление
   for (int i = 0; i < matrix_rows; i++)
   {
      delete[]arr[i];
   }
};

int main()
{
   //Вариант заполнения через двумерный массив через файл
   unsigned int matrixRowsColumns = 0;
   matrixRowsColumns = good_file_input("input.txt");
   //Создание
   int** arr = new int* [matrixRowsColumns]; //Строки в массиве
   for (auto i = 0; i < matrixRowsColumns; i++)
   {
      arr[i] = new int[matrixRowsColumns];   //столбцы
   }
   //Заполнение
   fillMatrixFromFile("input.txt", arr);

   ////Вариант заполнения через двумерный массив консолькой
   ////Ввод
   //unsigned int matrixRowsColumns = 0;
   //std::cout << "Enter rows/columns: "; //Так как матрица квадратная по условию
   //matrixRowsColumns = good_console_input();
   ////Создание
   //int** arr = new int* [matrixRowsColumns]; //Строки в массиве
   //for (auto i = 0; i < matrixRowsColumns; i++)
   //{
   //	arr[i] = new int[matrixRowsColumns]; //столбцы
   //}
   ////Заполнение
   //for (auto i = 0; i < matrixRowsColumns; i++)
   //{
   //	for (auto j = 0; j < matrixRowsColumns; j++)
   //	{
   //		std::cout << "Enter elem on x=" << i << " y=" << j << ": ";
   //		arr[i][j] = good_console_input();
   //	}
   //}

   //Теперь у меня двумерный массив arr содержит матрицу.
   //Заполняю три массива
   myIntVector1d A(matrixRowsColumns);		//значения
   myIntVector1d LJ(matrixRowsColumns);	//строки
   myIntVector1d LI(matrixRowsColumns);	//столбцы
   for (auto i = 0; i < matrixRowsColumns; i++)
   {
      for (auto j = 0; j < matrixRowsColumns; j++)
      {
         if (arr[i][j] != 0)
         {
            A.push(arr[i][j]);
            LJ.push(i);
            LI.push(j);
         }
      }
   }
   //Удаление
   for (int i = 0; i < matrixRowsColumns; i++)
   {
      delete[]arr[i];
   }




   List<int> list(A, LJ, LI);
   std::cout << std::endl << "Original matrix" << std::endl;
   list.show();
   list.rotate();
   std::cout << std::endl << "After 90 degrees rotate left" << std::endl;
   list.show();
   list.rotateV2();
   std::cout << std::endl << "After 90 degrees rotate right" << std::endl;
   list.show();


   //Вывод обратно в массивы
   std::cout << std::endl;
   myIntVector1d newA;
   myIntVector1d newLI;
   myIntVector1d newLJ;
   list.getA(newA);
   list.getLI(newLI);
   list.getLJ(newLJ);

   //Вывод на экран (дебаг)
   std::cout << "A:  ";
   for (auto i = 0; i < newA.length(); i++)
   {
      std::cout << newA.get()[i] << " ";
   }
   std::cout << std::endl << "LI: ";
   for (auto i = 0; i < newLI.length(); i++)
   {
      std::cout << newLI.get()[i] << " ";
   }
   std::cout << std::endl << "LJ: ";
   for (auto i = 0; i < newLJ.length(); i++)
   {
      std::cout << newLJ.get()[i] << " ";
   }
   std::cout << std::endl;
   return 0;
}