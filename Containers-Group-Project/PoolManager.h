#pragma once
#include "MemoryManager.h"
#include <new>


class badAlloc {
	size_t _size;
public:
	badAlloc(size_t size) : _size(size) {}
	size_t size() const { return _size; }
};

class PoolManager : public MemoryManager
{
	size_t   _poolSize;
	void* _poolStart;
	void* _poolEnd;
	void* _freeListHead;
	friend class MultiPoolManager;
public:
	PoolManager(size_t size, size_t poolSize): MemoryManager(size) {
		size = (size / poolSize) * poolSize; 
		_poolSize = poolSize;
		_poolStart = malloc(size);
		if (!_poolStart) {
			throw badAlloc(size);
		}
		_poolEnd = (char*)_poolStart + size;
		_freeListHead = _poolStart;
		void** current = (void**)_freeListHead;
		size_t numBlocks = size / poolSize;
		for (size_t i = 0; i < numBlocks-1; i += 1) {
			*current = (char*)current + poolSize;
			current = (void**)*current;
		}
		*current = nullptr;
	}
	~PoolManager() { free(_poolStart); };
	inline void* allocMem(size_t size) override {
		if (size > _poolSize || _freeListHead == nullptr) {
			return nullptr;
		}
		void* allocatedBlock = _freeListHead;
		_freeListHead = *(void**)_freeListHead;
		return allocatedBlock;
	}
	inline void freeMem(void* ptr) override {
		if (ptr < _poolStart || ptr >= _poolEnd) {
			return;
		}
		*(void**)ptr = _freeListHead;
		_freeListHead = ptr;
	}
};

class MultiPoolManager : public MemoryManager
{
	PoolManager* _pools;
	int _numPools = 5;
	int _poolSizes[5] = { 8, 16, 32, 64, 128 };
	size_t _maxBytes;
public:
	MultiPoolManager(size_t size, size_t sizes[5]) : MemoryManager(size) {
		_pools = (PoolManager*)malloc(_numPools * sizeof(PoolManager));
		if (!_pools) throw badAlloc(size);

		int constructed = 0;
		try {
			for (; constructed < _numPools; constructed++) {
				new (_pools + constructed) PoolManager(sizes[constructed], _poolSizes[constructed]);
			}
		}
		catch (...) {
			for (int i = 0; i < constructed; i++)
				_pools[i].~PoolManager();
			free(_pools);
			throw badAlloc(size);
		}
		_maxBytes = -1;
	}
	void* allocMem(size_t size) override {
		int c = 0, s = (size-1) >> 3;
		while(s && c<5) {
			s >>= 1;
			c++;
		}
		if(c<5) {
			return _pools[c].allocMem(size);
		}
		return malloc(size);
	}
	void freeMem(void* ptr) override {
		for (int i = 0; i < _numPools; i++) {
			if (ptr >= _pools[i]._poolStart && ptr < _pools[i]._poolEnd) {
				_pools[i].freeMem(ptr);
				return;
			}
		}
		free(ptr);
	}
	~MultiPoolManager() {
		for (int i = 0; i < _numPools; i++) {
			_pools[i].~PoolManager();
		}
		free(_pools);
	}
};