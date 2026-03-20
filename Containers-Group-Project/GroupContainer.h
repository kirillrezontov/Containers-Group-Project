#pragma once
#include "Container.h"
#include "List.h"
// Базовый класс для некоторой группы абстрактных контейнеров
class GroupContainer: public Container
{
protected:
	AbstractList** _tab;
	int _size;
	size_t _max_bytes;
	int _tab_size;
	class Iterator : public Container::Iterator {
	public:
		int _index;
		List** _tab;
		int _tab_size;
		Container::Iterator* _iter;
		void* getElement(size_t& size) override {
			return _iter->getElement(size);
		}
		void goToNext() override {
			if (_iter->hasNext()) _iter->goToNext();
			else {
				delete _iter;
				_index++;
				while(_index < _tab_size && (!_tab[_index] || _tab[_index]->empty())) _index++;
				if(_index < _tab_size) _iter = _tab[_index]->newIterator();
			}
		}
		bool hasNext() override {
			if (_iter->hasNext()) return true;
			int i = _index + 1;
			while (i < _tab_size) if (_tab[i] && !_tab[i]->empty()) return true;
			else continue;
		}
		bool equals(Container::Iterator* right) override {
			return _iter->equals(right);
		}
	};
	virtual int _hash(void* elem, size_t size) = 0;
public:
	GroupContainer(MemoryManager& mem) : Container(mem), _tab_size(256) { 
		_tab = (AbstractList**)_memory.allocMem(_tab_size*sizeof(AbstractList*));
		if (_tab) throw exception();
		for (int i = 0; i < _tab_size; i++) _tab[i] = nullptr;
	}
	
	int size() override { return _size; }
	size_t max_bytes() override { return _max_bytes; }
	GroupContainer::Iterator* newIterator() override{
		for (int i = 0; i < _tab_size; i++) {
			if (_tab[i] && !_tab[i]->empty()) {
				void* mem = _memory.allocMem(sizeof(GroupContainer::Iterator));
				GroupContainer::Iterator* _it = new(mem) GroupContainer::Iterator();
				_it->_iter = _tab[i]->newIterator();
			}
		}
		return nullptr;
	}
	GroupContainer::Iterator* find(void* elem, size_t size) override {
		Container::Iterator* lit;
		for (int i = 0; i < _tab_size; i++) {
			lit = _tab[i]->find(elem, size);
			if (lit) {
				void* mem = _memory.allocMem(sizeof(GroupContainer::Iterator));
				GroupContainer::Iterator* _it = new(mem) GroupContainer::Iterator();
				_it->_iter = lit;
			}
		}
		return nullptr;
	}
	bool empty() override { return _size == 0; }
	void remove(Container::Iterator* iter) override{
		GroupContainer::Iterator* Iter = (GroupContainer::Iterator*)iter;
		int old = _tab[Iter->_index]->size();
		_tab[Iter->_index]->remove(Iter->_iter);
		if (_tab[Iter->_index]->size() < old) _size--;
		if(!Iter->_iter->_this)
	}
};
