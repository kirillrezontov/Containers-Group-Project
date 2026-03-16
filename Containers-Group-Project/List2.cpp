#include "List2.h"

void* List2::Iterator::getElement(size_t& size)
{
	if (!_this)
		return NULL;
	size = _this->size;
	return _this->data;
}

bool List2::Iterator::hasNext()
{
	return _this && _this->next;
}

void List2::Iterator::goToNext()
{
	if (_this)
		_this = _this->next;
}

bool List2::Iterator::equals(AbstractList2::Iterator* right)
{
	List2::Iterator* _right = (List2::Iterator*)(right);
	if (!_right)
		return false;
	return _this == _right->_this;
}

void List2::Iterator::operator delete(void* ptr) {
	((List2::Iterator*)ptr)->_iter_memory;
}

List2::Iterator* List2::find(void* elem, size_t size)
{
	Node* _tmp = _head;
	while (_tmp) {
		if (_tmp->size == size && memcmp(_tmp->data, elem, size) == 0) {
			void* mem = _memory.allocMem(sizeof(List2::Iterator));
			List2::Iterator* _iter = new(mem) List2::Iterator();
			if (!_iter)
				return nullptr;
			_iter->_this = _tmp;
			_iter->_iter_memory = &_memory;
			return _iter;
		}
		_tmp = _tmp->next;
	}
	return nullptr;
}

List2::Iterator* List2::newIterator()
{
	if (!_head)
		return nullptr;
	void* mem = _memory.allocMem(sizeof(List2::Iterator));
	List2::Iterator* _iter = new(mem) List2::Iterator();
	if (!_iter)
		return nullptr;	
	_iter->_this = _head;
	_iter->_iter_memory = &_memory;
	return _iter;
}

void List2::remove(AbstractList2::Iterator* iter)
{
	List2::Iterator* _iter = (List2::Iterator*)(iter);
	if (!_iter || !_iter->_this)
		return;
	Node* _tmp = _iter->_this;
	_iter->_this = _tmp->next;	
	if (_tmp->prev)
		_tmp->prev->next = _tmp->next;
	else
		_head = _tmp->next;
	if (_tmp->next)
		_tmp->next->prev = _tmp->prev;
	else
		_tail = _tmp->prev;
	_memory.freeMem(_tmp->data);
	_memory.freeMem(_tmp);
	_size--;
}

int List2::size() { return _size; }

size_t List2::max_bytes() { return _max_bytes; }

bool List2::empty() { return _size == 0; }

void List2::clear()
{
	while (_head) {
		Node* _tmp = _head;
		_head = _head->next;
		_memory.freeMem(_tmp->data);
		_memory.freeMem(_tmp);
	}
	_tail = nullptr;
	_size = 0;
}

int List2::push_back(void* elem, size_t elemSize)
{
	if (!elem || elemSize == 0)
		return 1;
	if (!_head) {
		_head = (Node*)_memory.allocMem(sizeof(Node));
		if (!_head)
			return 1;
		_head->data = _memory.allocMem(elemSize);
		if (!_head->data) {
			_memory.freeMem(_head);
			return 1;
		}
		memcpy(_head->data, elem, elemSize);
		_head->size = elemSize;
		_head->next = nullptr;
		_head->prev = nullptr;
		_tail = _head;
		_size++;
		return 0;
	}
	else {
		Node* _new = (Node*)_memory.allocMem(sizeof(Node));
		if (!_new)
			return 1;
		_new->data = _memory.allocMem(elemSize);
		if (!_new->data) {
			_memory.freeMem(_new);
			return 1;
		}
		memcpy(_new->data, elem, elemSize);
		_tail->next = _new;
		_new->size = elemSize;
		_new->prev = _tail;
		_new->next = nullptr;
		_tail = _new;
		_size++;
		return 0;
	}
	return 1;
}

int List2::push_front(void* elem, size_t elemSize)
{
	if (!elem || elemSize == 0)
		return 1;
	if (!_head) {
		_head = (Node*)_memory.allocMem(sizeof(Node));
		if (!_head)
			return 1;
		_head->data = _memory.allocMem(elemSize);
		if (!_head->data) {
			_memory.freeMem(_head);
			return 1;
		}
		memcpy(_head->data, elem, elemSize);
		_head->size = elemSize;
		_head->next = nullptr;
		_head->prev = nullptr;
		_tail = _head;
		_size++;
		return 0;
	}
	else {
		Node* _new = (Node*)_memory.allocMem(sizeof(Node));
		if (!_new)
			return 1;
		_new->data = _memory.allocMem(elemSize);
		if (!_new->data) {
			_memory.freeMem(_new);
			return 1;
		}
		memcpy(_new->data, elem, elemSize);
		_head->prev = _new;
		_new->size = elemSize;
		_new->next = _head;
		_new->prev = nullptr;
		_head = _new;
		_size++;
		return 0;
	}
	return 1;
}

void List2::pop_back()
{
	if (!_tail)
		return;
	Node* _tmp = _tail;
	_tail = _tail->prev;
	if (_tail)
		_tail->next = nullptr;
	else
		_head = nullptr;
	_memory.freeMem(_tmp->data);
	_memory.freeMem(_tmp);
	_size--;
}

void List2::pop_front()
{
	if (!_head)
		return;
	Node* _tmp = _head;
	_head = _head->next;
	if (_head)
		_head->prev = nullptr;
	else
		_tail = nullptr;
	_memory.freeMem(_tmp->data);
	_memory.freeMem(_tmp);
	_size--;
}

void* List2::front(size_t& size)
{
	if (!_head)
		return nullptr;
	size = _head->size;
	return _head->data;
}

void* List2::back(size_t& size)
{
	if (!_tail)
		return nullptr;
	size = _tail->size;
	return _tail->data;
}

int List2::insert(AbstractList2::Iterator* iter, void* elem, size_t elemSize)
{
	List2::Iterator* _iter = (List2::Iterator*)(iter);
	if (!_iter || !_iter->_this)
		return 1;
	Node* _new = (Node*)_memory.allocMem(sizeof(Node));
	if (!_new)
		return 1;
	_new->data = _memory.allocMem(elemSize);
	if (!_new->data) {
		_memory.freeMem(_new);
		return 1;
	}
	memcpy(_new->data, elem, elemSize);
	_new->size = elemSize;
	_new->prev = _iter->_this->prev;
	_new->next = _iter->_this;
	if (_iter->_this->prev)
		_iter->_this->prev->next = _new;
	else
		_head = _new;
	_iter->_this->prev = _new;
	_size++;
	return 0;
}