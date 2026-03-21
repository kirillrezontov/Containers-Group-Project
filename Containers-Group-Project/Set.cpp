#include "Set.h"

int Set::_hash(void* elem, size_t size) {
	int hash = 0x12;
	char* data = (char*)elem;
	while(size--){
		hash ^= *data++;
		hash *= 0x37;
		hash ^= hash >> 8;
		hash &= 0xFF;
	}
	return hash;
}

int Set::insert(void* elem, size_t size) {
	int i = _hash(elem, size);
	if (!_tab[i]) {
		_tab[i] = new List(_memory);
		if (!_tab[i]) return 1;
		return _tab[i]->push_front(elem, size);
	}
	Container::Iterator* lit = _tab[i]->find(elem, size);
	if (lit) return 1;
	if (_tab[i]->insert(nullptr, elem, size)) {
		return 1;
	}
	_size++;
	return 0;
}