#pragma once
#include "ListAbstract.h"

class List : public AbstractList
{
	int _size;
	int _max_bytes;
	typedef struct Node {
		void* data;
		size_t size;
		Node* next;
	};
	Node* _head;
public:
	class Iterator : public AbstractList::Iterator {
	protected:
		Node* _this;
		Node* _prev;
		MemoryManager* _iter_memory;
		friend class List;
	public:
		Iterator() : _this(NULL), _prev(NULL), _iter_memory(NULL) {}
		~Iterator() {}
		void* getElement(size_t& size) override;
		bool hasNext() override;
		void goToNext() override;
		bool equals(AbstractList::Iterator* right) override;
		void operator delete(void* ptr);
	};
	List(MemoryManager& mem) : AbstractList(mem), _head(nullptr), _size(0), _max_bytes(0) {}
	int size() override;
	size_t max_bytes() override;
	List::Iterator* find(void* elem, size_t size) override;
	List::Iterator* newIterator() override;
	void remove(AbstractList::Iterator* iter) override;
	void clear() override;
	bool empty() override;
	int push_front(void* elem, size_t elemSize) override;
	void pop_front() override;
	void* front(size_t& size) override;
	int insert(AbstractList::Iterator* iter, void* elem, size_t elemSize) override;
};