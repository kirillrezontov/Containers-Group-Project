#pragma once
#include "Set.h"
#include "PoolManager.h"
#include <fstream>

class Tester {
protected:
	ofstream& _file;
	size_t _test_count;
public:
	class TesterSetupError {};
	Tester(size_t test_count, ofstream& file) : _file(file),_test_count(test_count){}
	virtual ~Tester() {}
	virtual void* generateTest(size_t& input_size, size_t objnum, int*& objsizes);
	virtual void runTests(int test_size) = 0;
};

class SetTester : public Tester {
	const int _tnum = 7;
	int* _res;
	int* _time;
public:
	SetTester(ofstream& file):Tester(_tnum,file){
		_res = (int*)calloc(_tnum, sizeof(int));
		if (!_res) throw TesterSetupError();
		_time = (int*)calloc(_tnum, sizeof(int));
		if (!_time) throw TesterSetupError();
	}
	~SetTester() {
		if(_res)free(_res);
		if(_time) free(_time);
	}
	void* generateInts(size_t objnum);
	int test(int& time, void* input, size_t input_size, size_t objnum, size_t* objsizes);
	void runTests(int test_size) override;
	int test_create_destroy(int& time, int test_size);
	int test_insert_find_remove(int& time, int test_size);
	int test_size(int& time, int test_size);
	int test_check_unique(int& time, int test_size);
	int test_diff_types(int& time, int test_size);
	int test_big_size(int& time, int test_size);
	int test_big_size_half_find_del(int& time, int test_size);
	void print_results();
};
