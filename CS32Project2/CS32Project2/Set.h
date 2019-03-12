#ifndef SET
#define SET

#include <iostream>
#include <string>

using namespace std;

using ItemType = string;

class Set
{
public:
	Set();
	~Set();
	Set(const Set &src);
	Set &operator=(const Set &src);

	bool empty() const;
	int size() const;
	bool insert(const ItemType& value);
	bool erase(const ItemType& value);
	bool contains(const ItemType& value) const;
	bool get(int pos, ItemType& value) const;
	void swap(Set& other);

private:
	struct Node
	{
		ItemType value;
		Node *next;
		Node *prev;
	};
	Node *head;
};

void unite(const Set& s1, const Set& s2, Set& result);

void subtract(const Set& s1, const Set& s2, Set& result);

#endif