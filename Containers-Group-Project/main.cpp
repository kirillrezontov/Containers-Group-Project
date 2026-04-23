#include <iostream>
#include "List.h"
#include "Set.h"
#include "PoolManager.h"
#include <ctime>

using namespace std;

int main(){

	size_t poolSize[5] = { 8*2e6, 16*2e6, 32*2e6, 64*2e6, 128*2e6 };
	MultiPoolManager mem(1, poolSize);
	Set s(mem);
	//s.rehash(1 << 18);

	size_t start = clock();
	for (int i = 0; i < 1000000; i++) {
		int r = s.insert(&i, sizeof(int));
		if (r) {
			std::cout << "Error: " << i << std::endl;
			return 1;
		}
	}


	for (int i = 0; i < 1e6; i++) {
		Container::Iterator* iter = s.find(&i, sizeof(int));
		if (!iter) {
			std::cout << "Error: " << i << std::endl;
			return 1;
		}
		size_t size;
		int* elem = (int*)iter->getElement(size);
		if (!elem || size != sizeof(int) || *elem != i) {
			std::cout << "Error: " << (elem ? *elem : -1) << std::endl;
			return 1;
		}
		delete iter;
	}

	for (int i = 1e6; i < 2e6; i++) {
		if (s.find(&i, sizeof(int))) {
			std::cout << "Error: " << i << std::endl;
			return 1;
		}
	}

	for (int i = 0; i < 1000000; i+=2) {
		s.remove(s.find(&i, sizeof(int)));
	}
	size_t end = clock();
	//цикл который проверяет. что все удаленные удалены
	size_t size;
	Container::Iterator* iter = s.newIterator();
	cout << end - start << "ms\n";
	for (; int* elem = (int*)iter->getElement(size);
		iter->goToNext()) {
		if (!elem || size != sizeof(int) || *elem < 0 || *elem >= 1000000 || *elem % 2 == 0) {
			std::cout << "Error: " << (elem ? *elem : -1) << std::endl;
			return 1;
		}
	}
	delete iter;
	s.clear();

	return 0;
}