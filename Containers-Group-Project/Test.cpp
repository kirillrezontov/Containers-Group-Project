#include "Test.h"
#include <ctime>

void SetTester::runTests(int test_size) {
	int i = 0;
	_res[i] = test_create_destroy(_time[i], test_size);
	cout << "Test " << i << ": create and destroy: " << (_res[i] == 0 ? "OK" : (_res[i] == 1 ? "FAIL" : "MEMORY ERROR")) << ", time: " << _time[i] << " ms\n";
	i++;
	_res[i] = test_insert_find_remove(_time[i], test_size);
	cout << "Test " << i << ": insert, find and remove: " << (_res[i] == 0 ? "OK" : (_res[i] == 1 ? "FAIL" : "MEMORY ERROR")) << ", time: " << _time[i] << " ms\n";
	i++;
	_res[i] = this->test_size(_time[i], test_size);
	cout << "Test " << i << ": size: " << (_res[i] == 0 ? "OK" : (_res[i] == 1 ? "FAIL" : "MEMORY ERROR")) << ", time: " << _time[i] << " ms\n";
	i++;
	_res[i] = this->test_check_unique(_time[i], test_size);
	cout << "Test " << i << ": check unique: " << (_res[i] == 0 ? "OK" : (_res[i] == 1 ? "FAIL" : "MEMORY ERROR")) << ", time: " << _time[i] << " ms\n";
	i++;
	_res[i] = this->test_diff_types(_time[i], test_size);
	cout << "Test " << i << ": different types: " << (_res[i] == 0 ? "OK" : (_res[i] == 1 ? "FAIL" : "MEMORY ERROR")) << ", time: " << _time[i] << " ms\n";
	i++;
	_res[i] = this->test_big_size(_time[i], test_size);
	cout << "Test " << i << ": big size: " << (_res[i] == 0 ? "OK" : (_res[i] == 1 ? "FAIL" : "MEMORY ERROR")) << ", time: " << _time[i] << " ms\n";
	i++;
	_res[i] = this->test_big_size_half_find_del(_time[i], test_size);
	cout << "Test " << i << ": big size half find and delete: " << (_res[i] == 0 ? "OK" : (_res[i] == 1 ? "FAIL" : "MEMORY ERROR")) << ", time: " << _time[i] << " ms\n";
	int passed = 0, failed = 0, memerr = 0;
	for (int i = 0; i < _tnum; i++) {
		if (_res[i] == 0) passed++;
		else if (_res[i] == 1) failed++;
		else if (_res[i] == 2) memerr++;
	}
	cout << "Summary: " << passed << " passed, " << failed << " failed, " << memerr << " memory errors\n";
}

