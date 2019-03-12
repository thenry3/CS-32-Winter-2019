#include "Set.h"

using namespace std;

Set::Set()
{
	head = nullptr;
}

Set::~Set()
{
	Node *p = head;
	while (p != nullptr)
	{
		Node *temp = p->next;
		delete p;
		p = temp;
	}
}

Set::Set(const Set &src)
{
	head = nullptr;
	Node *p = src.head;
	while (p != nullptr)
	{
		insert(p->value);
		p = p->next;
	}
}

Set& Set::operator=(const Set& src)
{
	head = src.head;
	return *this;
}


bool Set::empty() const
{
	if (head == nullptr)
		return true;
	return false;
}

int Set::size() const
{
	int count = 0;
	if (empty())
		return 0;
	Node *p = head;
	while (p != nullptr)
	{
		count++;
		p = p->next;
	}
	return count;
}

bool Set::insert(const ItemType& value)
{
	if (contains(value))
		return false;
	if (size() == 0)
	{
		Node *n = new Node;
		n->value = value;
		n->next = nullptr;
		n->prev = nullptr;
		head = n;
		return true;
	}
	Node *n = new Node;
	n->value = value;
	n->next = nullptr;
	Node *p = head;
	while (p->next != nullptr)
		p = p->next;
	p->next = n;
	n->prev = p;
	return true;
}

bool Set::erase(const ItemType& value)
{
	if (!contains(value) || size() == 0)
		return false;

	if (size() == 1)
	{
		Node *n = head;
		head = nullptr;
		delete n;
		return true;
	}

	if (head->value == value)
	{
		Node *n = head;
		head = head->next;
		head->prev = nullptr;
		delete n;
		return true;
	}
	
	Node *p = head;
	while (p != nullptr)
	{
		if (p->next->value == value && p->next != nullptr)
			break;
		p = p->next;
	}

	if (p->next->next == nullptr)
	{
		Node *temp = p->next;
		p->next = nullptr;
		delete temp;
		return true;
	}

	Node *temp = p->next;
	p->next = temp->next;
	temp->next->prev = p;
	delete temp;
	return true;
}

bool Set::contains(const ItemType& value) const
{
	Node *p = head;
	while (p != nullptr)
	{
		if (p->value == value)
			return true;
		p = p->next;
	}
	return false;
}

bool Set::get(int pos, ItemType& value) const
{
	if (pos >= size() || pos < 0)
		return false;
	Set temp = *this;
	if (size() == 2)
	{
		Node *n = temp.head;
		if (n->value > n->next->value)
		{
			temp.head = n->next;
			n->prev = n->next;
			n->next = nullptr;
			temp.head->next = n;
		}
	}
	else if (size() > 2)
	{
		for (int i = 0; i < size(); i++)
		{
			Node *n = temp.head;
			while (n->next != nullptr)
			{
				if (n->value > n->next->value)
				{
					if (n->prev == nullptr)
					{
						temp.head = n->next;
						n->next->next->prev = n;
						n->next = n->next->next;
						temp.head->next = n;

						temp.head->prev = nullptr;
						n->prev = temp.head;
					}
					else if (n->next->next == nullptr)
					{
						n->prev->next = n->next;
						n->next->prev = n->prev;
						n->prev = n->next;
						n->next = nullptr;
						n->prev->next = n;
					}
					else
					{
						n->prev->next = n->next;
						n->next->prev = n->prev;
						n->prev = n->next;
						n->next = n->next->next;
						n->prev->next = n;
						n->next->prev = n;
					}

				}
				if (n->next == nullptr)
					continue;
				n = n->next;
			}
		}
	}
	
	if (pos == 0)
		value = temp.head->value;
	else
	{
		Node *f = temp.head;
		for (int i = 0; i < pos; i++)
		{
			f = f->next;
		}
		value = f->value;
	}
	return true;
}

void Set::swap(Set& other)
{
	Node *temp1 = this->head;

	this->head = other.head;
	other.head = temp1;
}

void unite(const Set& s1, const Set& s2, Set& result)
{
	Set *f = &result;
	
	if (!f->empty())
	{
		for (int i = 0; i < f->size(); i++)
		{
			ItemType x;
			f->get(i, x);
			f->erase(x);
		}
	}

	for (int i = 0; i < s1.size(); i++)
	{
		ItemType x;
		s1.get(i, x);
		if (!f->contains(x))
			f->insert(x);
	}

	for (int i = 0; i < s2.size(); i++)
	{
		ItemType x;
		s2.get(i, x);
		if (!f->contains(x))
			f->insert(x);
	}
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
	Set *f = &result;
	
	if (!f->empty())
	{
		for (int i = 0; i < result.size(); i++)
		{
			ItemType x;
			f->get(i, x);
			f->erase(x);
		}
	}

	for (int i = 0; i < s1.size(); i++)
	{
		ItemType x;
		s1.get(i, x);

		if (!s2.contains(x))
			f->insert(x);
	}
}


