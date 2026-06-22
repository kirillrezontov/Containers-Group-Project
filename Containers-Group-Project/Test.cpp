#include "Test.h"

void SetTester::runTests() {
	for (int i = 0; i < _tnum; i++) {
		_res[i] = _tests[i](_time[i]);
		cout << "Test No." << i << ":\nresult: " << _res[i] << ", time: " << _time[i] << '\n';
	}
}

void* SetTester::generateTest(size_t size, int*& objsizes) {
	objsizes = (int*)malloc(sizeof(int) * size);
	srand(time(NULL));
	if (!objsizes) throw Tester::TesterSetupError();
	int input_size = 0;
	for (int i = 0; i < size; i++) { objsizes[i] = rand() % 128 + 1; input_size += objsizes[i]; }
	void* input = malloc(input_size);
	if (!input) throw Tester::TesterSetupError();
	for (int i = 0; i < size; i++) {
		uint8_t* p = (uint8_t*)input;
		for (int j = 0; j < objsizes[i]; j++) p[j] = rand() % 256;
		input = (void*)((uint8_t*)input + objsizes[i]);
	}
	return input;
}

void test(void* input, int objnum, int* objsizes) {
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
	MultiPoolManager mem(max * 5);
	Set set(mem); 
	int offset = 0;
	for (int i = 0; i < _test_count; i++) {
		set.insert((void*)((uint8_t*)input + offset), objsizes[i]);
		offset += objsizes[i];
	}
}

void SetTester::test_create_destroy() {
	
}