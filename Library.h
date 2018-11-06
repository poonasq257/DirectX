#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
using namespace std;

template<typename TYPE>
class Position 
{
public:
	Position() : ptr(nullptr) {}
	Position(TYPE* ptr) : ptr(ptr) {}
	Position(Position& other) : ptr(other.ptr) {}
public:
	void operator=(Position& other) { ptr = other.ptr; }
	void operator++() { ptr++; }
	void operator--() { ptr--; }
	Position& operator+(int size) 
	{
		ptr += size;
		return *this; 
	}
	Position& operator+(const Position& other) 
	{
		this->ptr += other.ptr;
		return *this;
	}
	Position& operator-(int size)
	{
		ptr -= size;
		return *this;
	}
	int operator-(const Position& other) { return this->ptr - other.ptr; }
	bool operator==(const Position& other) { return this->ptr == other.ptr; }
	bool operator!=(const Position& other) { return this->ptr != other.ptr; }
	TYPE* operator*() { return ptr; }
private:
	TYPE* ptr;
};

// Linked List Class based on Dummy Node 
template<typename TYPE>
class List
{
public:
	struct Node
	{
		TYPE data;
		Node* next;
	};
	using POSITION = Position<Node>;
public:
	List() : head(new Node()), tail(head), count(0) {}
	~List() { delete head; }
public:
	POSITION begin() { return POSITION(head); }
	POSITION end() { return POSITION(tail); }

	void Add(TYPE& data)
	{
		Node* newNode = new Node();
		newNode->data = data;
		tail->next = newNode;
		tail = newNode;
		count++;
	}
	void Remove(POSITION& pt)
	{
		Node* rNode = *pt;
		Node* prev = head;
		Node* cur = head->next;

		while (cur != nullptr)
		{
			if (rNode == cur) break;
			prev = cur;
			cur = cur->next;
		}
		if (cur == nullptr) return;

		prev->next = cur->next;
		delete rNode;
		count--;
	}
	void Remove(int idx)
	{
		if (idx >= count || idx < 0) return;

		Node* prev = head;
		Node* rNode = head->next;

		while (idx--)
		{
			prev = rNode;
			rNode = rNode->next;
		}
		prev->next = rNode->next;
		if (rNode == tail) tail = prev;

		delete rNode;
		count--;
	}
	void RemoveAll()
	{
		if (head->next == nullptr) return;

		Node* cur = head->next;
		Node* rNode;

		while (cur != nullptr)
		{
			rNode = cur;
			cur = cur->next;
			delete rNode;
		}

		count = 0;
	}
	POSITION GetHeadPosition() { return POSITION(head->next); }
	TYPE& GetNext(POSITION& pt)
	{
		Node* temp = *pt;
		pt = POSITION(temp->next);
		return temp->data;
	}
	int Count() const { return count; }
private:
	Node* head;
	Node* tail;
	size_t count;
};

// vector class 
template<typename TYPE>
class Vector
{
public:
	using POSITION = Position<TYPE>;
public:
	Vector() : capacity(0), count(0), base(nullptr) {}
	Vector(int size) : capacity(size), count(0), base(new TYPE[SIZE]) {}
	~Vector() { delete[] base; }
public:
	POSITION begin() { return POSITION(&base[0]); }
	POSITION end() { return POSITION(&base[count]); }

	TYPE& Front() { return base[0]; }
	TYPE& Back() { return base[count]; }

	int Count() const { return count; }
	int Capacity() const { return capacity; }
	bool IsEmpty() const { return count == 0; }

	void Swap(Vector& other)
	{
		Vector temp = other;

		other.capacity = this->capacity;
		other.count = this->count;
		other.base = this->base;

		this->capacity = temp.capacity;
		this->count = temp.count;
		this->base = temp.base;

		temp = Vector(); // ?
	}
	void Clear() { count = 0; }
	void Erase(POSITION& pt) // tempary
	{
		TYPE* rData = *pt;

		for (size_t i = 0; i < count; i++)
		{
			if (&base[i] == rData)
			{
				for (size_t j = i; j < count - 1; j++) base[j] = base[j + 1];
				count--;
				return;
			}
		}

		throw "Invalid Index";
	}
	void Insert(POSITION& pt, const TYPE& data)
	{
		int pos = pt - begin();
		if (pos < 0 || pos >= capacity)
		{
			throw "Invalid Index";
			return;
		}

		base[pos] = data;
		count++;
	}
	void Pop() { Erase(end() - 1); }
	void Push(const TYPE& data)
	{
		if (count == capacity) Reserve(capacity + 1); 
		Insert(end(), data);
	}
	void Resize(size_t size, const TYPE& val = 0)
	{
		if (count == size) return;
		else if (size < capacity) count = size;
		else
		{
			while (count < size) base[count++] = val;
		}
	}
	void Reserve(size_t size)
	{
		if (size <= capacity) return;

		TYPE* newSpace = new TYPE[size];
		for (size_t i = 0; i < capacity; i++) newSpace[i] = base[i];
		
		delete[] base;
		base = newSpace;
		capacity = size;
	}
public:
	TYPE& operator[](int idx) 
	{
		if(idx < count) return base[idx]; 
		else throw "Invalid Index";
	}
private:
	size_t capacity;
	size_t count;
	TYPE* base;
};

template<typename TYPE>
class Map
{

};
#endif