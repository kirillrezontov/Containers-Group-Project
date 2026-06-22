#pragma once
#include "Set.h"
#include "PoolManager.h"
#include "fstream"

class Tester {
protected:
	ofstream _file;
	size_t _test_count;
	int (**_tests)(int& time);
public:
	class TesterSetupError {};
	Tester(size_t test_count, const char* file) : _file(ofstream(file)),_test_count(test_count) {}
	virtual void generateTests() = 0;
	virtual void runTests() = 0;
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
		_tests = (int(**)(int&))malloc(sizeof(int(**)(int&)) * _tnum);
		if (!_tests) throw TesterSetupError();
	}
	void* generateTest(size_t objnum, int*& objsizes);
	void test(void* input, size_t objnum, int* objsizes);
	void generateTests() override;
	void runTests() override;
	void test_create_destroy();
	void test_insert_delete();
	void test_size();
	void test_check_unique();
	void test_diff_types();
	void test_big_size();
	void test_big_size_half_find_del();
};
