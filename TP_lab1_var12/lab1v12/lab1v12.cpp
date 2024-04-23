#include <iostream>    //работа с консолью
#include <fstream>     //работа с файлами
#include <string>      //хранение строк
#include <Windows.h>   //для смены кодировки ввода в консоль

using namespace std;

//структура элемента списка
//содержит значение и указатель на следующий элемент
struct Node
{
   Node(string v)
   {
      value = v;
   }

   Node* next = nullptr;
   string value;
};

//основа списка
//хранит указатели на стартовый и конечный элемент
//имеет методы для взаимодействия с элементами списка
class List
{
public:

   void push(Node* element)   //добавляет element в конец списка
   {
      if (begin == nullptr)   //если список пустой
      {
         begin = element;
         end   = begin;
      }
      else                    //если список не пустой
      {
         end->next    = element;
         end         = end->next;
      }
   };

   string get()               //возвращает значение первого элемента
   {
      return begin->value;
   }

   unsigned int getSize()     //возвращает размер списка
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

   void print(ostream& out)   //вывод элементов в поток out
   {
      Node* current = begin;
      while (current != nullptr)
      {
         try
         {
            out << current->value << endl;
            current = current->next;
         }
         catch (exception er)
         {
            cout << er.what() << endl;
         }
      }
   }

   void readFromFile(string filename)      //чтение списка из файла
   {
      ifstream fin(filename);
      if (!fin.is_open())
      {
         cout << "Wrong file name! Restart the programm." << endl;
         return;
      }
      string   lineFromFile;

      //считываем файл в список
      while (!fin.eof())
      {
         getline(fin, lineFromFile);
         this->push(new Node(lineFromFile));
      }
   }

   void readFromConsole()                       //чтение списка из консоли
   {
      int lineCount = 0;
      string line;

      while (true)                              //ввод размера
      {
         cout << "Enter count of lines: ";
         cin >> lineCount;

         if (cin.fail())
         {
            cout << "Wrong number!" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            system("pause");
            continue;
         }

         break;
      }
      cin.ignore(INT_MAX, '\n');

      for (int i = 0; i < lineCount; i++)
      {
         getline(cin, line);
         this->push(new Node(line));
      }
   }

   void deleteComments(string fileForComments)//удаление из списка комментарии, записывая их в файл 
   {
      ofstream commentsFile(fileForComments);

      Node* current = begin;
      string comment;
      unsigned int index = 0;
      bool isComment = false;

      while (current != nullptr)
      {
         //если найден однострочный комментарий
         index = current->value.find("//");
         if (index < current->value.size())
         {
            comment = current->value;
            comment.erase(0, index);
            commentsFile << comment;
            
            current->value.erase(index, current->value.size());
         }
         
         //если найдено начало многострочного комментария
         index = current->value.find("/*");
         if (index < current->value.size())
         {
            isComment = true;
         }

         //если найден конец многострочного комментария
         index = current->value.find("*/");
         if (index < current->value.size())
         {
            isComment = false;
            comment = current->value;
            comment.erase(index + 2, current->value.size());
            commentsFile << comment;

            current->value.erase(0, index + 2);

            index = current->value.find("/*");
            if (index < current->value.size())
            {
               comment.erase(0, index);
               commentsFile << comment;
            }
         }

         //если вся строка комментарий
         if (isComment == true)
         {
            comment = current->value;
            commentsFile << comment;

            current->value.clear();
         }

         commentsFile << endl;
         current = current->next;
      }
   }

private:
   Node* begin = nullptr;                 //адрес начального элемента
   Node* end   = nullptr;                 //адрес конечного элемента
};

int main()
{
   SetConsoleCP(1251);                    //ввод с консоли в кодировке 1251
   List file;                             //список (пока что пустой)
   bool choice = false;                   //хранит выбор метода ввода

   cout << "Read from file? 0 if no, something else if yes: ";
   cin >> choice;

   if (choice == true)
   {
      string fileName = "";
      cout << "Enter a File Name (for read): ";
      cin >> fileName;
      file.readFromFile(fileName);      //записываем строки из файла в список
   }
   else
   {
      file.readFromConsole();             //записываем строки из консоли в список
   }
   string fileNameComments = "", fileNameCode = "";
   cout << "Enter a File Name (for comments): ";
   cin >> fileNameComments;
   cout << "Enter a File Name (for code): ";
   cin >> fileNameCode;
   file.deleteComments(fileNameComments); //удаляем из списка комментарии, записывая их в файл
   ofstream code(fileNameCode);           //создаём поток для вывода кода без комментариев
   file.print(code);                      //запись в файл

   return 0;
}