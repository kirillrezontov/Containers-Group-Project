#include "List.h"

void* List::Iterator::getElement(size_t& size) {
	if (!_this)
		return NULL;
	size = _this->size;
	return _this->data;
}

bool List::Iterator::hasNext() {
	return _this && _this->next;
}

void List::Iterator::goToNext() {
	if (_this)
		_this = _this->next;
}

bool List::Iterator::equals(AbstractList::Iterator* right) {
	List::Iterator* _right = (List::Iterator*)(right);
	if (!_right)
		return false;
	return _this == _right->_this;
}

void List::Iterator::operator delete(void* ptr) {
	if (ptr) {
		((List::Iterator*)ptr)->_iter_memory->freeMem(ptr);
	}
}

List::Iterator* List::find(void* elem, size_t size) {
	Node* _tmp = _head;
	Node* _prev = nullptr;
	while (_tmp) {
		if (_tmp->size == size && memcmp(_tmp->data, elem, size) == 0) {
			void* mem = _memory.allocMem(sizeof(List::Iterator));
			List::Iterator* _iter = new(mem) List::Iterator();
			if (!_iter)
				return nullptr;
			_iter->_this = _tmp;
			_iter->_prev = _prev;
			_iter->_iter_memory = &_memory;
			return _iter;
		}
		_prev = _tmp;
		_tmp = _tmp->next;
	}
	return nullptr;
}

List::Iterator* List::newIterator() {
	if (!_head)
		return nullptr;
	void* mem = _memory.allocMem(sizeof(List::Iterator));
	List::Iterator* _iter = new(mem) List::Iterator();
	if (!_iter)
		return nullptr;	
	_iter->_this = _head;
	_iter->_iter_memory = &_memory;
	return _iter;
}

void List::remove(AbstractList::Iterator* iter) {
	List::Iterator* _iter = (List::Iterator*)(iter);
	if (!_iter || !_iter->_this)
		return;
	Node* _tmp = _iter->_this;
	Node* _prev = _iter->_prev;
	_iter->_this = _tmp->next;	
	if (_tmp == _head)
		_head = _tmp->next;
	else {
		_prev->next = _tmp->next;
	}
	_memory.freeMem(_tmp->data);
	_memory.freeMem(_tmp);
	_size--;
}

int List::size() { return _size; }

size_t List::max_bytes() { return _max_bytes; }

bool List::empty() { return _size == 0; }

void List::clear() {
	while (_head) {
		Node* _tmp = _head;
		_head = _head->next;
		_memory.freeMem(_tmp->data);
		_memory.freeMem(_tmp);
	}
	_size = 0;
}

int List::push_front(void* elem, size_t elemSize) {
	if (!elem || elemSize == 0)
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
	_new->next = _head;
	_head = _new;
	_size++;
	return 0;
}

void List::pop_front() {
	if (!_head)
		return;
	Node* _tmp = _head;
	_head = _head->next;
	_memory.freeMem(_tmp->data);
	_memory.freeMem(_tmp);
	_size--;
}

void* List::front(size_t& size) {
	if (!_head)
		return nullptr;
	size = _head->size;
	return _head->data;
}

int List::insert(AbstractList::Iterator* iter, void* elem, size_t elemSize) {
	List::Iterator* _iter = (List::Iterator*)(iter);
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
	_new->next = _iter->_this;
	if (_iter->_prev)
		_iter->_prev->next = _new;
	else
		_head = _new;
	_iter->_prev = _new;
	_size++;
	return 0;
}