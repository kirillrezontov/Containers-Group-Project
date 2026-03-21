#pragma once
#include "ListAbstract.h"
#include <cstring>


class List :
	public AbstractList
{
private:
	struct List_node
	{
		size_t size;
		void* data;
		List_node* next;
	};
	List_node* first_element;
	int count;
	friend class GroupContainer;
	class ListIterator :
		public Iterator
	{
		public:
		List_node* current;
		List_node* prev;
		ListIterator(List_node* c, List_node* p);
		~ListIterator() {}
		void* getElement(size_t& size);
		bool hasNext();
		void goToNext();
		bool equals(Iterator* right);
	};
public:
	List(MemoryManager& mem);
	~List();
	int size();
	size_t max_bytes();
	Iterator* find(void* elem, size_t size);
	Iterator* newIterator();
	void remove(Iterator* iter);
	void clear();
	bool empty();
	int push_front(void* elem, size_t elemSize);
	void pop_front();
	void* front(size_t& size);
	int insert(Iterator* iter, void* elem, size_t elemSize);
};








