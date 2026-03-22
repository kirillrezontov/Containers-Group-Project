#pragma once
#include "ListAbstract.h"

class List : public AbstractList
{
protected:
	typedef struct Node
	{
		void* data;
		size_t size;
		Node* next;
	} Node;
	int _size;
	Node* _head;
	class Iterator : public Container::Iterator {
	public:
		Node* _this;
		Node* _prev;
		void* getElement(size_t& size) override;
		bool hasNext() override;
		void goToNext() override;
		bool equals(Container::Iterator* right) override;
	};
public:
	List(MemoryManager& mem) : AbstractList(mem), _head(nullptr), _size(0){}
	int size() override;
	size_t max_bytes() override;
	Container::Iterator* find(void* elem, size_t size) override;
	Container::Iterator* newIterator() override;
	void remove(Container::Iterator* iter) override;
	void clear() override;
	bool empty() override;

	~List();
	int push_front(void* elem, size_t elemSize) override;
	void pop_front() override;
	void* front(size_t& size) override;
	int insert(Container::Iterator* iter, void* elem, size_t elemSize) override;
};
