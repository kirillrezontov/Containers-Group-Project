#pragma once
#include "ListAbstract.h"
#include "GroupContainer.h"
class List : public AbstractList
{
	friend class GroupContainer;
	struct Node {
		void* data;
		size_t size;
		Node* next;
	};
	size_t _size;
	Node* _head;
	class Iterator : public AbstractList::Iterator {

}