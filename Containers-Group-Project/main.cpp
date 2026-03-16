#include <iostream>
#include "List2.h"
#include "List.h"
#include "Mem.h"
using namespace std;
static void printList2(List2 &l)
{
	List2::Iterator* it = l.newIterator();
	if (!it) {
		cout << "<empty>\n";
		return;
	}
	while (true) {
		size_t sz;
		int *v = (int*)it->getElement(sz);
		if (v) cout << *v << " ";
		if (it->hasNext()) it->goToNext(); else break;
	}
	cout << endl;
	delete it;
}

static void printList(List &l)
{
	List::Iterator* it = l.newIterator();
	if (!it) {
		cout << "<empty>\n";
		return;
	}
	while (true) {
		size_t sz;
		int *v = (int*)it->getElement(sz);
		if (v) cout << *v << " ";
		if (it->hasNext()) it->goToNext(); else break;
	}
	cout << endl;
	delete it;
}

static void testList(Mem& mem)
{
	cout << "=== Testing List (singly linked) ===" << endl;
	List l(mem);
	cout << "Test: push_front 3,2,1 and traversal" << endl;
	int v1 = 3, v2 = 2, v3 = 1;
	l.push_front(&v1, sizeof(int));
	l.push_front(&v2, sizeof(int));
	l.push_front(&v3, sizeof(int));
	printList(l); // expected: 1 2 3

	cout << "size: " << l.size() << "\n";

	cout << "Test: insert 7 before 2 (using find)" << endl;
	int seven = 7;
	List::Iterator* found = l.find(&v2, sizeof(int)); // find 2
	if (found) {
		l.insert(found, &seven, sizeof(int));
	}
	printList(l); // expected: 1 7 2 3

	cout << "Test: remove 7 (using find + remove)" << endl;
	List::Iterator* found7 = l.find(&seven, sizeof(int));
	if (found7) {
		l.remove(found7);
	}
	printList(l); // expected: 1 2 3

	cout << "Test: pop_front" << endl;
	l.pop_front(); // removes 1
	printList(l); // expected: 2 3

	cout << "Test: clear and empty" << endl;
	l.clear();
	printList(l); // expected: <empty>
	cout << "empty: " << (l.empty() ? "true" : "false") << endl;
}

static void testList2(Mem& mem)
{
	cout << "=== Testing List2 (doubly linked) ===" << endl;
	List2 l(mem);

	cout << "Test: push_back 5,10,15 and traversal" << endl;
	int a = 5, b = 10, c = 15;
	l.push_back(&a, sizeof(int));
	l.push_back(&b, sizeof(int));
	l.push_back(&c, sizeof(int));
	printList2(l); // expected: 5 10 15

	cout << "size: " << l.size() << "\n";

	cout << "front/back: ";
	size_t sz;
	int *fv = (int*)l.front(sz);
	int *bv = (int*)l.back(sz);
	if (fv) cout << *fv << " ";
	if (bv) cout << *bv << "\n";

	cout << "Test: push_front 1" << endl;
	int one = 1;
	l.push_front(&one, sizeof(int));
	printList2(l); // expected: 1 5 10 15

	cout << "Test: insert 7 before 10 (using find)" << endl;
	int seven = 7;
	// find iterator must not be used to call methods (it's allocated by MemoryManager in find)
	List2::Iterator* found = l.find(&b, sizeof(int)); // find 10
	if (found) {
		l.insert(found, &seven, sizeof(int)); // insert 7 before 10
		// do not delete 'found' (allocated by MemoryManager)
	}
	printList2(l); // expected: 1 5 7 10 15

	cout << "Test: remove 7 (using find + remove)" << endl;
	List2::Iterator* found7 = l.find(&seven, sizeof(int));
	if (found7) {
		l.remove(found7);
		// do not delete 'found7'
	}
	printList2(l); // expected: 1 5 10 15

	cout << "Test: pop_front and pop_back" << endl;
	l.pop_front(); // removes 1
	l.pop_back();  // removes 15
	printList2(l); // expected: 5 10
	cout << "size: " << l.size() << "\n";

	cout << "Test: clear and empty" << endl;
	l.clear();
	printList2(l); // expected: <empty>
	cout << "empty: " << (l.empty() ? "true" : "false") << endl;
}

int main() {
	Mem mem(1024);
	testList(mem);
	cout << std::endl;
	testList2(mem);
	return 0;
}
