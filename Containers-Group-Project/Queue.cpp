#include "Queue.h"

// new () node[];
/*------------------------Iterator-------------------------*/
void* Queue::Iterator::getElement(size_t& size) { return NULL; }
bool Queue::Iterator::hasNext() { return false; }
void Queue::Iterator::goToNext() {}
bool Queue::Iterator::equals(Iterator* right) { return false; }

/*-------------------------Container---------------------------*/
int Queue::size() { return 0; }
size_t Queue::max_bytes() { return 0; }
Queue::Iterator* Queue::find(void* elem, size_t size) { return 0; }
Queue::Iterator* Queue::newIterator() { return 0; }
void Queue::remove(Queue::Iterator* iter) { return; }
void Queue::clear() { return; }
bool Queue::empty() { return NULL; }

/*---------------------------Queue-------------------------*/

Queue::Queue(MemoryManager& mem) : AbstractQueue(mem), length(_memory.size()) {
    buf = (void**)_memory.allocMem(sizeof(void*) * length);
    _size = (int*)_memory.allocMem(sizeof(int) * length);
}

int Queue::push(void* elem, size_t size) { return 0; }
int Queue::pop() { return 0; }
void* Queue::front(size_t& size) { return NULL; }
void* Queue::back(size_t& size) { return NULL; }
int Queue::insert(Queue::Iterator* iter, void* elem, size_t size) { return 0; }
