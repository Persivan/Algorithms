#include <iostream>
#include <fstream>

using namespace std;

//Используется только для структуры очереди
#define QMAX 100

struct queue {
   int qu[QMAX];
   int rear, frnt;
};

//Функция инициализации очереди
//Получает в параметрах очередь
void init(queue* q)
{
   q->frnt = 1;
   q->rear = 0;
   return;
}

//Функция добавления элемента в очередь
//Получает в параметрах очередь и значение, которое добавляем
void insert(queue* q, int x)
{
   if (q->rear < QMAX - 1)
   {
      q->rear++;
      q->qu[q->rear] = x;
   }
   else
   {
      cout << "Очередь полна!" << endl;
   }
   return;
}

//Функция на проверку, пуста ли наша очередь
//В параметрах очередь
//Возвращает 1, если очередь пуста и 0 в противном случае
bool isempty(queue* q)
{
   //if (q->rear < q->frnt)
   //{
   //   return 1;
   //}
   //else
   //{
   //   return 0;
   //}
   return (q->rear < q->frnt);
}

//Функция вывода элементов очереди
//В параметрах очередь
void print(queue* q)
{
   //Проверяем пуста ли очередь
   if (isempty(q) == 1)
   {
      cout << "Очередь пуста!" << endl;
      return;
   }
   for (int i = q->frnt; i <= q->rear; i++)
   {
      cout << q->qu[i] << " ";
   }
   cout << endl;
}

//Выполняем функцию по заданию
void magicFunc(int* arr, queue* q, int N)
{
   int C;
   cout << endl << "Введите число С: ";
   //Защита от дурака
   while (!(cin >> C) || (cin.peek() != '\n'))
   {
      cin.clear();
      while (cin.get() != '\n');
      cout << "Неверно, попробуйте еще! C = ";
   }
   //Заполняем элементами которые меньше С
   for (int i = 0; i < N; i++)
   {
      if (arr[i] < C)
      {
         insert(q, arr[i]);
      }
   }
   //Заполняем элементами которые равны С
   bool flag = false;
   for (int i = 0; i < N; i++)
   {
      if (arr[i] == C)
      {
         flag = true;
         break;
      }
   }
   if (flag == 1)
   {
      for (int i = 0; i < C; i++)
      {
         insert(q, C);
      }
   }
   //Заполняем элементами которые больше С
   for (int i = 0; i < N; i++)
   {
      if (arr[i] > C)
      {
         insert(q, arr[i]);
      }
   }
   cout << endl << "Получилась очередь: " << endl;
   print(q);
   cout << endl;
}

int main() {
   queue* q = new queue;
   int i, x, N;
   setlocale(LC_ALL, "Russian");
   //Инициализируем очередь
   init(q);

   int number;
   cout << "Как вы хотите ввести данные?" << endl;
   cout << "1: Заполнить от руки" << endl << "2: Ввод из файла " << endl;
   cout << "Number = ";
   cin >> number;

   //Заполнение от руки
   if (number == 1) {
      cout << "Введите колличество действительных чисел: ";
      while (!(cin >> N) || (cin.peek() != '\n') || (N < 1)) {
         cin.clear();
         while (cin.get() != '\n');
         cout << "Неверно!Попробуйте еще раз: N = ";
      }
      //Выделение памяти под массив
      int* arr = new int[N];
      cout << endl << "Введите элементы очереди: ";
      for (int i = 0; i < N; i++)
      {
         cin >> arr[i];
      }
      //Выполняем функцию по заданию
      magicFunc(arr, q, N);

      //Очищаем память от массива и очереди, которые больше не используются
      delete[] arr;
      delete q;
   }
   //Считывание из файла
   else if (number == 2)
   {
      fstream fs("text.txt");
      N = 0;
      if (!fs)
      {
         cout << "Файл не найден!" << endl;
         system("pause");
      }

      //Получаем количество слов из файла в N
      while (!(fs.eof()))
      {
         int a;
         fs >> a;
         N++;
      }
      //Помещаем курсор в начало файла
      fs.seekg(0);

      //Выделение памяти под массив
      int* arr = new int[N];
      //Заполнение массива из файла
      for (int i = 0; i < N; i++)
      {
         fs >> arr[i];
      }
      cout << endl;

      //Закрываем файл
      fs.close();
      //Вывод
      cout << "Числа из файла:" << endl;
      for (int i = 0; i < N; i++)
      {
         cout << arr[i] << "  ";
      }
      cout << endl;

      //Сортируем массив
      magicFunc(arr, q, N);

      //Освобождаем память под массив и очередь
      delete[] arr;
      delete(q);
   }
   return 0;
}