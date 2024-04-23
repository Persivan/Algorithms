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
      char data;              // Значения звена
      bool marked;            // Метка для финальной цепочки
      bool hole;              // Дырки
      bool userNum;           // Введеное пользователем ли число
      Item() : data(0), marked(0), hole(0), userNum(0)
      {
      }
   };
   int fieldSize;             // Размер игрового поля
   Item** field;              // Игровое поле
   // Рекурсивные функции
   void rPrint(int, int);                  // Вывод поля (field.data)
   void rCreatePath(int, int, int);                // Строит цепочку
   int rGetMaxPathLength(char**&, int, int, int);   // Поиск лучших координат для следующего хода
   bool rCreatePath2(int, int, int);               // Строит цепочку по другому алгоритму
public:
   Field(int size);
   // Обычный вывод всего поля
   void print();                                   // Печать игровых полей
   void setValue(int, int, int);                   // Установка значения
   void createHole(int, int);                      // Создание дырки на игровом поле
   bool inRange(int, int, int);                    // Указанная координата в области ли игрового поля
   bool hasStart();                                // Проверка есть ли начальное значение (1)
   bool hasEnd();                                  // Проверка есть ли последнее значение (fieldSize^2 - количество дырок)
   char lastValue();                               // Узнает через количество дырок какое должно быть последнее значение
   bool checkValueAround(char**&, int&, int&, int&);  // Проверяет есть ли вокруг указанных координат число больше на 1, 
                                                      // если да записывает в y, x его координаты
   bool checkValueAround2(int&, int&, int&);          // Проверяет есть ли вокруг указанных координат число больше на 1, 
                                                      // если да записывает в y, x его координаты
   bool deadEnd(char**&, int&, int&);              // Проверят тупик
   bool deadEnd2(int&, int&, int&);                // Проверят тупик по другому алгоритму

   // Запускающие рекурсивные функции
   void createPath();                              // Функция решающая задачу
};

int main()
{
   cout << "Size: " << endl;
   int size = consoleInt();
   Field newField(size);
   newField.print();
   // Создаем оборваные края
   cout << "Create holes (-1 for stop)" << endl;
   while (true)
   {
      //break;
      cout << "Y coordinate: " << endl;
      int coorY = consoleInt();
      if (coorY == -1)
      {
         break;
      }
      cout << "X coordinate: " << endl;
      int coorX = consoleInt();
      if (coorX == -1)
      {
         break;
      }
      newField.createHole(coorY, coorX);
   }
   newField.print();
   // Заполянем поле значениями от пользователя
   cout << "Create numbers (-1 for stop)" << endl;
   while (true)
   {
      cout << "X coordinate: " << endl;
      int coorX = consoleInt();
      if (coorX == -1)
      {
         if (!newField.hasStart() || !newField.hasEnd())
         {
            cout << "You need to add 1 (where to start) and N (where to end)" << endl;
            continue;
         }
         break;
      }
      cout << "Y coordinate: " << endl;
      int coorY = consoleInt();
      if (coorY == -1)
      {
         if (!newField.hasStart() || !newField.hasEnd())
         {
            cout << "You need to add 1 (where to start) and N (where to end)" << endl;
            continue;
         }
         break;
      }
      cout << "Value[" << coorX << "," << coorY << "]: " << endl;
      int value = consoleInt();
      if (value == -1)
      {
         if (!newField.hasStart() || !newField.hasEnd())
         {
            cout << "You need to add 1 (where to start) and N (where to end)" << endl;
            continue;
         }
         break;
      }
      newField.setValue(coorX, coorY, value);
      newField.print();
   }
   cout << endl << endl << endl;
   newField.print();
   // Алгоритм решающий задачу
   newField.createPath();
   // Вывод после решения
   cout << "=============================================" << endl << endl;
   newField.print();

}

void Field::rPrint(int i, int j)
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
   if (field[i][j].hole)
   {
      cout << " " << "\t";
   }
   else
   {
      cout << (int)field[i][j].data << "\t";

   }
   // След число в строке
   j++;
   // Переход к след
   rPrint(i, j);
}

