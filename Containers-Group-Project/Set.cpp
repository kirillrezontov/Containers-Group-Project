#include "Set.h"

int Set::insert(void* elem, size_t size) {
	int i = _hash(elem, size);
	Container::Iterator* it = _tab[i].find(elem, size);
	if (it) {
		delete it; return -1;
	}
	if (_tab[i].size() > _list_size_limit) {
		rehash(); i = _hash(elem, size);
	}
	if (_tab[i].push_front(elem, size)) {
		return 1;
	}
	_size++;
	return 0;
}