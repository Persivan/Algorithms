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
            cout << "Error: " << ex.what() << endl;
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


class Field
{
private:
   struct Item                // Структура звена
   {
      int data;               // Значения звена
      bool marked;            // Метка для финальной цепочки
      bool path;              // Является ли цепочкой
      bool startPath;         // Является ли началом цепочки
      int pathNum;            // Номер чепочки
      Item() : data(0), marked(0), path(0), startPath(0), pathNum(0)
      {
      }
   };
   int pathCounter;
   int fieldSize;
   Item** field;
   // Рекурсивные функции
   void rPrint(Item**, int, int);                  // Вывод поля
   void rMagicFunc(int, int, int&, int&, int&);    // Решает задачу по алгоритму
   int rGetMaxPathLength(bool**, int, int);        // Считает для указанного элемента какой длины путь можно построить
   void rCreateMagicPath(int, int, int);           // Рисует кривую
public:
   Field(int size);
   // Обычный вывод всего поля
   void print();                                   // Печать всех полей
   void setValue(int, int, int);                   // Установка конкректного значения в конкретную точку (для дебага)
   bool elementDeadEnd(bool**, int, int);          // Тупик ли (когда все элементы вокруг отмечены)
   bool inRange(int, int, int);                    // Указанная координата в области ли игрового поля
   void createPath(int, int);                      // Интерфейс для создания цепочки

   // Запускающие рекурсивные функции
   void magicFunc();                               // Алгоритм по заданию
};

int main()
{
   cout << "Size: " << endl;
   int size = consoleInt();
   Field newField(size);
   newField.print();
   // Заполянем поле цепочками от пользователя
   cout << "Make paths (-1 for stop)" << endl;
   while (true)
   {
      //break;
      cout << "X coordinate: " << endl;
      int coorX = consoleInt();
      if (coorX == -1)
      {
         break;
      }
      cout << "Y coordinate: " << endl;
      int coorY = consoleInt();
      if (coorY == -1)
      {
         break;
      }
      /*cout << "Value[" << coorX << "," << coorY << "]: " << endl;
      int value = consoleInt();
      if (value == -1)
      {
         break;
      }*/
      //newField.setValue(coorX, coorY, value);
      newField.createPath(coorX, coorY);
   }
   newField.print();
   // Алгоритм решающий задачу
   newField.magicFunc();
   cout << "=============================================" << endl;
   newField.print();

}

void Field::rPrint(Item** field, int i, int j)
{
   // Переход на след строку
   if (j >= fieldSize)
   {
      cout << endl;
      i++;
      j = 0;
   }
   // Выход из рекурсии
   if (i >= fieldSize)
   {
      return;
   }
   // Вывод числа
   cout << field[i][j].data << "\t";
   // След число в строке
   j++;
   // Переход к след
   rPrint(field, i, j);
}

void Field::rMagicFunc(int y, int x, int& maxPathX, int& maxPathY, int& maxPathLength)
{
   // Переход на след строку
   if (x >= fieldSize)
   {
      y++;
      x = 0;
   }
   // Выход из рекурсии
   if (y >= fieldSize)
   {
      return;
   }
   // Вывод числа
   int maxPath = 0;
   // Если это середина пути пишем 0, для этого элемента нельзя построить
   if (field[y][x].path == true && field[y][x].startPath == false)
   {
      cout << "Checking max path for [" << y << "," << x << "](" << field[y][x].data << ") " << maxPath << endl;
   }
   else
   {
      // Создаем поле для меток
      bool** fieldMarks = new bool* [fieldSize];
      for (int i = 0; i < fieldSize; ++i)
      {
         fieldMarks[i] = new bool[fieldSize];
      }
      for (int i = 0; i < fieldSize; i++)
      {
         for (int j = 0; j < fieldSize; j++)
         {
            fieldMarks[i][j] = field[i][j].marked;
         }
      }
      // Получаем длину макс пути для текущего элемента
      maxPath = rGetMaxPathLength(fieldMarks, y, x);
      cout << "Checking max path for [" << y << "," << x << "](" << field[y][x].data << ") " << maxPath << endl;
      // Если нашли идеальный путь, занимающий все игровое поле, обрабатываем его
      if (maxPath == fieldSize * fieldSize)
      {
         cout << "Nice we found good place to start!" << endl;
         maxPathX = x;
         maxPathY = y;
         return;
      }
      // Если найденный на текущей итерации путь лучше чем найденный в прошлый раз
      if (maxPath > maxPathLength)
      {
         maxPathLength = maxPath;
         maxPathX = x;
         maxPathY = y;
      }
   }
   // Переход к след
   x++;
   rMagicFunc(y, x, maxPathX, maxPathY, maxPathLength);
}