void Field::rCreatePath(int y, int x, int value)
{
   // выход, если уже отмечено
   if (field[y][x].marked)
   {
      return;
   }
   // Обрабатываем текущий узел
   field[y][x].marked = true;
   field[y][x].data = value;
   // Если рядом есть элемент value+1
   // Создаем копию поля информации
   char** fieldData = new char* [fieldSize];
   for (int i = 0; i < fieldSize; i++)
   {
      fieldData[i] = new char[fieldSize];
   }
   // Заполняем нулями
   for (int i = 0; i < fieldSize; i++)
   {
      for (int j = 0; j < fieldSize; j++)
      {
         if (field[i][j].marked)
         {
            fieldData[i][j] = field[i][j].data;
         }
         else
         {
            fieldData[i][j] = 0;
         }
      }
   }
   // Сама проверка
   if (checkValueAround(fieldData, y, x, value))
   {
      rCreatePath(y, x, value + 1);
   }
   else
   {
      // Ищем макс в глубину
      int maxLength = 0, newX = 0, newY = 0;
      bool flag = 0;
      // Проверяем 
      for (int i = -1; i <= 1; i++)
      {
         for (int j = -1; j <= 1; j++)
         {
            // Переход к след если это вне зоны или дырка или отмеченное значение (текущая точка уже отмечена)
            if (!inRange(fieldSize, y + i, x + j) || field[y + i][x + j].hole || field[y + i][x + j].marked)
            {
               continue;
            }
            // Если нашли значение больше maxLength
            int nextLength = rGetMaxPathLength(fieldData, y + i, x + j, value + 1);
            if (maxLength < nextLength)
            {
               newY = y + i;
               newX = x + j;
               maxLength = nextLength;
            }
            // Выход если нашли идеальный путь
            int emptyCounter = 0;
            for (int i = 0; i < fieldSize; i++)
            {
               for (int j = 0; j < fieldSize; j++)
               {
                  if (fieldData[i][j] == 0 && field[i][j].hole == 0)
                  {
                     emptyCounter++;
                  }
               }
            }
            if (maxLength == emptyCounter)
            {
               flag = 1;
               break;
            }
         }
         if (flag)
         {
            break;
         }
      }
      rCreatePath(newY, newX, value + 1);
   }
}

int Field::rGetMaxPathLength(char**& fieldDataFrom, int y, int x, int value)
{
   // Создаем копию поля информации
   char** fieldData = new char* [fieldSize];
   for (int i = 0; i < fieldSize; i++)
   {
      fieldData[i] = new char[fieldSize];
   }
   // Заполняем нулями
   for (int i = 0; i < fieldSize; i++)
   {
      for (int j = 0; j < fieldSize; j++)
      {
         fieldData[i][j] = fieldDataFrom[i][j];
      }
   }
   // Выходы
   // Если пришли в элемент заданный пользователем
   if (field[y][x].userNum)
   {
      // Если пришли с "правильным значением" пропускаем
      // Если с неправильным, возвращаем 0
      if (field[y][x].data != value)
      {
         return 0;
      }
   }
   // Если в тупике
   if (deadEnd(fieldData, y, x))
   {
      // Обработка текущего
      fieldData[y][x] = value;
      return 1;
   }
   
   // Обработка текущего
   fieldData[y][x] = value;
   // Обращение к следующему
   int maxLength = 0, newX = 0, newY = 0;


   // Если рядом есть элемент value+1
   if (checkValueAround(fieldData, y, x, value))
   {
      return 1 + rGetMaxPathLength(fieldData, y, x, value + 1);
   }
   else
   {
      // Ищем макс в глубину
      maxLength = 0;
      bool flag = 0;
      // Проверяем 
      for (int i = -1; i <= 1; i++)
      {
         for (int j = -1; j <= 1; j++)
         {
            // Переход к след если это вне зоны или дырка или 
            // отмеченное значение (текущая точка уже отмечена) 
            if (!inRange(fieldSize, y + i, x + j) || field[y + i][x + j].hole || fieldData[y + i][x + j] != 0)
            {
               continue;
            }
            // Если нашли значение больше maxLength
            int nextLength = rGetMaxPathLength(fieldData, y + i, x + j, value + 1);
            if (maxLength < nextLength)
            {
               newY = y + i;
               newX = x + j;
               maxLength = nextLength;
            }
            // Выход если нашли идеальный путь
            int emptyCounter = 0;
            for (int i = 0; i < fieldSize; i++)
            {
               for (int j = 0; j < fieldSize; j++)
               {
                  if (fieldData[i][j] == 0 && field[i][j].hole == 0)
                  {
                     emptyCounter++;
                  }
               }
            }
            if (maxLength == emptyCounter)
            {
               flag = 1;
               break;
            }
         }
         if (flag)
         {
            break;
         }
      }
   }
   // Обрабатываем текущий
   return 1 + maxLength;
}

