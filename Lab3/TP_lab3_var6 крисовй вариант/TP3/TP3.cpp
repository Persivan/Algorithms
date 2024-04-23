#include <iostream>
#include <fstream>
#include "Tree.h"
#include "List.h"

using namespace std;


int* readInt() {
    string instr = "";
    int res = 0;
    while (true) {
        cin >> instr;
        if (instr == "-") return nullptr;
        try {
            res = atoi(instr.c_str());
            break;
        }
        catch (const exception& ex) {
            cout << "Again!!!!!!" << endl;
        }
    }
    return &res;
};

int* readInt(ifstream* file) {
    string instr = "";
    int res = 0;
    while (true) {
        *file >> instr;
        if (instr == "-") return nullptr;
        try {
            res = atoi(instr.c_str());
            break;
        }
        catch (const exception& ex) {}
    }
    return &res;
}

void readFromFile(ifstream* file, List<int>* arr, int* whatToFind) {
    bool flag = false;
    while (!file->eof()) {
        int* n = readInt(file);
        if (flag) {
            *whatToFind = *n;
            return;
        }
        if (n == nullptr)
            flag = true;
        else
            arr->push_back(*n);
    }
}

void readFromConsole(List<int>* arr, int* whatToFind) {
    cout << "Enter values, print ' - ' in the and" << endl;
    int* n = readInt();
    while (n != nullptr) {
        arr->push_back(*n);
        n = readInt();
    }
    *whatToFind = *readInt();
}

int main() {
    Tree<int> tree = Tree<int>();
    List<int> arr = List<int>();

    int isConsoleInput = -1;
    cout << "0: console input\n1: file input" << endl;
    while (true) {
        isConsoleInput = *readInt();
        if (isConsoleInput != 0 && isConsoleInput != 1)
            cout << "\n0: console input\n1: file input" << endl;
        else
            break;
    }
    
    int whatToFind = 0;

    ifstream file("input.txt");
    isConsoleInput ? readFromFile(&file, &arr, &whatToFind) : readFromConsole(&arr, &whatToFind);
    cout << "Input: ";
    arr.print();
    cout << "What to find: " << whatToFind << endl;

    for (int i = 0; i < arr.size(); i++)
        tree.push(arr[i]);

    int k = 0;
    tree.find(whatToFind, &k);
    cout << endl << "Answer: " << k << endl;

    tree.clear();
}