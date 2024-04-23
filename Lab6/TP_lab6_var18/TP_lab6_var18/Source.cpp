#include "iostream"
#include "conio.h"
#include <stdio.h>

using namespace std;

struct mas
{
	//������ �������� �� �������� ����� �������
	int A;
	//������ �����
	int LI;
	mas* next;
};

int create(mas*, int*, int, int, int**);
int** create1(int, int, int**);
int** shiftRight(int, int, int**, int**);
void print(mas*, int*, int, int, int**);

int main()
{
	setlocale(LC_ALL, "Russian");
	int m, n, * LJ;
	mas* X;
	int number;
	cout << "��� �� ������ ������ ������?" << endl;
	cout << "1: ��������� �� ����" << endl << "2: ���� �� ����� " << endl;
	cout << "Number = ";
	while (!(cin >> number) || (cin.peek() != '\n') || (number < 1))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Error! Try again: number = ";
	}
	cout << "- - - - - - - - - - - - - - - - - - " << endl;

	//���������� �� ����
	if (number == 1) {
		cout << "Enter number of rows and columns:\n";
		cout << "rows = ";
		//������ �� ��������� ����� ������
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
			arr[i] = new int[n];

		int** arr1;
		arr1 = new int* [m];
		for (int i = 0; i < m; i++)
			arr1[i] = new int[n];
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
	//���������� �� �����
	else
	{
		FILE* in;
		//�������� ����� �� ������
		fopen_s(&in, "input.txt", "rt");
		if (!in)
		{
			cout << "���� �� ������!" << endl;
			return -1;
		}
		//��������� ������ ��������
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

//������� �������� �������
int** create1(int m, int n, int** arr)//������� �������� �������
{
	cout << "\nEnter your matrix:\n";
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> arr[i][j];
		}
	}
	cout << endl;
	return arr;
}

//������� ������������ ������ ��������
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

	//����������� ����� �������� �� shift
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

//������� ������������ ��������
int create(mas* X, int* LJ, int m, int n, int** arr)
{
	int i, j, num, k, f;
	mas* z = X, * p, * q;

	num = 0;
	z->A = 0;
	p = z;
	q = z;

	//������������ �������� A, LI, LJ
	for (j = 0; j < n; j++)
	{
		f = 0;
		for (i = 0; i < m; i++)
		{
			//���� ��������� �������
			if (arr[i][j] != 0)
			{
				if (f == 0)
				{
					LJ[j] = num;
					f = 1;
				}
				//���� �������� ����� ������ �������
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
	//����� ��������� ���������
	cout << "A: ";
	while (p != 0)
	{
		cout << p->A << " ";
		p = p->next;
	}
	cout << "\n";
	//����� LI (������ i ������� ���������� ��������)
	cout << "LI: ";
	while (q != 0)
	{
		cout << q->LI << " ";
		q = q->next;
	}
	cout << "\n";
	//����� LJ(�������������� 1 �����. ���� � ������ �������)
	cout << "LJ: ";
	LJ[n] = num;
	for (j = 0; j <= n; j++)
		cout << LJ[j] << " ";
	return 0;
}

//������� ������ �������
void print(mas* X, int* LJ, int m, int n, int** arr) //������� ������ �������
{
	mas* p;
	p = X;
	int i, j, k, f;
	k = 0;

	for (j = 0; j < n; j++)
	{
		// ����� ��������� ��������� � ������
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

	// �����
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
