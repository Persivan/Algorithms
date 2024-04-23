#include <iostream>
#include <string>				//Для stoi
#include <fstream>			//Для чтения файла




template<typename T>								//Будем использовать шаблонный класс
class List	                              //Создаем тип данных Список
{
private:
	struct Item                            //Структура, являющаяся звеном списка
	{
		T data;                             //Значение
		Item* next;                         //Указатель на следующее звено списка
		Item* prev;                         //Указатель на предыдущее звено списка
	};
	Item* head;										//Указатель на начало списка
	Item* tail;										//Указатель на конец списка
	List::Item* create(const T&);				//Создание нового звена
public:
	List() : head(nullptr), tail(nullptr) {};	//Конструкторы
	List(const T&);
	~List();											//Деструктор
	void addTail(const T&);						//Добавление звена в хвост списка
	void addHead(const T&);						//Добавление звена в голову списка
	T removeHead();								//Удаление головного звена
	T removeTail();								//Удаление хвостового звена
	void show();									//Вывод списка на экран
	void moveInCycle(int);						//Сдвиг на k элементов вправо, int >= 0 
	bool isSym();									//Проверка является ли список симметричным
	void switchItem(int, int);					//Меняет элементы под позициями местами
	void matumbaFunc();						   //Заменяет элементы на ближайший больший,
														//Если такого нет то на 0
	T pop();											//Выдает голову и удаляет ее (типо стэк)
	T test();
};



template<typename T>
List<T>::List(const T& vl)
{
	head = create(vl);
	tail = head;
}

template<typename T>
List<T>::~List()
{
	while (head)
	{
		removeHead();
	}
}

template <typename T>
typename List<T>::Item* List<T>::create(const T& vl)
{
	Item* item = new Item;
	item->data = vl;
	item->next = nullptr;
	item->prev = nullptr;
	return item;
}

template<typename T>
void List<T>::addTail(const T& vl)
{
	if (tail && head)
	{
		tail->next = create(vl);
		tail->next->prev = tail;
		tail = tail->next;
	}
	else
	{
		head = create(vl);
		tail = head;
	}
}

template<typename T>
void List<T>::addHead(const T& vl)
{
	if (tail && head)
	{
		Item* temp = create(vl);
		temp->next = head;
		head = temp;
	}
	else
	{
		head = create(vl);
		tail = head;
	}

}

template<typename T>
T List<T>::removeHead()
{
	if (head)
	{
		Item* temp = head->next;
		T data = head->data;
		delete head;
		head = temp;
		return data;
	}
	else
	{
		return (T)0;
	}
}

template<typename T>
T List<T>::removeTail()
{
	if (tail)
	{
		Item* temp = tail->prev;
		T data = tail->data;
		delete tail;
		tail = temp;
		tail->next = nullptr;
		return data;
	}
	else
	{
		return (T)0;
	}
}


template<typename T>
void List<T>::show()
{
	Item* current = head;
	while (current)
	{
		std::cout << current->data << " ";
		current = current->next;
	}
	std::cout << std::endl;
}

template<typename T>
void List<T>::moveInCycle(int iterations)
{
	if (iterations < 0)
	{
		std::cout << "Number is < 0!" << std::endl;
	}
	for (int i = 0; i < iterations; i++)
	{
		addTail(head->data);
		removeHead();
	}
}

template<typename T>
bool List<T>::isSym()
{
	if (head != nullptr && tail != nullptr)						//Проверим существует ли список
	{																			//Список есть, тогда проверяем симметрию
		Item* current_left = head;										//Сохраним указатель списка с левой стороны
		Item* current_right = tail;									//Сохраним указатель списка с правой стороны
		while (current_left->data == current_right->data)		//Пока значения совпадают
		{
			if (current_left->next == nullptr || current_right->prev == nullptr)		//Проверяем существуют ли следующие элементы
			{
				break;
			}
			current_left = current_left->next;						//Сохраним следующий указатель списка с левой стороны
			current_right = current_right->prev;					//Сохраним следующий указатель списка с правой стороны
		}
		return (current_left->next == nullptr && current_right->prev == nullptr);	//Если указатели дошли до конца списка
																											//с двух стороны, то возвращаем 1
	}
	else
	{																			//Списка нет, тогда симметричности нет
		return 0;
	}
}