int Field::rGetMaxPathLength(bool** fieldMarks, int y, int x)
{
   // Выходы
   // Если в тупике
   bool inRangeBottom = y + 1 < fieldSize;
   bool inRangeTop = y - 1 >= 0;
   bool inRangeLeft = x - 1 >= 0;
   bool inRangeRight = x + 1 < fieldSize;
   // Хотя бы одна внутри и отмечена
   bool checkBottom = (inRangeBottom && fieldMarks[y + 1][x]) || !inRangeBottom;
   bool checkTop = (inRangeTop && fieldMarks[y - 1][x]) || !inRangeTop;
   bool checkRight = (inRangeRight && fieldMarks[y][x + 1]) || !inRangeRight;
   bool checkLeft = (inRangeLeft && fieldMarks[y][x - 1]) || !inRangeLeft;
   if (checkBottom && checkTop && checkRight && checkLeft)
   {
      return 1;
   }
   // Обрабатываем текущий
   fieldMarks[y][x] = 1;
   // Ищем макс в глубину
   int maxLength = 0;
   // Если текущий элемент находится на пути, то ТАКОВ ПУТЬ
   if (field[y][x].path == true)
   {
      int currPathNum = field[y][x].pathNum;
      // Находим куда идет путь
      if (inRange(fieldSize, y + 1, x) && fieldMarks[y + 1][x] == false && field[y + 1][x].path == true && field[y + 1][x].pathNum == currPathNum)
      {
         maxLength = rGetMaxPathLength(fieldMarks, y + 1, x);
      }
      else if (inRange(fieldSize, y - 1, x) && fieldMarks[y - 1][x] == false && field[y - 1][x].path == true && field[y - 1][x].pathNum == currPathNum)
      {
         maxLength = rGetMaxPathLength(fieldMarks, y - 1, x);
      }
      else if (inRange(fieldSize, y, x + 1) && fieldMarks[y][x + 1] == false && field[y][x + 1].path == true && field[y][x + 1].pathNum == currPathNum)
      {
         maxLength = rGetMaxPathLength(fieldMarks, y, x + 1);
      }
      else if (inRange(fieldSize, y, x - 1) && fieldMarks[y][x - 1] == false && field[y][x - 1].path == true && field[y][x - 1].pathNum == currPathNum)
      {
         maxLength = rGetMaxPathLength(fieldMarks, y, x - 1);
      }
      else
      {
         // Переходим к след элементу (обход всегда начинаю вниз, а далее по часовой)
         // Если он внутри поля и (не пересекается с путем или пересекается но это начало пути
         if (inRange(fieldSize, y + 1, x) && (!field[y + 1][x].path || field[y + 1][x].startPath) && fieldMarks[y + 1][x] == false)
         {
            maxLength = rGetMaxPathLength(fieldMarks, y + 1, x);
         }
         if (inRange(fieldSize, y, x - 1) && (!field[y][x - 1].path || field[y][x - 1].startPath) && fieldMarks[y][x - 1] == false)
         {
            int newLen = rGetMaxPathLength(fieldMarks, y, x - 1);
            if (maxLength < newLen)
            {
               maxLength = newLen;
            }
         }
         if (inRange(fieldSize, y - 1, x) && (!field[y - 1][x].path || field[y - 1][x].startPath) && fieldMarks[y - 1][x] == false)
         {
            int newLen = rGetMaxPathLength(fieldMarks, y - 1, x);
            if (maxLength < newLen)
            {
               maxLength = newLen;
            }
         }
         if (inRange(fieldSize, y, x + 1) && (!field[y][x + 1].path || field[y][x + 1].startPath) && fieldMarks[y][x + 1] == false)
         {
            int newLen = rGetMaxPathLength(fieldMarks, y, x + 1);
            if (maxLength < newLen)
            {
               maxLength = newLen;
            }
         }
      }
   }
   // Иначе ищем максимальный
   else
   {
      // Переходим к след элементу (обход всегда начинаю вниз, а далее по часовой)
      // Если он внутри поля и (не пересекается с путем или пересекается но это начало пути
      if (inRange(fieldSize, y + 1, x) && (!field[y + 1][x].path || field[y + 1][x].startPath) && fieldMarks[y + 1][x] == false)
      {
         maxLength = rGetMaxPathLength(fieldMarks, y + 1, x);
      }
      if (inRange(fieldSize, y, x - 1) && (!field[y][x - 1].path || field[y][x - 1].startPath) && fieldMarks[y][x - 1] == false)
      {
         int newLen = rGetMaxPathLength(fieldMarks, y, x - 1);
         if (maxLength < newLen)
         {
            maxLength = newLen;
         }
      }
      if (inRange(fieldSize, y - 1, x) && (!field[y - 1][x].path || field[y - 1][x].startPath) && fieldMarks[y - 1][x] == false)
      {
         int newLen = rGetMaxPathLength(fieldMarks, y - 1, x);
         if (maxLength < newLen)
         {
            maxLength = newLen;
         }
      }
      if (inRange(fieldSize, y, x + 1) && (!field[y][x + 1].path || field[y][x + 1].startPath) && fieldMarks[y][x + 1] == false)
      {
         int newLen = rGetMaxPathLength(fieldMarks, y, x + 1);
         if (maxLength < newLen)
         {
            maxLength = newLen;
         }
      }
   }
   // Обрабатываем текущий
   return 1 + maxLength;
}

