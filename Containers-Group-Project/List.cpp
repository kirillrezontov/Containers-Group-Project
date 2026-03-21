#include "List.h"
#include <cstring>

List::List(MemoryManager& mem): AbstractList(mem)
{
	first_element = NULL;
	count = 0;
}


List::~List()
{
	clear();
}


int List::size()
{
	return count;
}


size_t List::max_bytes()
{
    return _memory.maxBytes();
}

bool List::empty()
{
    return count == 0;
}


void List::clear()
{
    List_node* cur = first_element;
    while (cur)
    {
        List_node* next = cur->next;
        _memory.freeMem(cur->data);
        _memory.freeMem(cur);
        cur = next;
    }
    first_element = NULL;
    count = 0;
}


int List::push_front(void* elem, size_t elemSize)
{
    List_node* node = (List_node*)_memory.allocMem(sizeof(List_node));
    if (node==NULL)
    {
        return 1;
    }
	node->data = _memory.allocMem(elemSize);
	if (node->data == NULL)
    {
		free(node);
        return 1;
    }
    memcpy(node->data, elem, elemSize);
    node->size = elemSize;
    node->next = first_element;
    first_element = node;
    count++;
    return 0;
}


void List::pop_front()
{
    if (first_element == NULL)
    {
        return;
    }
    List_node* temp = first_element;
    first_element = first_element->next;
    _memory.freeMem(temp->data);
    _memory.freeMem(temp);
    count--;
}


void* List::front(size_t& size)
{
    if (first_element == NULL)
    {
        return NULL;
    }
    size = first_element->size;
    return first_element->data;
}


Container::Iterator* List::newIterator()
{
    if (first_element == NULL)
    {
        return NULL;
    }
    return new ListIterator(first_element, NULL);
}


Container::Iterator* List::find(void* elem, size_t size)
{
    List_node* cur = first_element;
	List_node* prev = NULL;
    while (cur!=NULL)
    {
        if (cur->size == size && memcmp(cur->data, elem, size) == 0)
        {
            return new ListIterator(cur, prev);
        }
		prev = cur;
        cur = cur->next;
    }
    return NULL;
}


void List::remove(Iterator* iter)
{
    if (iter == NULL)
    {
        return;
   }
    ListIterator* it = (ListIterator*)iter;
    List_node* target = it->current;
	if (target == NULL)
        return;
	if (it->prev == NULL)
    {
        first_element = target->next;
    }
    else
    {
        it->prev->next = target->next;
    }
	it->current = target->next;
	_memory.freeMem(target->data);
	_memory.freeMem(target);
}


int List::insert(Iterator* iter, void* elem, size_t elemSize)
{
    if (iter == NULL)
    {
        return 1;
    }
    ListIterator* it = (ListIterator*)iter;
	List_node* cur = it->current;
    if (!cur)
        return 1;
    List_node* node = (List_node*)_memory.allocMem(sizeof(List_node));
    if (node==NULL)
    { 
        return 1;
    }
    node->data = _memory.allocMem(elemSize);
    if (node->data == NULL)
    {
        return 1;
    }
    memcpy(node->data, elem, elemSize);
    node->size = elemSize;
    node->next = cur->next;
    cur->next = node;
    count++;
    return 0;
}


List::ListIterator::ListIterator(List_node* c, List_node* p)
{
	prev = p;
    current = c;
}


void* List::ListIterator::getElement(size_t& size)
{
    if (current == NULL)
    {
        return NULL;
    }
    size = current->size;
    return current->data;
}


bool List::ListIterator::hasNext()
{
    if (current == NULL)
    {
        return 0;
    }
    return current->next != NULL;
}


void List::ListIterator::goToNext()
{
    if (current != NULL)
    {
        current = current->next;
    }
}


bool List::ListIterator::equals(Iterator* right)
{
    ListIterator* r = (ListIterator*)right;
    return current == r->current;
}