template<typename T>
void List<T>::switchItem(int posFirstElem, int posSecondElem)
{
	//Отсортируем
	if (posFirstElem > posSecondElem)
	{
		int temp = posFirstElem;
		posFirstElem = posSecondElem;
		posSecondElem = temp;
	}
	else if (posFirstElem == posSecondElem)
	{
		std::cout << "posFirstElem == posSecondElem" << std::endl;
		return;
	}
	//Защита на пустой список
	if (head == nullptr)
	{
		std::cout << "head == nullptr" << std::endl;
		return;
	}
	Item* firstElem = head;
	for (int i = 1; i < posFirstElem; i++)
	{
		//Защита от того что ввели позицию больше чем у нас элементов
		if (firstElem->next == nullptr)
		{
			std::cout << "firstElem->next == nullptr" << std::endl;
			return;
		}
		firstElem = firstElem->next;
	}
	Item* secondElem = firstElem;
	for (int i = posFirstElem; i < posSecondElem; i++)
	{
		//Защита от того что ввели позицию больше чем у нас элементов
		if (secondElem->next == nullptr)
		{
			std::cout << "secondElem->next == nullptr" << std::endl;
			return;
		}
		secondElem = secondElem->next;
	}
	T vl = firstElem->data;
	firstElem->data = secondElem->data;
	secondElem->data = vl;
}

template<typename T>
void List<T>::matumbaFunc()
{
	if (head == nullptr)				//Проверка на пустой
	{
		std::cout << "Your list is empty!" << std::endl;
		return;
	}
	//Начало работы функции, два указателя на начало списка
	Item* currentItem = head;		
	Item* nextItem = head;
	//Проходимся по всему списку
	while (currentItem != nullptr) {
		nextItem = currentItem;		//Начиная с текущего сравниваем со следующими
		while (nextItem != nullptr && currentItem->data >= nextItem->data)
		{
			nextItem = nextItem->next;
		}
		if (nextItem != nullptr)
		{
			currentItem->data = nextItem->data;
		}
		else
		{
			currentItem->data = 0;
		}
		currentItem = currentItem->next;
	}
}

template<typename T>
T List<T>::pop()
{
	if (head == nullptr)
	{
		std::cout << "Your list is empty!" << std::endl;			
		return T(); //Довольно костыльно возвращать NULL, если есть решение получше стоит его использовать
	}
	else
	{
		T current = head->data;
		removeHead();
		return current;
	}
}

template<typename T>
T List<T>::test()
{
	return T();
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

int int_console_input()
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

int console_input()
{
	int res = 0;
	std::string inStr = "";
	std::cout << "Enter a number ";
	std::cin >> inStr;
	res = std::stoi(inStr);
	return res;
}

int file_input(const std::string filename)
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

const int simple_console_input()
{
	int res = 0;
	std::cout << "Enter a number: ";
	std::cin >> res;
	return res;
}

const int int_input_console()
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

int main()
{
	matumbaClass explain(10);
	//explain.setValue(10);
	std::cout << explain.getValue();
	//std::string inputstr = "";
	//std::cout << "Enter string length: ";
	//int stringLength = 0;
	//std::cin >> stringLength;
	//for (int i = 0; i < stringLength; i++)
	//{
	//	std::cout << "Enter a char on " << i << " position: ";
	//	char inChar = ' ';
	//	inChar = int_console_input();
	//	inputstr += inChar;
	//}

	//List<int> stack;  // обьявил стэк
	////соответственно
	//for (int i = 0; i < 0; i++)
	//{ //потому что я ломал дискорд
	//	stack.addHead(i); //добавляем только в голову
	//}
	//stack.show();	//Вывод стэка
	//std::cout << stack.pop() << std::endl; //получаем эл из головы
	//std::cout << stack.pop() << std::endl; //получаем эл из головы
	//std::cout << stack.pop() << std::endl; //получаем эл из головы
	//std::cout << stack.pop() << std::endl; //получаем эл из головы
	return 0;
}
