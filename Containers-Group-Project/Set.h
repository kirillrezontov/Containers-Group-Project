#pragma once
#include "SetAbstract.h"

class Set : public AbstractSet
{
private:
	int _hash(void* elem, size_t size) override;
public:
	Set(MemoryManager& mem) : AbstractSet(mem) {}
	~Set() {};
	int insert(void* elem, size_t size) override;
};