void* Tester::generateTest(size_t &input_size, size_t objnum, int*& objsizes) {
	objsizes = (int*)malloc(sizeof(int) * objnum);
	srand(time(NULL));
	if (!objsizes) throw Tester::TesterSetupError();
	for (int i = 0; i < objnum; i++) { objsizes[i] = rand() % 128 + 1; input_size += objsizes[i]; }
	void* input = malloc(input_size);
	if (!input) throw Tester::TesterSetupError();
	unsigned char* p = (unsigned char*)input;
	for (int i = 0; i < objnum; i++) {
		for (int j = 0; j < objsizes[i]; j++) p[j] = rand() % 256;
		p = p + objsizes[i];
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

int SetTester::test(int &time, void* input, size_t input_size, size_t objnum, size_t* objsizes) {
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
		cout << "Allocated memory size: " << max * 5 << '\n';
		Set set(mem);
		int result = 0, duples_n = 0, duples_s = 0;
		void* p = input;
		for (int i = 0; i < objnum; i++) {
			result = set.insert(p, objsizes[i]);
			if (result == 1) {
				cout << "Element of size " << objsizes[i] << " already exists\n";
				duples_s += objsizes[i];
				duples_n++;
			}
			if (result == 2) {
				cout << "Error inserting element of size " << objsizes[i] << '\n';
			}
			p = (void*)((char*)p + objsizes[i]);
		}
		if (duples_n) {
			size_t size, offset = 0; int i = 0; void* elem;
			Container::Iterator* it = set.newIterator();
			for (; (elem = it->getElement(size))!=NULL; it->goToNext()) {
				memcpy((char*)input + offset, elem, size); objsizes[i++] = size;
				offset += size;
			}
			delete it;
			if (offset != input_size-duples_s) {
				cout << "Error: total size of elements in set (" << offset << ") does not match input size excluding doubles (" << input_size - duples_s << ")\n";
				time = clock() - start;
				return 1;
			}
			input_size = offset;
			objnum -= duples_n;
		}
		p = input;
		for (int i = 0; i < objnum; i++) {
			Container::Iterator* it = set.find(p, objsizes[i]);
			if (!it) {
				cout << "Element of size " << objsizes[i] << " not found\n";
			}
			else {
				size_t size;
				void* elem = it->getElement(size);
				if (size != objsizes[i] || memcmp(elem, p, size) != 0) {
					cout << "Element of size " << objsizes[i] << " not found\n";
				}
				delete it;
			}
			p = (void*)((char*)p + objsizes[i]);
		}
		p = input;
		for (int i = 0; i < objnum; i += 2) {
			Container::Iterator* it = set.find(p, objsizes[i]);
			if (!it) {
				cout << "Element of size " << objsizes[i] << " not found\n";
			}
			else {
				set.remove(it);
				delete it;
			}
			p = (void*)((char*)p + objsizes[i]);
		}
		p = input;
		for (int i = 0; i < objnum; i++) {
			Container::Iterator* it = set.find(p, objsizes[i]);
			if (i % 2 == 0) {
				if (it) {
					cout << "Element of size " << objsizes[i] << " not deleted\n";
					delete it;
				}
			}
			else {
				if (!it) {
					cout << "Element of size " << objsizes[i] << " not found\n";
				}
				else {
					size_t size;
					void* elem = it->getElement(size);
					if (size != objsizes[i] || memcmp(elem, p, size) != 0) {
						cout << "Element of size " << objsizes[i] << " not found\n";
					}
					delete it;
				}
			}
			p = (void*)((char*)p + objsizes[i]);
		}
		time = clock() - start;
		cout << "Test completed in " << time << " ms\n";
		return 0;
	}
	catch (badAlloc& e) {
		cout << "Memory allocation error" << '\n';
		return 2;
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
	int* objsizes = NULL;
	void* input = generateTest(input_size, test_size, objsizes);
	int start = clock();
	try {
		MultiPoolManager mem(1280 * test_size);
		Set set(mem);
		void* p = input;
		for (int i = 0; i < test_size; i++) {
			set.insert(p, objsizes[i]);
			Container::Iterator* it = set.find(p, objsizes[i]);
			if (!it) {
				free(input);
				free(objsizes);
				time = clock() - start;
				return 1;
			}
			set.remove(it);
			p = (void*)((char*)p + objsizes[i]);
			delete it;
		}
	}
	catch (badAlloc& e) {
		free(input);
		free(objsizes);
		return 2;
	}
	time = clock() - start;
	free(input);
	free(objsizes);
	return 0;
}

int SetTester::test_size(int& time, int test_size) {
	void* input = generateInts(test_size);
	int start = clock();
	try {
		MultiPoolManager mem(640 * test_size);
		Set set(mem);
		void* p = input; int s = 0;
		for (int i = 0; i < test_size; i++) {
			p = (void*)((char*)p + sizeof(int));
			s = set.size();
		}
		if (set.size() != test_size) {
			free(input);
			time = clock() - start;
			return 1;
		}
	}
	catch (badAlloc& e) {
		free(input);
		return 2;
	}
	time = clock() - start;
	free(input);
	return 0;
}

int SetTester::test_check_unique(int& time, int test_size) {
	void* input = generateInts(test_size);
	int start = clock();
	try {
		MultiPoolManager mem(1280 * test_size);
		Set set(mem);
		void* p = input;
		for (int i = 0; i < test_size; i++) {
			set.insert(p, sizeof(int));
			p = (void*)((char*)p + sizeof(int));
		}
		p = input;
		for (int i = 0; i < test_size; i++) {
			int result = set.insert(p, sizeof(int));
			p = (void*)((char*)p + sizeof(int));
			if (result != 1) {
				free(input);
				time = clock() - start;
				return 1;
			}
		}
	}
	catch (badAlloc& e) {
		free(input);
		return 2;
	}
	time = clock() - start;
	free(input);
	return 0;
}

int SetTester::test_diff_types(int& time, int test_size) {
	size_t input_size = 0;
	int* objsizes = NULL;
	void* input = generateTest(input_size, test_size, objsizes);
	int start = clock();
	try {
		MultiPoolManager mem(1280 * test_size);
		Set set(mem);
		void* p = input;
		for (int i = 0; i < test_size; i++) {
			set.insert(p, objsizes[i]);
			p = (void*)((char*)p + objsizes[i]);
		}
		p = input;
		for (int i = 0; i < test_size; i++) {
			Container::Iterator* it = set.find(p, objsizes[i]);
			p = (void*)((char*)p + objsizes[i]);
			if (!it) {
				free(input);
				free(objsizes);
				time = clock() - start;
				return 1;
			}
			delete it;
		}
	}
	catch (badAlloc& e) {
		free(input);
		free(objsizes);
		return 2;
	}
	time = clock() - start;
	free(input);
	free(objsizes);
	return 0;
}

int SetTester::test_big_size(int& time, int test_size) {
	test_size *= 1000;
	size_t input_size = 0;
	int* objsizes = NULL;
	void* input = generateTest(input_size, test_size, objsizes);
	int start = clock();
	try {
		MultiPoolManager mem(1280 * test_size);
		Set set(mem);
		void* p = input;
		for (int i = 0; i < test_size; i++) {
			set.insert(p, objsizes[i]);
			p = (void*)((char*)p + objsizes[i]);
		}
		p = input;
		for (int i = 0; i < test_size; i++) {
			Container::Iterator* it = set.find(p, objsizes[i]);
			p = (void*)((char*)p + objsizes[i]);
			if (!it) {
				free(input);
				free(objsizes);
				time = clock() - start;
				return 1;
			}
			delete it;
		}
	}
	catch (badAlloc& e) {
		free(input);
		free(objsizes);
		return 2;
	}
	time = clock() - start;
	free(input);
	free(objsizes);
	return 0;
}

int SetTester::test_big_size_half_find_del(int& time, int test_size) {
	test_size *= 1000;
	size_t input_size = 0;
	int* objsizes = NULL;
	void* input = generateTest(input_size, test_size, objsizes);
	int start = clock();
	try {
		MultiPoolManager mem(1280 * test_size);
		Set set(mem);
		int result = 0, duples_n = 0, duples_s = 0;
		void* p = input;
		for (int i = 0; i < test_size; i++) {
			result = set.insert(p, objsizes[i]);
			if (result == 1) {
				cout << "Element of size " << objsizes[i] << " already exists\n";
				duples_s += objsizes[i];
				duples_n++;
			}
			if (result == 2) {
				time = clock() - start;
				return 2;
			}
			p = (void*)((char*)p + objsizes[i]);
		}
		
		if (duples_n) {
			size_t size, offset = 0; int i = 0; void* elem;
			Container::Iterator* it = set.newIterator();
			for (; (elem = it->getElement(size)) != NULL; it->goToNext()) {
				memcpy((char*)input + offset, elem, size); objsizes[i++] = size;
				offset += size;
			}
			delete it;
			if (offset != input_size - duples_s) {
				cout << "Error: total size of elements in set (" << offset << ") does not match input size excluding doubles (" << input_size - duples_s << ")\n";
				time = clock() - start;
				return 1;
			}
			input_size = offset;
			test_size -= duples_n;
		}
		p = input;
		for (int i = 0; i < test_size; i++) {
			if (i % 2 == 0) {
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
			else {
				Container::Iterator* it = set.find(p, objsizes[i]);
				if (!it) {
					free(input);
					free(objsizes);
					time = clock() - start;
					return 1;
				}
				delete it;
			}
			p = (void*)((char*)p + objsizes[i]);
		}
	}
	catch (badAlloc& e) {
		free(input);
		free(objsizes);
		return 2;
	}
	time = clock() - start;
	free(input);
	free(objsizes);
	return 0;
}

void SetTester::print_results() {
	int passed = 0, failed = 0, memerr = 0;
	for (int i = 0; i < _tnum; i++) {
		if (_res[i] == 0) passed++;
		else if (_res[i] == 1) failed++;
		else if (_res[i] == 2) memerr++;
	}
	cout << "Summary: " << passed << " passed, " << failed << " failed, " << memerr << " memory errors\n";
}