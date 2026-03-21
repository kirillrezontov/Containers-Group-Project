#include <iostream>
#include "List.h"
#include "List2.h"
#include "Set.h"
#include "Mem.h"
template <class T> void test(T& c) {
	int n = 1000000;
	for (int i = 0; i < n; i++) {
		c.push_front(&i, sizeof(int));
	}
    Container::Iterator* iter = c.newIterator();
	for (int i = 0; i < n; i++, iter->goToNext()) {
		size_t size;
		int* elem = (int*)iter->getElement(size);
		if (!elem || size != sizeof(int) || *elem != n - i - 1) {
			std::cout << "Error: " << i << std::endl;
			return;
		}
	}
	delete iter;
	std::cout << "Remove\n";
	size_t size;
	iter = c.newIterator();
	for (int i = 0; i < n; i += 2, iter->goToNext()) {
		std::cout << '\r' << i;
		if (i != *(int*)iter->getElement(size)) {
			std::cout << "Error: " << i << std::endl;
			return;
		}
		c.remove(iter);
	}
	for (int i = 0; i < n; i+=2) {
		size_t size;
		int* elem = (int*)c.front(size);
		if (elem){
			std::cout << "Error: " << i << std::endl;
		}
		c.pop_front();
	}
}

int main(){
	Mem mem(1024);
	int a = 5;
	int b = 10;
	int c = 15;
	int d = 5;
	Set s(mem);
	s.insert(&a, sizeof(int));
	s.insert(&b, sizeof(int));
	s.insert(&c, sizeof(int));
	s.insert(&d, sizeof(int));
	size_t size;
	for (Container::Iterator* iter = s.newIterator();
		int* elem = (int*)iter->getElement(size); 
		iter->goToNext()) 
	{
		std::cout << *elem << std::endl;
	}
	s.clear();
	return 0;
}