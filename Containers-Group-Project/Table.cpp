#include "Table.h"

void* Table::Iterator::getElement(size_t& size) {
	if (!_iter) return nullptr;
	size_t sizeF = 0;
	char* elem = (char*)_iter->getElement(sizeF);
	size = *(size_t*)elem;
	elem += (sizeof(size_t) + size);
	size = *(size_t*)elem;
	return (void*)(elem + sizeof(size_t));
}

Container::Iterator* Table::newIterator() {
	for (int i = 0; i < _tab_size; i++) {
		if (!_tab[i].empty()) {
			Table::Iterator* _it = new Table::Iterator();
			_it->_iter = _tab[i].newIterator();
			_it->_cur = _tab + i;
			_it->_end = _tab + _tab_size;
			return _it;
		}
	}
	return nullptr;
}

int Table::insertByKey(void* key, size_t keySize, void* elem, size_t elemSize) {
	int i = _hash(key, keySize);
	Container::Iterator* it = ((KeyList*)(_tab + i))->findByKey(key, keySize);
	if (it) {
		delete it; return -1;
	}
	if (_tab[i].size() > _list_size_limit) {
		rehash(); i = _hash(key, keySize);
	}
	size_t sizeNew = keySize + elemSize + 2 * sizeof(size_t);
	void *elemNew = _memory.allocMem(sizeNew);
	if (!elemNew) { delete it; return -1; }
	char*el = (char*)elemNew;
	memcpy(el, &keySize, sizeof(size_t));	el += sizeof(size_t);
	memcpy(el, key, keySize);				el += keySize;
	memcpy(el, &elemSize, sizeof(size_t));	el += sizeof(size_t);
	memcpy(el, elem, elemSize);
	if (_tab[i].push_front(elemNew, sizeNew)) {
		return 1;
	}
	_size++;
	return 0;
}

GroupContainer::Iterator* Table::findByKey(void* key, size_t keySize) {
	int i = _hash(key, keySize);
	if (_tab[i].empty()) return nullptr;
	Container::Iterator* lit = ((KeyList*)(_tab + i))->findByKey(key, keySize);
	if (lit) {
		Table::Iterator* _it = new Table::Iterator();
		_it->_iter = lit;
		_it->_cur = _tab + i;
		_it->_end = _tab + _tab_size;
		return _it;
	}
	return nullptr;
}

void Table::removeByKey(void* key, size_t keySize) {
	int i = _hash(key, keySize);
	if (_tab[i].empty()) return;
	Container::Iterator* lit = ((KeyList*)(_tab + i))->findByKey(key, keySize);
	if (lit) {
		_tab[i].remove(lit);
		_size--;
		delete lit;
	}
}

void* Table::at(void* key, size_t keySize, size_t& valueSize) {
	int i = _hash(key, keySize);
	if (_tab[i].empty()) return nullptr;
	Container::Iterator* lit = ((KeyList*)(_tab + i))->findByKey(key, keySize);
	if (lit) {
		valueSize = 0;
		char* elem = (char*)lit->getElement(valueSize);
		valueSize = *(size_t*)elem;
		elem += (sizeof(size_t) + valueSize);
		valueSize = *(size_t*)elem;
		return (void*)(elem + sizeof(size_t));
	}
	return nullptr;
}
