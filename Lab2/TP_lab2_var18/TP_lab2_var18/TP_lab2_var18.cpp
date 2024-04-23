#include "iostream"
#include "conio.h"
#include <stdio.h>
#include <string>

using namespace std;

struct mas
{
	//Массив значений по столбцам слева направо
	int A;
	//Номера строк
	int LI;
	mas* next;
};

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



//Функция создания матрицы
int** create1(int m, int n, int** arr)//функция создания матрицы
{
	cout << "\nEnter your matrix:\n";
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			arr[i][j] = consoleInt();
		}
	}
	cout << endl;
	return arr;
}

//Функция циклического сдвига столбцов
int** shiftRight(int n, int m, int** arr, int** arr1)
{
	int shift;
	cout << "Enter length of cycle: ";
	while (!(cin >> shift) || (cin.peek() != '\n') || (shift < 1)) {
		cin.clear();
		while (cin.get() != '\n');
		cout << "Error! Try again: shift = ";
	}

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			arr1[i][j] = arr[i][j];
		}
	}

	//Циклический сдвиг столбцов на shift
	for (int i = 0; i < m; i++) {
		for (int start_j = 0, k = 0; k < n; start_j++) {

			int temp_0 = arr1[i][start_j];
			for (int j = (start_j + shift) % n; j != start_j; j = (j + shift) % n, k++) {
				int temp_1 = arr1[i][j];
				arr1[i][j] = temp_0;
				temp_0 = temp_1;
			}
			arr1[i][start_j] = temp_0;
			k++;
		}
	}
	return arr1;
}

//функция формирования массивов
int create(mas* X, int* LJ, int m, int n, int** arr)
{
	int i, j, num, k, f;
	mas* z = X, * p, * q;

	num = 0;
	z->A = 0;
	p = z;
	q = z;

	//Формирование массивов A, LI, LJ
	for (j = 0; j < n; j++)
	{
		f = 0;
		for (i = 0; i < m; i++)
		{
			//если ненулевой элемент
			if (arr[i][j] != 0)
			{
				if (f == 0)
				{
					LJ[j] = num;
					f = 1;
				}
				//если вводится самый первый элемент
				if (!z->A)
				{
					z->A = arr[i][j];
					z->LI = i;
					z->next = 0;
					num++;
				}
				else
				{
					z->next = new mas;
					z = z->next;
					z->A = arr[i][j];
					z->LI = i;
					z->next = 0;
					num++;
				}
			}
		}

	}
	//вывод ненулевых элементов
	cout << "A: ";
	while (p != 0)
	{
		cout << p->A << " ";
		p = p->next;
	}
	cout << "\n";
	//вывод LI (индекс i каждого ненулевого элемента)
	cout << "LI: ";
	while (q != 0)
	{
		cout << q->LI << " ";
		q = q->next;
	}
	cout << "\n";
	//вывод LJ(местоположения 1 ненул. эл—та в каждом столбце)
	cout << "LJ: ";
	LJ[n] = num;
	for (j = 0; j <= n; j++)
		cout << LJ[j] << " ";
	return 0;
}

//Функция вывода матрицы
void print(mas* X, int* LJ, int m, int n, int** arr) //функция вывода матрицы
{
	mas* p;
	p = X;
	int i, j, k, f;
	k = 0;

	for (j = 0; j < n; j++)
	{
		// число ненулевых элементов в строке
		f = LJ[j + 1] - LJ[j];
		k = 0;
		for (i = 0; i < m; i++)
			if ((p->LI == i) && (k < f))
			{
				arr[i][j] = p->A;
				if (p->next) p = p->next;
				k++;
			}
			else
			{
				arr[i][j] = 0;
			}
	}

	// Вывод
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			cout << arr[i][j] << ' ';
		}
		cout << "\n";
	}
	cout << endl;
}


int main()
{
	setlocale(LC_ALL, "Russian");
	int m, n, * LJ;
	mas* X;
	int number;
	cout << "Как вы хотите ввести данные?" << endl;
	cout << "1: Заполнить от руки" << endl << "2: Ввод из файла " << endl;
	cout << "Number = ";
	while (!(cin >> number) || (cin.peek() != '\n') || (number < 1))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Error! Try again: number = ";
	}
	cout << "- - - - - - - - - - - - - - - - - - " << endl;

	//Заполнение от руки
	if (number == 1) {
		cout << "Enter number of rows and columns:\n";
		cout << "rows = ";
		//защита от неверного ввода данных
		while (!(cin >> m) || (cin.peek() != '\n') || (m < 1)) {
			cin.clear();
			while (cin.get() != '\n');
			cout << "Error! Try again: rows = ";
		}
		cout << "columns = ";
		while (!(cin >> n) || (cin.peek() != '\n') || (n < 1)) {
			cin.clear();
			while (cin.get() != '\n');
			cout << "Error! Try again: columns = ";
		}

		int** arr;
		arr = new int* [m];
		for (int i = 0; i < m; i++)
		{
			arr[i] = new int[n];
		}

		int** arr1;
		arr1 = new int* [m];
		for (int i = 0; i < m; i++)
		{
			arr1[i] = new int[n];
		}

		X = new mas;
		LJ = new int[n + 1];

		create1(m, n, arr);
		cout << "Your matrix:\n";
		create(X, LJ, m, n, arr);
		cout << "\n- - - - - - - - - -\n";
		print(X, LJ, m, n, arr);

		shiftRight(n, m, arr, arr1);
		cout << "Your matrix after cyclic shift:\n";
		create(X, LJ, m, n, arr1);
		cout << "\n- - - - - - - - - -\n";
		print(X, LJ, m, n, arr1);

	}
	//Считывание из файла
	else
	{
		FILE* in;
		//Открытие файла на чтение
		fopen_s(&in, "input.txt", "rt");
		if (!in)
		{
			cout << "Файл не найден!" << endl;
			return -1;
		}
		//считываем первые элементы
		fscanf_s(in, "%d", &m);
		fscanf_s(in, "%d", &n);

		int** arr;
		arr = new int* [m];
		for (int i = 0; i < m; i++)
			arr[i] = new int[n];

		int** arr1;
		arr1 = new int* [m];
		for (int i = 0; i < m; i++)
			arr1[i] = new int[n];

		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				fscanf_s(in, "%d", &arr[i][j]);
			}
		}

		X = new mas;
		LJ = new int[n + 1];

		cout << "Your matrix:\n";
		create(X, LJ, m, n, arr);
		cout << "\n- - - - - - - - - -\n";
		print(X, LJ, m, n, arr);

		shiftRight(n, m, arr, arr1);
		cout << "Your matrix after cyclic shift:\n";
		create(X, LJ, m, n, arr1);
		cout << "\n- - - - - - - - - -\n";
		print(X, LJ, m, n, arr1);
	}
	return 0;
}
