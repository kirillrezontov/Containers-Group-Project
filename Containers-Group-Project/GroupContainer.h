#pragma once
#include "Container.h"
#include "List.h"
// Базовый класс для некоторой группы абстрактных контейнеров

class GroupContainer : public Container
{
protected:
	List** _tab;
	int _size;
	int _tab_size;
	class Iterator : public Container::Iterator {
	public:
		List** _tab;
		int _index;
		int _tab_size;
		List::Iterator* _iter;
		void* getElement(size_t& size) override {
			if (!_iter) return nullptr;
			return _iter->getElement(size);
		}
		void goToNext() override {
			if (_iter->hasNext()) _iter->goToNext();
			else {
				delete _iter;
				_index++;
				while (_index < _tab_size && (!_tab[_index] || _tab[_index]->empty())) _index++;
				if (_index < _tab_size) _iter = (List::Iterator*)_tab[_index]->newIterator();
				else _iter = nullptr;
			}
		}
		bool hasNext() override {
			if (_iter->hasNext()) return true;
			for (int i = _index + 1; i < _tab_size; i++)
				if (_tab[i] && !_tab[i]->empty()) return true;
			return false;
		}
		bool equals(Container::Iterator* right) override {
			GroupContainer::Iterator* _right = (GroupContainer::Iterator*)right;
			return _iter->equals(_right->_iter);
		}
	};
	virtual int _hash(void* elem, size_t size) = 0;
public:
	GroupContainer(MemoryManager& mem) : Container(mem), _tab_size(1<<19) {
		//_tab = (List**)_memory.allocMem(_tab_size * sizeof(List*));
		_tab = (List**)malloc(_tab_size * sizeof(List*));
		if (!_tab) throw exception();
		for (int i = 0; i < _tab_size; i++) _tab[i] = nullptr;
	}
	~GroupContainer() {
		for (int i = 0; i < _tab_size; i++) if (_tab[i]) delete _tab[i];
		//_memory.freeMem(_tab);
		free(_tab);
	}

	int size() override { return _size; }
	size_t max_bytes() override { return _memory.maxBytes(); }
	GroupContainer::Iterator* newIterator() override {
		for (int i = 0; i < _tab_size; i++) {
			if (_tab[i] && !_tab[i]->empty()) {
				GroupContainer::Iterator* _it = new GroupContainer::Iterator();
				_it->_iter = (List::ListIterator*)_tab[i]->newIterator();
				_it->_tab = _tab;
				_it->_index = i;
				_it->_tab_size = _tab_size;
				return _it;
			}
		}
		return nullptr;
	}
	GroupContainer::Iterator* find(void* elem, size_t size) override {
		int i = _hash(elem, size);
		if (!_tab[i]) return nullptr;
		List::ListIterator* lit = (List::ListIterator*)_tab[i]->find(elem, size);
		if (lit) {
			GroupContainer::Iterator* _it = new GroupContainer::Iterator();
			_it->_iter = lit;
			_it->_tab = _tab;
			_it->_index = i;
			_it->_tab_size = _tab_size;
			return _it;
		}
		return nullptr;
	}
	bool empty() override { return _size == 0; }
	void remove(Container::Iterator* iter) override {
		if (!iter) return;
		GroupContainer::Iterator* Iter = (GroupContainer::Iterator*)iter;
		int i = Iter->_index;
		int old = _tab[i]->size();
		List::ListIterator* lit = new List::ListIterator(*Iter->_iter);
		Iter->goToNext();
		_tab[i]->remove(lit);
		delete lit;
		if (_tab[i]->size() < old) _size--;
	}
	void clear() override {
		for (int i = 0; i < _tab_size; i++) if (_tab[i]) _tab[i]->clear();
		_size = 0;
	}
};