bool Field::rCreatePath2(int y, int x, int value)
{
   // Если тупик
   if (deadEnd2(y, x, value))
   {
      // Проверяем удовлетворяет ли он решению
      if (field[y][x].data == value && value == lastValue())
      {
         field[y][x].data = value;
         field[y][x].marked = 1;
         return true;
      }
      else
      {
         return false;
      }
   }
   // Если рядом есть следующий элемент без раздумий идем в него
   if (checkValueAround2(y, x, value))
   {
      // Если следующие значения не дошли до lastValue(), то эта неправильная ветка, выходим из нее!
      if (!rCreatePath2(y, x, value + 1))
      {
         field[y][x].data = 0;
         field[y][x].marked = 0;
      }
   }
   //Обрабатываем текущий
   field[y][x].data = value;
   field[y][x].marked = 1;
   // Переходим к следующему
   bool goodElem = 0; // Здесь будет храниться результат проверки для текущего элемента
   for (int i = -1; i <= 1; i++)
   {
      for (int j = -1; j <= 1; j++)
      {
         // подставляйте везде НЕ 
         // если это вне зоны или 
         // дырка или 
         // отмеченное значение или
         // текущее или
         // не следующее значение
         if (!(!inRange(fieldSize, y + i, x + j) || 
            field[y + i][x + j].hole || 
            field[y + i][x + j].marked || 
            (i == 0 && j ==0) ||
            (field[y + i][x + j].userNum && field[y + i][x + j].data != value + 1)))
         {
            // Если следующие значения не дошли до lastValue(), то эта неправильная ветка, выходим из нее!
            if (value == 4)
            {
               cout << "";
            }
            if (!rCreatePath2(y + i, x + j, value + 1))
            {
               field[y + i][x + j].data = 0;
               field[y + i][x + j].marked = 0;
            }
            else
            {
               goodElem = 1;
            }
         }
         
      }
   }
   if (!goodElem)
   {
      field[y][x].data = 0;
      field[y][x].marked = 0;
      return false;
   }
   

}

Field::Field(int size)
{
   field = new Item * [size];
   for (int i = 0; i < size; i++)
   {
      field[i] = new Item[size];
   }
   fieldSize = size;
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
   rPrint(0, 0);
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
   cout << "===Holes (1 - yes, 0 - no)===" << endl;
   for (auto i = 0; i < fieldSize; i++)
   {
      for (auto j = 0; j < fieldSize; j++)
      {
         cout << field[i][j].hole << " ";
      }
      cout << endl;
   }
}

void Field::setValue(int x, int y, int value)
{
   if (!inRange(fieldSize, y, x))
   {
      cout << "Out of field[" << x << "," << y << "]: " << endl;
      return;
   }
   if (field[x][y].hole)
   {
      cout << "There is a hole[" << x << "," << y << "]: " << endl;
      return;
   }
   cout << "Value created" << endl;
   field[x][y].userNum = true;
   field[x][y].data = value;
}

void Field::createHole(int y, int x)
{
   if (!inRange(fieldSize, y, x))
   {
      cout << "Out of range!" << endl;
      return;
   }
   cout << "Hole created!" << endl;
   field[y][x].hole = true;
}

bool Field::inRange(int fieldSize, int y, int x)
{
   bool inRangeBottom = y < fieldSize;
   bool inRangeTop = y >= 0;
   bool inRangeLeft = x >= 0;
   bool inRangeRight = x < fieldSize;
   return inRangeBottom && inRangeTop && inRangeLeft && inRangeRight;
}

bool Field::hasStart()
{
   bool flag = false;
   for (int i = 0; i < fieldSize; i++)
   {
      for (int j = 0; j < fieldSize; j++)
      {
         if (field[i][j].data == 1)
         {
            flag = 1;
         }
      }
   }
   return flag;
}

