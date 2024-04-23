#pragma once

template <typename T>
class List {
private:
	struct item {
		item() {}
		item(T val) { value = val; }
		void set(T val) { value = val; }

		T value;
		item* next = nullptr;
	};
	item* start = nullptr;
	item* end = nullptr;
	int len = 0;
public:
	void swap(int n1, int n2) {
		T t = this->get(n1);
		this->set(n1, this->get(n2));
		this->set(n2, t);
	}

	void reverse(int start, int end) {
		for (int i = 0; i < (end - start) / 2; i++)
			swap(start + i, end - i);
	}

	void push_back(T val) {
		item* n = new item(val);
		if (len == 0) {
			start = n;
			end = start;
		}
		else {
			end->next = n;
			end = n;
		}
		len++;
	}

	void push_back() {
		T val;
		push_back(val);
	}

	void setSize(int s) {
		for (int i = 0; i < s; i++)
			push_back();
	}

	void setSize(int s, T val) {
		for (int i = 0; i < s; i++)
			push_back(val);
	}

	T& operator[] (int n) {
		item* curr = start;
		if (n >= len || n < 0) exit(-1);
		for (int i = 0; i < n; i++)
			curr = curr->next;
		return curr->value;
	}

	void set(int n, T val) {
		item* curr = start;
		if (n >= len || n < 0) exit(-1);
		for (int i = 0; i < n; i++)
			curr = curr->next;
		curr->set(val);
	}

	T get(int n) {
		item* curr = start;
		if (n >= len || n < 0) exit(-1);
		for (int i = 0; i < n; i++)
			curr = curr->next;
		return curr->value;
	}

	int size() {
		return len;
	}

	void sort() {
		for (int i = 0; i < len - 1; i++)
			for (int k = i + 1; k < len; k++)
				if (this->get(i) > this->get(k)) this->swap(i, k);
	}

	List<T> copy() {
		List<T> list;
		for (int i = 0; i < len; i++)
			list.push_back(this->get(i));
		return list;
	}

	List<T> copy(int start, int end) {
		List<T> list;
		for (int i = start; i < end; i++)
			list.push_back(this->get(i));
		return list;
	}

	void print() {
		for (int i = 0; i < len; i++)
			std::cout << this->get(i) << " ";
		std::cout << std::endl;
	}

	void clear() {
		if (len > 0) {
			item* next = start->next;
			for (int i = 0; i < len; i++) {
				delete start;
				start = next;
				if (next != nullptr) next = start->next;
			}
		}
	}
};