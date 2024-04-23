#pragma once
#include "List.h"

template<typename T>
class Tree {
private:
	struct item {
		item(){}
		item(T val) { value = val; }
		bool isChild() {
			if (left == nullptr && right == nullptr)
				return false;
			return true;
		}

		T value;
		item* left = nullptr;
		item* right = nullptr;
	};
	item* root = nullptr;

	void find(T val, item* leaf, int* count) {
		if (val == leaf->value)
			*count += 1;
		if (val < leaf->value)
			if (leaf->left != nullptr)
				find(val, leaf->left, count);
		if (val >= leaf->value)
			if (leaf->right != nullptr)
				find(val, leaf->right, count);
		if (!leaf->isChild())
			return;
	}

	void getAllValues(item* leaf, List<T>* arr) {
		arr->push_back(leaf->value);
		if (leaf->left != nullptr)
			getAllValues(leaf->left, arr);
		if (leaf->right != nullptr)
			getAllValues(leaf->right, arr);
		if (!leaf->isChild())
			return;
	}

	void push(item* currLeaf, T val) {
		if (val < currLeaf->value)
			if (currLeaf->left == nullptr)
				currLeaf->left = new item(val);
			else
				push(currLeaf->left, val);
		else
			if (currLeaf->right == nullptr)
				currLeaf->right = new item(val);
			else
				push(currLeaf->right, val);
	}

	void clear(item* curr) {
		if (curr->left != nullptr)
			clear(curr->left);
		if (curr->right != nullptr)
			clear(curr->right);
		delete curr;
	}
public:
	void push(T val) {
		if (root == nullptr)
			root = new item(val);
		else
			push(root, val);
	}

	void toArray(List<int>* arr) {
		getAllValues(root, arr);
	}

	void find(T val, int* count) {
		this->find(val, root, count);
	}

	void clear() {
		if (root != nullptr)
			this->clear(root);
		root = nullptr;
	}
};