void Field::rCreateMagicPath(int y, int x, int value)
{
   // выход, если уже отмечено
   if (field[y][x].marked)
   {
      return;
   }

   // Обрабатываем текущий
   field[y][x].marked = 1;
   field[y][x].data = value++;

   // Создаем поле для меток
   bool** fieldMarks = new bool* [fieldSize];
   for (int i = 0; i < fieldSize; ++i)
   {
      fieldMarks[i] = new bool[fieldSize];
   }
   // Заполняем поле текущими
   for (auto i = 0; i < fieldSize; i++)
   {
      for (auto j = 0; j < fieldSize; j++)
      {
         fieldMarks[i][j] = field[i][j].marked;
      }
   }

   // Если текущий элемент находится на пути, то ТАКОВ ПУТЬ
   if (field[y][x].path == true)
   {
      int currPathNum = field[y][x].pathNum;
      // Находим куда идет путь
      if (inRange(fieldSize, y + 1, x) && field[y + 1][x].marked == false && field[y + 1][x].path == true && field[y + 1][x].pathNum == currPathNum)
      {
         rCreateMagicPath(y + 1, x, value);
      }
      else if (inRange(fieldSize, y - 1, x) && field[y - 1][x].marked == false && field[y - 1][x].path == true && field[y - 1][x].pathNum == currPathNum)
      {
         rCreateMagicPath(y - 1, x, value);
      }
      else if (inRange(fieldSize, y, x + 1) && field[y][x + 1].marked == false && field[y][x + 1].path == true && field[y][x + 1].pathNum == currPathNum)
      {
         rCreateMagicPath(y, x + 1, value);
      }
      else if (inRange(fieldSize, y, x - 1) && field[y][x - 1].marked == false && field[y][x - 1].path == true && field[y][x - 1].pathNum == currPathNum)
      {
         rCreateMagicPath(y, x - 1, value);
      }
      else
      {
         // Ищем макс в глубину
         int maxLength = 0, newX = 0, newY = 0;
         // Переходим к след элементу (обход всегда начинаю вниз, а далее по часовой)
         // Если он внутри поля и (не пересекается с путем или пересекается но это начало пути
         if (inRange(fieldSize, y + 1, x) && (!field[y + 1][x].path || field[y + 1][x].startPath) && fieldMarks[y + 1][x] == false)
         {
            maxLength = rGetMaxPathLength(fieldMarks, y + 1, x);
            newX = x;
            newY = y + 1;
         }
         if (inRange(fieldSize, y, x - 1) && (!field[y][x - 1].path || field[y][x - 1].startPath) && fieldMarks[y][x - 1] == false)
         {
            int newLen = rGetMaxPathLength(fieldMarks, y, x - 1);
            if (maxLength < newLen)
            {
               maxLength = newLen;
               newX = x - 1;
               newY = y;
            }
         }
         if (inRange(fieldSize, y - 1, x) && (!field[y - 1][x].path || field[y - 1][x].startPath) && fieldMarks[y - 1][x] == false)
         {
            int newLen = rGetMaxPathLength(fieldMarks, y - 1, x);
            if (maxLength < newLen)
            {
               maxLength = newLen;
               newX = x;
               newY = y - 1;
            }
         }
         if (inRange(fieldSize, y, x + 1) && (!field[y][x + 1].path || field[y][x + 1].startPath) && fieldMarks[y][x + 1] == false)
         {
            int newLen = rGetMaxPathLength(fieldMarks, y, x + 1);
            if (maxLength < newLen)
            {
               maxLength = newLen;
               newX = x + 1;
               newY = y;
            }
         }
         rCreateMagicPath(newY, newX, value);
      }
   }
   // Иначе ищем максимальный
   else
   {
      // Ищем макс в глубину
      int maxLength = 0, newX = 0, newY = 0;
      // Переходим к след элементу (обход всегда начинаю вниз, а далее по часовой)
      // Если он внутри поля и (не пересекается с путем или пересекается но это начало пути
      if (inRange(fieldSize, y + 1, x) && (!field[y + 1][x].path || field[y + 1][x].startPath) && fieldMarks[y + 1][x] == false)
      {
         maxLength = rGetMaxPathLength(fieldMarks, y + 1, x);
         newX = x;
         newY = y + 1;
      }
      if (inRange(fieldSize, y, x - 1) && (!field[y][x - 1].path || field[y][x - 1].startPath) && fieldMarks[y][x - 1] == false)
      {
         int newLen = rGetMaxPathLength(fieldMarks, y, x - 1);
         if (maxLength < newLen)
         {
            maxLength = newLen;
            newX = x - 1;
            newY = y;
         }
      }
      if (inRange(fieldSize, y - 1, x) && (!field[y - 1][x].path || field[y - 1][x].startPath) && fieldMarks[y - 1][x] == false)
      {
         int newLen = rGetMaxPathLength(fieldMarks, y - 1, x);
         if (maxLength < newLen)
         {
            maxLength = newLen;
            newX = x;
            newY = y - 1;
         }
      }
      if (inRange(fieldSize, y, x + 1) && (!field[y][x + 1].path || field[y][x + 1].startPath) && fieldMarks[y][x + 1] == false)
      {
         int newLen = rGetMaxPathLength(fieldMarks, y, x + 1);
         if (maxLength < newLen)
         {
            maxLength = newLen;
            newX = x + 1;
            newY = y;
         }
      }
      rCreateMagicPath(newY, newX, value);
   }
}

