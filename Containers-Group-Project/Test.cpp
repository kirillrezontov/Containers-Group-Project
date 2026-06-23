#include "Test.h"
#include <ctime>

void SetTester::runTests(int test_size) {
	for (int i = 0; i < _tnum; i++) {
		_res[i] = _tests[i](_time[i], test_size);
		cout << "Test No." << i << ":\nresult: " << _res[i] << ", time: " << _time[i] << '\n';
	}
}

void* Tester::generateTest(size_t &input_size, size_t objnum, int*& objsizes) {
	objsizes = (int*)malloc(sizeof(int) * objnum);
	srand(time(NULL));
	if (!objsizes) throw Tester::TesterSetupError();
	for (int i = 0; i < objnum; i++) { objsizes[i] = rand() % 128 + 1; input_size += objsizes[i]; }
	void* input = malloc(input_size);
	if (!input) throw Tester::TesterSetupError();
	for (int i = 0; i < objnum; i++) {
		char* p = (char*)input;
		for (int j = 0; j < objsizes[i]; j++) p[j] = rand() % 256;
		input = (void*)((char*)input + objsizes[i]);
	}
	return input;
}

void* SetTester::generateInts(size_t objnum) {
	int* input = (int*)malloc(objnum * sizeof(int));
	if (!input) throw Tester::TesterSetupError();
	for (int i = 0; i < objnum; i++) {
		input[i] = i;
	}
	return (void*)input;
}

void SetTester::test(void* input, size_t input_size, size_t objnum, size_t* objsizes) {
	int nsize[5] = { 0, 0, 0, 0, 0 };
	for (int i = 0; i < objnum; i++) {
		if (objsizes[i] <= 8) nsize[0]++;
		else if (objsizes[i] <= 16) nsize[1]++;
		else if (objsizes[i] <= 32) nsize[2]++;
		else if (objsizes[i] <= 64) nsize[3]++;
		else if (objsizes[i] <= 128) nsize[4]++;
	}
	size_t max = 0;
	for (int i = 0; i < 5; i++) {
		if (nsize[i]*(8<<i) > max) max = nsize[i] * (8 << i);
	}
	try {
		int start = clock();
		MultiPoolManager mem(max * 5); 
		_file << "Allocated memory size: " << max * 5 << '\n';
		Set set(mem);
		int result = 0, duples_n = 0, duples_s = 0;
		void* p = input;
		for (int i = 0; i < objnum; i++) {
			result = set.insert(p, objsizes[i]);
			if (result == 1) {
				_file << "Element of size " << objsizes[i] << " already exists\n";
				duples_s += objsizes[i];
				duples_n++;
			}
			if (result == 2) {
				_file << "Error inserting element of size " << objsizes[i] << '\n';
			}
			p = (void*)((char*)p + objsizes[i]);
		}
		if (duples_n) {
			size_t size, offset = 0; int i = 0;
			for (Container::Iterator* it = set.newIterator(); it != NULL; it->goToNext()) {
				void* elem = it->getElement(size);
				memcpy((char*)input + offset, elem, size); objsizes[i++] = size;
				offset += size;
			}
			if (offset != input_size-duples_s) {
				_file << "Error: total size of elements in set (" << offset << ") does not match input size excluding doubles (" << input_size - duples_s << ")\n";
				return;
			}
			input_size = offset;
			objnum -= duples_n;
		}

		for (int i = 0; i < objnum; i++) {
			p = (void*)((char*)input + i * objsizes[i]);
			Container::Iterator* it = set.find(p, objsizes[i]);
			if (!it) {
				_file << "Element of size " << objsizes[i] << " not found\n";
			}
			else {
				size_t size;
				void* elem = it->getElement(size);
				if (size != objsizes[i] || memcmp(elem, p, size) != 0) {
					_file << "Element of size " << objsizes[i] << " not found\n";
				}
				delete it;
			}
		}
		for (int i = 0; i < objnum; i += 2) {
			p = (void*)((char*)input + i * objsizes[i]);
			Container::Iterator* it = set.find(p, objsizes[i]);
			if (!it) {
				_file << "Element of size " << objsizes[i] << " not found\n";
			}
			else {
				set.remove(it);
				delete it;
			}
		}
		for (int i = 0; i < objnum; i++) {
			p = (void*)((char*)input + i * objsizes[i]);
			Container::Iterator* it = set.find(p, objsizes[i]);
			if (i % 2 == 0) {
				if (it) {
					_file << "Element of size " << objsizes[i] << " not deleted\n";
					delete it;
				}
			}
			else {
				if (!it) {
					_file << "Element of size " << objsizes[i] << " not found\n";
				}
				else {
					size_t size;
					void* elem = it->getElement(size);
					if (size != objsizes[i] || memcmp(elem, p, size) != 0) {
						_file << "Element of size " << objsizes[i] << " not found\n";
					}
					delete it;
				}
			}
		}
		int end = clock();
		_file << "Test completed in " << (end - start) << " ms\n";
	}
	catch (badAlloc& e) {
		_file << "Memory allocation error" << '\n';
		return;
	}
}

int SetTester::test_create_destroy(int& time, int test_size) {
	int start = clock();
	try {
		MultiPoolManager mem(1024);
		Set set(mem);
	}
	catch (badAlloc& e) {
		return 2;
	}
	int end = clock();
	time = end - start;
	return 0;
}

int SetTester::test_insert_find_remove(int& time, int test_size) {
	size_t input_size = 0;
	int* objsizes;
	void* input = generateTest(input_size, test_size, objsizes);
	int start = clock();
	try {
		MultiPoolManager mem(1280 * test_size);
		Set set(mem);
		for (int i = 0; i < test_size; i++) {
			void* p = (void*)((char*)input + i * objsizes[i]);
			set.insert(p, objsizes[i]);
		}
		for (int i = 0; i < test_size; i++) {
			void* p = (void*)((char*)input + i * objsizes[i]);
			Container::Iterator* it = set.find(p, objsizes[i]);
			if (!it) {
				free(input);
				free(objsizes);
				time = clock() - start;
				return 1;
			}
			delete it;
		}
		for (int i = 0; i < test_size; i += 2) {
			void* p = (void*)((char*)input + i * objsizes[i]);
			Container::Iterator* it = set.find(p, objsizes[i]);
			if (!it) {
				free(input);
				free(objsizes);
				time = clock() - start;
				return 1;
			}
			set.remove(it);
			delete it;
		}
	}
	catch (badAlloc& e) {
		return 2;
	}
	time = clock() - start;
	free(input);
	free(objsizes);
	return 0;
}

int SetTester::test_size(int& time) {
	size_t input_size = 0;
	int* objsizes;
	void* input = generateInts(1000000000);
	int start = clock();
	try {
		MultiPoolManager mem(1024 * 1024);
		Set set(mem);
		for (int i = 0; i < 1000; i++) {
			void* p = (void*)((char*)input + i * objsizes[i]);
			set.insert(p, objsizes[i]);
		}
		if (set.size() != 1000) {
			free(input);
			free(objsizes);
			time = clock() - start;
			return 1;
		}
	}
	catch (badAlloc& e) {
		return 2;
	}
	time = clock() - start;
	free(input);
	free(objsizes);
	return 0;
}