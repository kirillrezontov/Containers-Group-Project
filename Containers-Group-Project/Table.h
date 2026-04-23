#pragma once
#include "TableAbstract.h"

class Table : public AbstractTable {
private:
	using AbstractTable::find;
	using AbstractTable::remove;
	class Iterator :public GroupContainer::Iterator {
	public:
		Iterator():GroupContainer::Iterator(){}
		~Iterator(){}
		void* getElement(size_t& size) override;
	};
public:
	Table(MemoryManager& mem): AbstractTable(mem){}
	~Table(){}
	Container::Iterator* newIterator() override;
	int insertByKey(void* key, size_t keySize, void* elem, size_t elemSize) override;
	void removeByKey(void* key, size_t keySize) override;
	GroupContainer::Iterator* findByKey(void* key, size_t keySize) override;
	void* at(void* key, size_t keySize, size_t& valueSize) override;
};