Field::Field(int size)
{
   field = new Item*[size];
   for (int i = 0; i < size; i++)
   {
      field[i] = new Item[size];
   }
   fieldSize = size;
   pathCounter = 0;
}

void Field::print()
{
   cout << "===Your field===" << endl;
   // Обычный
   //for (auto i = 0; i < fieldSize; i++)
   //{
   //   for (auto j = 0; j < field[i].size(); j++)
   //   {
   //      cout << field[i][j].data << " ";
   //   }
   //   cout << endl;
   //}
   // Рекурсивный вывод значений
   cout << "===Values===" << endl;
   rPrint(field, 0, 0);
   // Но это неудобно поэтому дальше обычный вывод
   cout << "===Marks===" << endl;
   for (auto i = 0; i < fieldSize; i++)
   {
      for (auto j = 0; j < fieldSize; j++)
      {
         cout << field[i][j].marked << " ";
      }
      cout << endl;
   }
   cout << "===Paths (1 - start/end, 2 - middle)===" << endl;
   for (auto i = 0; i < fieldSize; i++)
   {
      for (auto j = 0; j < fieldSize; j++)
      {
         if (field[i][j].startPath)
         {
            cout << "1 ";
         }
         else if (field[i][j].path)
         {
            cout << "2 ";
         }
         else
         {
            cout << "0 ";
         }
      }
      cout << endl;
   }
   cout << "===Paths===" << endl;
   for (auto i = 0; i < fieldSize; i++)
   {
      for (auto j = 0; j < fieldSize; j++)
      {
         cout << field[i][j].pathNum << " ";
      }
      cout << endl;
   }
}

void Field::setValue(int x, int y, int value)
{
   if (x >= fieldSize || y >= fieldSize)
   {
      cout << "Out of field[" << x << "," << y << "]: " << endl;
      return;
   }
   field[x][y].data = value;
}

bool Field::elementDeadEnd(bool** fieldMarks, int x, int y)
{
   int size = fieldSize;
   bool inRangeBottom = y + 1 < size;
   bool inRangeTop = y - 1 >= 0;
   bool inRangeLeft = x - 1 >= 0;
   bool inRangeRight = x + 1 < size;
   // Хотя бы одна внутри и отмечена
   bool checkBottom = (inRangeBottom && fieldMarks[y + 1][x]) || !inRangeBottom;
   bool checkTop = (inRangeTop && fieldMarks[y - 1][x]) || !inRangeTop;
   bool checkRight = (inRangeRight && fieldMarks[y][x + 1]) || !inRangeRight;
   bool checkLeft = (inRangeLeft && fieldMarks[y][x - 1]) || !inRangeLeft;
   return checkBottom && checkTop && checkRight && checkLeft;
}

