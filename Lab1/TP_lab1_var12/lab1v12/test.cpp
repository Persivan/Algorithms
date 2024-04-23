#include <iostream>
#include <fstream>
#include <string>

/* тест */

/*тест многострочных комментариев

конец
*/

//структура элемента очереди
//содержит значение и указатель на следующий элемент
struct Node
{
 Node(std::string v)
 {
  value = v;
 }
 Node* next = nullptr;
 std::string value;
};

//основа очереди
//хранит указатели на стартовый и конечный элемент
//имеет методы для взаимодействия с элементами очереди
class Queue
{
public:
 //добавляет element в конец очереди
 void push(Node* element)
 {
  if (begin == nullptr) //если очередь пустая
  {
   begin = element;
   end = begin;
  }
  else //если очередь не пустая
  {
   end->next = element;
   end = end->next;
  }
 };

 //возвращает первый элемент очереди, удаляя его
 Node* pop()
 {
  if (begin == nullptr)
  {
   return nullptr; //если очередь состоит из 0 элементов
  }

  if (begin == end) //если очередь состоит из 1 элемента
  {
   Node* temp = begin;
   begin = nullptr;
   end = nullptr;
   return temp;
  }
  else //если очередь состоит из >1 элемента
  {
   Node* temp = begin;
   begin = begin->next;
   return temp;
  }
 };

 //возвращает значение первого элемента
 std::string get()
 {
  return begin->value;
 }

 //возвращает размер очереди
 unsigned int getSize()
 {
  Node* current = begin;
  unsigned int size = 0;
  while (current != nullptr)
  {
 size++;
 current = current->next;
  }
  return size;
 }

 //вывод элементов через пробел в поток out
 void print(std::ostream& out)
 {
  Node* current = begin;
  while (current != nullptr)
  {
   out << current->value << " ";
   current = current->next;
  }
  out << std::endl;
 }

private:
 Node* begin = nullptr; //адрес начального элемента
 Node* end = nullptr; //адрес конечного элемента
};

//аналог std::stoi, но который не крашится при вводе буквы
int parseInt(std::string* str)
{
 const std::string numbers = "0123456789";
 int res = 0;
 //перебираем буквы str
 for (int j = 0; j < (*str).length(); j++)
 {
  //перебираем цифры
  for (int i = 0; i < 10; i++)
  {
   if ((*str)[j] == numbers[i])
   {
    res *= 10;
    res += numbers[i] - '0';
    break;
   }
  }
 }
 //проверяем на отрицательность
 if ((*str)[0] == '-') res *= -1;
 return res;
}

int main(int args, char* argv[])
{
 Queue queue_1, queue_2;
 //Переменная для хранения предыдущего ввода
 std::string strOld = "";
 if (args == 1)
 {
 //чтение из консоли если нет
 //параметра запуска
  std::string countStr;
  int count = 0;
  std::string str;

  //Создание первой очереди
  std::cout << "Enter the number of elements"<<
   " of the first order : ";
  std::cin >> countStr;
  count = parseInt(&countStr);

  //защита от ввода буквы или отрицательного числа
  if (count <= 0)
  {
   std::cout << "It is not a positive number!";
   return -1;
  }

  //запись очереди
  for (int i = 0; i < count; i++)
  {
   std::cout << i+1 << "-th element: ";
   std::cin >> str;
   if (str < strOld)
   {
    std::cout << "Order isn't sort!" << std::endl;
    return -2;
   }
   strOld = str;
   queue_1.push(new Node(str));
  }

  strOld = "";
  //Создание второй очереди
  std::cout << "Enter the number of elements"<<
   " of the second order : ";
  std::cin >> countStr;
  count = parseInt(&countStr);

  //защита от ввода буквы или отрицательного числа
  if (count <= 0)
  {
   std::cout << "It is not a positive number!";
   return -1;
  }

  //запись очереди
  for (int i = 0; i < count; i++)
  {
   std::cout << i+1 << "-th element: ";
   std::cin >> str;
   if (str < strOld)
   {
    std::cout << "Order isn't sort!" << std::endl;
    return -2;
   }
   strOld = str;
   queue_2.push(new Node(str));
  }
 }
 else //чтение с файла
 {
  std::ifstream fin(argv[1]);
  std::string tempStr;
  strOld = "";
  int count = 0;

  //чтение первой очереди
  std::getline(fin, tempStr);
  count = parseInt(&tempStr);

  //защита от ввода буквы или отрицательного числа
  if (count <= 0)
  {
   std::cout << "First count is not a "<<
    "positive number!";
   return -1;
  }

  for (int j = 0; j < count; j++) {
   std::getline(fin, tempStr);
   if (tempStr < strOld)
   {
    std::cout << "Order isn't sort!" << std::endl;
    return -2;
   }
   strOld = tempStr;
   queue_1.push(new Node(tempStr));
  }

  strOld = "";
  //чтение второй очереди
  std::getline(fin, tempStr);
  strOld = tempStr;
  count = parseInt(&tempStr);

  //защита от ввода буквы или отрицательного числа
  if (count <= 0)
  {
   std::cout << "Second count is not a "<<
    "positive number!";
   return -1;
  }

  for (int j = 0; j < count; j++)
  {
   std::getline(fin, tempStr);
   if (tempStr < strOld)
   {
    std::cout << "Order isn't sort!" << std::endl;
    return -2;
   }
   queue_2.push(new Node(tempStr));
  }
 }

 //Создание третьей очереди
 Queue queue_3;

 //цикл будет выполняться пока одна из двух
 //очередей не закончится
 while (queue_1.getSize() != 0
  && queue_2.getSize() != 0)
 {
  //мы берём меньшее значение из очередей,
  //добавляем его в третью очередь и удаляем его
  //из изначальной очереди
  if (queue_1.get() < queue_2.get())
  {
   queue_3.push(queue_1.pop());
  }
  else
  {
   queue_3.push(queue_2.pop());
  }
 }

 //Если первая очередь пустая,
 //то заносим в третью очередь всю вторую очередь
 while (queue_2.getSize())
 {
  queue_3.push(queue_2.pop());
 }

 //Если вторая очередь пустая,
 //то заносим в третью очередь всю первую очередь
 while (queue_1.getSize())
 {
  queue_3.push(queue_1.pop());
 }

 //Вывод итогового массива
 std::cout << "Queue 3: ";
 queue_3.print(std::cout);
 return 0;
}