bool Field::hasEnd()
{
   // Создаем флаг
   bool flag = false;
   // Узнаем какое значение должно быть последним
   char lastValueShouldBe = lastValue();
   // Проверяем есть ли последнее число
   for (int i = 0; i < fieldSize; i++)
   {
      for (int j = 0; j < fieldSize; j++)
      {
         if (field[i][j].data == lastValueShouldBe)
         {
            flag = 1;
         }
      }
   }
   return flag;
}

char Field::lastValue()
{
   // Считаем количество дырок
   int holeCounter = 0;
   for (int i = 0; i < fieldSize; i++)
   {
      for (int j = 0; j < fieldSize; j++)
      {
         if (field[i][j].hole == 1)
         {
            holeCounter++;
         }
      }
   }
   return fieldSize * fieldSize - holeCounter;
}

bool Field::checkValueAround(char**& fieldData, int& y, int& x, int& value)
{
   // Проверяем 
   for (int i = -1; i <= 1; i++)
   {
      for (int j = -1; j <= 1; j++)
      {
         // Переход к след если это вне зоны или дырка или отмеченное значение
         if (!inRange(fieldSize, y + i, x + j) || field[y + i][x + j].hole || fieldData[y + i][x + j] != 0)
         {
            continue;
         }
         // Если нашли значение
         if (field[y + i][x + j].data == value + 1)
         {
            y = y + i;
            x = x + j;
            return true;
         }
      }
   }
   return false;
}

bool Field::checkValueAround2(int& y, int& x, int& value)
{
   // Проверяем 
   for (int i = -1; i <= 1; i++)
   {
      for (int j = -1; j <= 1; j++)
      {
         // Переход к след если это вне зоны или дырка или отмеченное значение
         if (!inRange(fieldSize, y + i, x + j) || field[y + i][x + j].hole || field[y + i][x + j].data != 0)
         {
            continue;
         }
         // Если нашли значение
         if (field[y + i][x + j].data == value + 1)
         {
            y = y + i;
            x = x + j;
            return true;
         }
      }
   }
   return false;
}

bool Field::deadEnd(char**& fieldData, int& y, int& x)
{
   // Проверка вокруг
   // Проверяем 
   for (int i = -1; i <= 1; i++)
   {
      for (int j = -1; j <= 1; j++)
      {
         // Переход к след если это вне зоны или дырка или отмеченное значение
         if (!inRange(fieldSize, y + i, x + j) || field[y + i][x + j].hole || fieldData[y + i][x + j] != 0 || (i == 0 && j ==0))
         {
            continue;
         }
         // Если нашли
         else
         {
            return false;
         }
      }
   }
   return true;
}

bool Field::deadEnd2(int& y, int& x, int& value)
{
   // Проверка вокруг
   for (int i = -1; i <= 1; i++)
   {
      for (int j = -1; j <= 1; j++)
      {
         // Переход к след если это вне зоны или дырка или отмеченное значение
         if (!inRange(fieldSize, y + i, x + j) || field[y + i][x + j].hole || field[y + i][x + j].marked || (i == 0 && j == 0) || 
            (field[y + i][x + j].userNum && field[y + i][x + j].data != value + 1)) // Неотмеченное пользовательское число которое больше текущего более чем на 1
         {
            continue;
         }
         // Если нашли
         else
         {
            return false;
         }
      }
   }
   return true;
}

void Field::createPath()
{
   cout << "Loading..." << endl;
   // Найдем начало цепочки
   int coorX = 0, coorY = 0;
   for (int i = 0; i < fieldSize; i++)
   {
      for (int j = 0; j < fieldSize; j++)
      {
         if (field[i][j].data == 1)
         {
            coorX = j;
            coorY = i;
         }
      }
   }
   cout << "Found start at (" << coorY << "," << coorX << ")" << endl;
   // Запускаем рекурсивную функцию строящую цепочку
   // Создаем копию поля информации
   char** fieldData = new char* [fieldSize];
   for (int i = 0; i < fieldSize; i++)
   {
      fieldData[i] = new char[fieldSize];
   }
   // Заполняем нулями
   for (int i = 0; i < fieldSize; i++)
   {
      for (int j = 0; j < fieldSize; j++)
      {
         fieldData[i][j] = 0;
      }
   }
   //cout << "Best length = " << rGetMaxPathLength(fieldData, coorY, coorX, 1) << endl;
   //rCreatePath(coorY, coorX, 1);
   cout << "is path created? " << rCreatePath2(coorY, coorX, 1) << endl;
}
