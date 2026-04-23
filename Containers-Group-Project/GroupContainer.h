#pragma once
#include "Container.h"
#include "List.h"
// Базовый класс для некоторой группы абстрактных контейнеров
//
class GroupContainer : public Container
{
protected:
	List* _tab;
	int _size;
	int _tab_size;
	int _list_size_limit = 32;
	class Iterator : public Container::Iterator {
	public:
		List* _cur;
		List* _end;
		Container::Iterator* _iter;
		Iterator() : _cur(nullptr), _end(nullptr), _iter(nullptr) {}
		~Iterator() {
			if (_iter) delete _iter;
		}
		virtual void* getElement(size_t& size) override {
			if (!_iter) return nullptr;
			return _iter->getElement(size);
		}
		void goToNext() override {
			if (!_iter) return;
			if (_iter->hasNext()) _iter->goToNext();
			else {
				delete _iter;
				_cur++;
				while (_cur != _end && _cur->empty()) _cur++;
				if(_cur!= _end) _iter = _cur->newIterator();
				else _iter = nullptr;
			}
		}
		bool hasNext() override {
			if (!_iter) return false;
			if (_iter->hasNext()) return true;
			List* tmp = _cur + 1;
			while (tmp != _end && tmp->empty()) tmp++;
			if (tmp != _end) return true;
			return false;
		}
		bool equals(Container::Iterator* right) override {
			GroupContainer::Iterator* _right = (GroupContainer::Iterator*)right;
			return _iter->equals(_right->_iter);
		}
	};
	virtual int _hash(void* elem, size_t size) {
		const uint8_t* p = (const uint8_t*)elem;
		uint32_t h = 2166136261u; 
		while (size--) {
			h ^= *p++;
			h ^= h << 13;
			h ^= h >> 7;
			h ^= h << 3;
		}
		return h & (_tab_size-1);
	}
		
public:
	GroupContainer(MemoryManager& mem) : Container(mem), _tab_size(1<<4), _size(0) {
		_tab = (List*)_memory.allocMem(_tab_size * sizeof(List));
		if (!_tab) throw exception();
		for (int i = 0; i < _tab_size; i++) new(_tab + i) List(_memory);
	}
	~GroupContainer() {
		for (int i = 0; i < _tab_size; i++) _tab[i].~List();
		_memory.freeMem(_tab);
	}

	int size() override { return _size; }

	size_t max_bytes() override { return _memory.maxBytes(); }

	virtual Container::Iterator* newIterator() override {
		for (int i = 0; i < _tab_size; i++) {
			if (!_tab[i].empty()) {
				GroupContainer::Iterator* _it = new GroupContainer::Iterator();
				_it->_iter = _tab[i].newIterator();
				_it->_cur = _tab + i;
				_it->_end = _tab + _tab_size;
				return _it;
			}
		}
		return nullptr;
	}
	Container::Iterator* find(void* elem, size_t size) override {
		int i = _hash(elem, size);
		if (_tab[i].empty()) return nullptr;
		Container::Iterator* lit = _tab[i].find(elem, size);
		if (lit) {
			GroupContainer::Iterator* _it = new GroupContainer::Iterator();
			_it->_iter = lit;
			_it->_cur = _tab + i;
			_it->_end = _tab + _tab_size;
			return _it;
		}
		return nullptr;
	}
	bool empty() override { return _size == 0; }

	void remove(Container::Iterator* iter) override {
		if (!iter) return;
		GroupContainer::Iterator* _iter = (GroupContainer::Iterator*)iter;
		List* list = _iter->_cur;
		Container::Iterator* lit = _iter->_iter;
		if (lit->hasNext()) {
			list->remove(lit);
			_size--;
		}
		else {
			list->remove(lit);
			_size--;
			_iter->goToNext();
		}
	}
	void clear() override {
		for (int i = 0; i < _tab_size; i++) _tab[i].clear();
		_size = 0;
	}
	void rehash(int tabSize) {
		if (tabSize & (tabSize - 1)) return;
		List* newmem = (List*)_memory.allocMem(sizeof(List) * (tabSize));
		if (!newmem) return;
		memcpy(newmem, _tab, sizeof(List) * (_tab_size));
		for (int i = _tab_size; i < tabSize; i++) new(newmem + i) List(_memory);
		_memory.freeMem(_tab);
		_tab = newmem;
		_tab_size = tabSize;
		GroupContainer::Iterator* it = (GroupContainer::Iterator*)newIterator();
		void* el;
		size_t sz; int index;
		for (; it && (el = it->getElement(sz)); ) {
			index = _hash(el, sz);
			if (_tab + index != ((GroupContainer::Iterator*)it)->_cur) {
				newmem[index].push_front(el, sz);
				remove(it);
			}
			else it->goToNext();
		}
		delete it;
	}
	void rehash() {
		rehash(_tab_size << 1);
	}
};