#pragma once
#include "List2Abstract.h"

class List2 : public AbstractList2
{
protected:
	int _size;
	int _max_bytes;
	typedef struct Node
	{
		void* data;
		size_t size;
		Node* next;
		Node* prev;
	} Node;
	Node* _head;
	Node* _tail;
public:
	class Iterator : public AbstractList2::Iterator {
	protected:
		Node* _this;
		MemoryManager* _iter_memory;
		friend class List2;
	public:
		Iterator() : _this(NULL), _iter_memory(NULL) {}
		void* getElement(size_t& size) override;
		bool hasNext() override;
		void goToNext() override;
		bool equals(AbstractList2::Iterator* right) override;
		void operator delete(void* ptr);
	};
	List2(MemoryManager& mem) : AbstractList2(mem), _head(nullptr), _tail(nullptr), _size(0), _max_bytes(0){}
	int size() override;
	size_t max_bytes() override;
	List2::Iterator* find(void* elem, size_t size) override;
	List2::Iterator* newIterator() override;
	void remove(AbstractList2::Iterator* iter) override;
	void clear() override;
	bool empty() override;

	int push_back(void* elem, size_t elemSize) override;
	int push_front(void* elem, size_t elemSize) override;
	void pop_back() override;
	void pop_front() override;
	void* front(size_t& size) override;
	void* back(size_t& size) override;
	int insert(AbstractList2::Iterator* iter, void* elem, size_t elemSize) override;
};