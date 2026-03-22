#include "List.h"

void* List::Iterator::getElement(size_t& size) {
	size = _this->size;
	return _this->data;
}

bool List::Iterator::hasNext() {
	return _this && _this->next;
}

void List::Iterator::goToNext() {
	_prev = _this;
	if (_this && _this->next) _this = _this->next;
}

bool List::Iterator::equals(Container::Iterator* right) {
	List::Iterator* _right = (List::Iterator*)right;
	if (!(_right && _this && _right->_this)) return false;
	if (_right->_this == _this) return true;
	return false;
}

int List::size() { return _size; }

size_t List::max_bytes() { return _memory.maxBytes(); }

Container::Iterator* List::find(void* elem, size_t size) {
	Node* curr = _head;
	Node* prev = nullptr;
	while (curr) {
		if (curr->size == size && !memcmp(curr->data, elem, size))
		{
			List::Iterator* _new = new List::Iterator;
			if (!_new) return nullptr;
			_new->_prev = prev;
			_new->_this = curr;
			return _new;
		}
		prev = curr;
		curr = curr->next;
	}
	return nullptr;
}
Container::Iterator* List::newIterator() {
	if (!_head) return nullptr;
	List::Iterator* _new = new List::Iterator;
	if (!_new) return nullptr;
	_new->_prev = nullptr;
	_new->_this = _head;
	return _new;
}

void List::remove(Container::Iterator* iter) {
	List::Iterator* _iter = (List::Iterator*)iter;
	Node* next = _iter->_this->next;
	if (_iter->_prev) _iter->_prev->next = next;
	else _head = next;
	_memory.freeMem(_iter->_this->data);
	_memory.freeMem(_iter->_this);
	_iter->_this = next;
	_size--;
}
void List::clear() {
	Node* next = nullptr;
	while (_head) {
		_memory.freeMem(_head->data);
		next = _head->next;
		_memory.freeMem(_head);
		_head = next;
	}
	_size = 0;
}
bool List::empty() {
	return _size == 0;
}

List::~List() {
	clear();
}
int List::push_front(void* elem, size_t elemSize) {
	Node* _new = (Node*)_memory.allocMem(sizeof(Node));
	if (!_new) return 1;
	_new->data = _memory.allocMem(elemSize);
	if (!_new->data) { _memory.freeMem(_new); return 1; }
	memcpy(_new->data, elem, elemSize);
	_new->size = elemSize;
	_new->next = _head;
	_head = _new;
	_size++;
	return 0;
}
void List::pop_front() {
	if (!_head) return;
	Node* next = _head->next;
	_memory.freeMem(_head->data);
	_memory.freeMem(_head);
	_head = next;
	_size--;
}
void* List::front(size_t& size) {
	if (!_head) return nullptr;
	size = _head->size;
	return _head->data;
}
int List::insert(Container::Iterator* iter, void* elem, size_t elemSize) {
	List::Iterator* _iter = (List::Iterator*)iter;
	Node* curr = _iter->_this;
	Node* prev = _iter->_prev;
	Node* _new = (Node*)_memory.allocMem(sizeof(Node));
	if (!_new) return 1;
	_new->data = _memory.allocMem(elemSize);
	if (!_new->data) { _memory.freeMem(_new); return 1; }
	memcpy(_new->data, elem, elemSize);
	_new->size = elemSize;
	if (prev) prev->next = _new;
	else _head = _new;
	_new->next = curr;
	_iter->_prev = _new;
	_size++;
	return 0;
}