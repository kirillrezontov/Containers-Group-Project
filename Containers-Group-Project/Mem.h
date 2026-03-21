#pragma once
#include "MemoryManager.h"

// Простейший менеджер памяти, использует ::new и ::delete
class Mem: public MemoryManager
{
public:
    Mem(size_t sz): MemoryManager(sz) {}

    void* allocMem(size_t sz)
    {
        return new char[sz];
    }

    void freeMem(void* ptr)
    {
        delete[] ptr;
    }
};

class Kchau : public MemoryManager
{
	void* _mem;
    void* _hole;
    int _max;
public:
    Kchau(size_t sz) : MemoryManager(sz) {
        _mem = malloc(sz);
    }
    void* allocMem(size_t sz) override {
        if (_hole) {
            void* ret = _hole;
            _hole = *(void**)_hole;
            return ret;
        }
		void* ret = (char*)_mem+_max;
        _max += 64;
        return ret;
	}
    void freeMem(void* ptr) override {
        *(void**)ptr = _hole;
        _hole = ptr;
	}
    ~Kchau() {
        free(_mem);
	}
};