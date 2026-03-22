#include "Set.h"

int Set::_hash(void* elem, size_t size) {
	/*int hash = 0x1234;
	char* data = (char*)elem;
	while(size--){
		hash ^= *data++;
		hash *= 0x9E37;
		hash ^= hash >> 8;
		hash &= 0xFFFF;
	}
	return hash;*/
	const uint8_t* p = (const uint8_t*)elem;
	uint32_t h = 2166136261u;  // FNV offset basis

	while (size--) {
		h ^= *p++;
		h ^= h << 13;
		h ^= h >> 7;
		h ^= h << 3;
	}
	return h & 0x7FFFF;
}

int Set::insert(void* elem, size_t size) {
	int i = _hash(elem, size);
	if (!_tab[i]) {
		_tab[i] = new List(_memory);
		if (!_tab[i]) return 1;
		if(_tab[i]->push_front(elem, size)) return 1;
		_size++; return 0;
	}
	Container::Iterator* lit = _tab[i]->find(elem, size);
	if (lit) {
		delete lit;  
		return 1;
	}
	if (_tab[i]->push_front(elem, size)) {
		return 1;
	}
	_size++;
	return 0;
}