bool Field::inRange(int fieldSize, int y, int x)
{
   bool inRangeBottom = y < fieldSize;
   bool inRangeTop = y >= 0;
   bool inRangeLeft = x >= 0;
   bool inRangeRight = x < fieldSize;
   return inRangeBottom && inRangeTop && inRangeLeft && inRangeRight;
}

void Field::createPath(int y, int x)
{
   if (!inRange(fieldSize, y, x))
   {
      cout << "Out of range!" << endl;
      return;
   }
   if (field[y][x].path == true)
   {
      cout << "Already path!" << endl;
      return;
   }
   pathCounter++;
   field[y][x].startPath = true;
   field[y][x].path = true;
   field[y][x].pathNum = pathCounter;
   int shiftX = 0, shiftY = 0;
   while (true)
   {
      // Показываем текущее поле
      cout << "===Paths===" << endl;
      for (auto i = 0; i < fieldSize; i++)
      {
         for (auto j = 0; j < fieldSize; j++)
         {
            cout << field[i][j].pathNum << " ";
         }
         cout << endl;
      }
      // Предлагаем куда то пойти
      cout << "Where you want to go? " << endl <<
         "1) down" << endl <<
         "2) right" << endl <<
         "3) up" << endl <<
         "4) left" << endl <<
         "5) end path" << endl;
      int comm = consoleInt();
      if (comm == 1)
      {
         shiftY++;
         // Проверяем а можем ли
         // Выход за границы
         if (!inRange(fieldSize, y + shiftY, x + shiftX))
         {
            cout << "Out of range, try again" << endl;
            shiftY--;
            continue;
         }
         // Попытка пересечь линию
         if (field[y + shiftY][x + shiftX].path == 1)
         {
            cout << "Cannot cross over path, try again" << endl;
            shiftY--;
            continue;
         }
         // Кусок кода дублируется
      }
      else if (comm == 2)
      {
         shiftX++;
         // Выход за границы
         if (!inRange(fieldSize, y + shiftY, x + shiftX))
         {
            cout << "Out of range, try again" << endl;
            shiftX--;
            continue;
         }
         // Попытка пересечь линию
         if (field[y + shiftY][x + shiftX].path == 1)
         {
            cout << "Cannot cross over path, try again" << endl;
            shiftX--;
            continue;
         }
         // Кусок кода дублируется
      }
      else if (comm == 3)
      {
         shiftY--;
         // Выход за границы
         if (!inRange(fieldSize, y + shiftY, x + shiftX))
         {
            cout << "Out of range, try again" << endl;
            shiftY++;
            continue;
         }
         // Попытка пересечь линию
         if (field[y + shiftY][x + shiftX].path == 1)
         {
            cout << "Cannot cross over path, try again" << endl;
            shiftY++;
            continue;
         }
         // Кусок кода дублируется
      }
      else if (comm == 4)
      {
         shiftX--;
         // Выход за границы
         if (!inRange(fieldSize, y + shiftY, x + shiftX))
         {
            cout << "Out of range, try again" << endl;
            shiftX++;
            continue;
         }
         // Попытка пересечь линию
         if (field[y + shiftY][x + shiftX].path == 1)
         {
            cout << "Cannot cross over path, try again" << endl;
            shiftX++;
            continue;
         }
         // Кусок кода дублируется
      }
      else if (comm == 5)
      {
         if (shiftX == 0 && shiftY == 0)
         {
            cout << "Path should be at least length 2" << endl;
            field[y][x].startPath = false;
            field[y][x].path = false;
            field[y][x].pathNum = 0;
         }
         else
         {
            field[y + shiftY][x + shiftX].startPath = true;
            field[y + shiftY][x + shiftX].pathNum = pathCounter;
         }
         break;
      }
      else
      {
         cout << "Unknown command" << endl;;
      }
      // Если все норм строим линию
      field[y + shiftY][x + shiftX].path = 1;
      field[y + shiftY][x + shiftX].pathNum = pathCounter;
   }
}

void Field::magicFunc()
{
   //field[0][1].marked = true;
   //field[1][0].marked = true;
   //cout << "0, 0 = " << elementDeadEnd(field, 0, 0) << endl;
   int maxPathX = 0;
   int maxPathY = 0;
   int maxPathLength = 0;
   //rMagicFunc(0, 0, maxPathX, maxPathY, maxPathLength);
   rCreateMagicPath(maxPathX, maxPathY, 0);
}
