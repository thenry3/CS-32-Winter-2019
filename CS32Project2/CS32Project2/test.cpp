#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Set a;
	a.insert(2);
	a.insert(8);
	a.insert(3);
	a.insert(9);
	a.insert(5);

	Set b;
	b.insert(6);
	b.insert(3);
	b.insert(8);
	b.insert(5);
	b.insert(10);

	Set c;
	c.insert(1);

	Set d;
	d.insert(5);

	subtract(a, b, c);
	unite(a, b, d);

	assert(c.contains(2) && c.contains(9) && d.contains(9) && d.contains(3) && d.contains(6) && d.contains(5) && d.contains(10) && d.contains(2) && d.contains(8));
}


int main()
{
	test();
	cout << "Passed all tests" << endl;
}

/*#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;


void test()
{
	Set ss;
	//assert(ss.size() == 0); //test size when empty
	assert(ss.empty()); //test empty

	assert(ss.insert("roti")); //test insert
	assert(ss.size() == 1); //test size when not empty
	assert(!ss.empty()); //test for not empty

	assert(ss.insert("pita"));
	assert(ss.contains("pita")); //test contain
	assert(!ss.contains("hello")); //test contain

	assert(ss.insert("hi"));
	assert(ss.erase("hi")); //test erase
	assert(!ss.erase("train")); //nothing to remove
	assert(!ss.contains("hi"));

	ItemType x = "laobing";
	assert(ss.get(0, x) && x == "pita"); //test get function
	assert(ss.get(1, x) && x == "roti"); //test get function

	Set aa;
	aa.insert("chicken");
	aa.insert("toy");
	aa.insert("apple");
	aa.swap(ss);
	assert(aa.contains("pita") && aa.contains("roti") && !ss.contains("roti") && ss.contains("chicken") && ss.contains("toy") && ss.contains("apple")); //test swap
}


int main()
{
	test();
	cout << "Passed all tests" << endl;
}*/
