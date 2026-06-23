#pragma once
#include "Set.h"
#include "PoolManager.h"
#include <fstream>

class Tester {
protected:
	ofstream _file;
	size_t _test_count;
	int (**_tests)(int& time, int test_size);
public:
	class TesterSetupError {};
	Tester(size_t test_count, const char* file) : _file(ofstream(file)),_test_count(test_count), _tests(NULL) {}
	virtual void* generateTest(size_t& input_size, size_t objnum, int*& objsizes);
	virtual void runTests(int test_size) = 0;
};

class SetTester : public Tester {
	const int _tnum = 7;
	int* _res;
	int* _time;
public:
	SetTester(size_t test_count, const char* file):Tester(test_count, file){
		_res = (int*)malloc(sizeof(int) * _tnum);
		if (!_tests) throw TesterSetupError();
		_time = (int*)malloc(sizeof(int) * _tnum);
		if (!_tests) throw TesterSetupError();
		_tests = (int(**)(int&, int))malloc(sizeof(int(**)(int&, int)) * _tnum);
		if (!_tests) throw TesterSetupError();
	}
	void* generateInts(size_t objnum);
	void test(void* input, size_t input_size, size_t objnum, size_t* objsizes);
	void runTests(int test_size) override;
	int test_create_destroy(int& time, int test_size);
	int test_insert_find_remove(int& time, int test_size);
	int test_size(int& time, int test_size);
	int test_check_unique(int& time, int test_size);
	int test_diff_types(int& time, int test_size);
	int test_big_size(int& time, int test_size);
	int test_big_size_half_find_del(int& time, int test_size);
};
