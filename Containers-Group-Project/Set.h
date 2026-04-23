#pragma once
#include "SetAbstract.h"

class Set : public AbstractSet
{
private:
public:
	Set(MemoryManager& mem) : AbstractSet(mem) {}
	~Set() {};
	int insert(void* elem, size_t size) override;
};