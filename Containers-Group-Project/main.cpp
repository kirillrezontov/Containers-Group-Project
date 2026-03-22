#include <iostream>
#include "List.h"
#include "List2.h"
#include "Set.h"
#include "PoolManager.h"
#include <ctime>
using namespace std;

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
	size_t poolSize[5] = { 8*2e6, 16*2e6, 32*2e6, 64*2e6, 128*2e6 };
	MultiPoolManager mem(1, poolSize);
	Set s(mem);

	size_t start = clock();
	for (int i = 0; i < 1000000; i++) {
		int r = s.insert(&i, sizeof(int));
		if (r) {
			std::cout << "Error: " << i << std::endl;
			return 1;
		}
	}

	size_t size;
	for (Container::Iterator* iter = s.newIterator();
		int* elem = (int*)iter->getElement(size); 
		iter->goToNext()) {
		if (!elem || size != sizeof(int) || *elem < 0 || *elem >= 1000000) {
			std::cout << "Error: " << (elem ? *elem : -1) << std::endl;
			return 1;
		}
	}

	for (int i = 0; i < 1000000; i+=2) {
		s.remove(s.find(&i, sizeof(int)));
	}
	size_t end = clock();

	cout << end - start << "ms\n";

	for (Container::Iterator* iter = s.newIterator();
		int* elem = (int*)iter->getElement(size);
		iter->goToNext()) {
		if (!elem || size != sizeof(int) || *elem < 0 || *elem >= 1000000 || *elem % 2 == 0) {
			std::cout << "Error: " << (elem ? *elem : -1) << std::endl;
			return 1;
		}
	}
	s.clear();
	return 0;
}