#include "Queue.h"

/*------------------------Iterator-------------------------*/
void* Queue::getElement(size_t size) { return NULL; }
bool Queue::hasNext() { return false; }
void Queue::goToNext() { return; }
bool Queue::equals(Iterator* right) { return false; }

/*-------------------------Container---------------------------*/
int Queue::size() { return 0; }
size_t Queue::max_bytes() { return 0; }
Queue::Iterator* Queue::find(void* elem, size_t size) { return 0; }
Queue::Iterator* Queue::newIterator() { return 0; }
void Queue::remove(Iterator* iter) { return; }
void Queue::clear() { return; }
bool Queue::empty() { return NULL; }

/*---------------------------Queue-------------------------*/
int Queue::push(void* elem, size_t size) { return 0; }
int Queue::pop() { return 0; }
void* Queue::front(size_t& size) { return NULL; }
void* Queue::back(size_t& size) { return NULL; }
int Queue::insert(Iterator* iter, void* elem, size_t size) { return 0; }