#include <iostream>	
#include <string>				//Для stoi
#include <fstream>			//Для чтения файла
#include "backup2.h"


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


//------------------------------------
//-----Ввод из файла с проверкой------
//------------------------------------
int int_input_file(const std::string filename)
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

//------------------------------------
//-----Ввод из консоли с проверкой----
//------------------------------------
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
	List<int> list
	for (int i = 0; i < 10; i++)
	{
		list.addHead(i);
	}
	list.show();
	list.moveInCycle(int_input_console());					//Ввод производится из консоли
	//list.moveInCycle(int_input_file("input.txt"));	  //Ввод производится из консоли
	list.show();
	return 0;
}
