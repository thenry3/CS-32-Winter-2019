#include "newSet.h"
#include <algorithm>
#include <iostream>

using namespace std;


Set::Set(int items)
{
	maximum = items;
	item_array_ = new ItemType[maximum];
	insert_count = 0;
}

Set::Set(const Set& src)
{
	insert_count = src.insert_count;
	maximum = src.maximum;
	item_array_ = new ItemType[src.maximum];
	for (int x = 0; x < maximum; x++)
	{
		item_array_[x] = src.item_array_[x];
	}
}

Set& Set::operator=(const Set& src)
{
	insert_count = src.insert_count;
	maximum = src.maximum;
	item_array_ = new ItemType[src.maximum];
	for (int i = 0; i < src.maximum; i++)
	{
		item_array_[i] = src.item_array_[i];
	}
	return *this;
}


bool Set::empty() const
{
	if (size() == 0)
		return true;
	return false;
}

int Set::size() const
{
	return insert_count;
}

bool Set::insert(const ItemType& value)
{
	if (contains(value) || size() == maximum)
	{
		return false;
	}
	item_array_[size()] = value;
	insert_count++;
	return true;
}

bool Set::erase(const ItemType& value)
{
	if (contains(value))
	{
		int count = 0;
		while (value != item_array_[count])
		{
			++count;
		}
		for (int i = count; i < size(); i++)
		{
			item_array_[i] = item_array_[i + 1];
		}
		insert_count--;
		return true;
	}
	return false;
}

bool Set::contains(const ItemType& value) const
{
	for (int i = 0; i < insert_count; i++)
	{
		if (value == item_array_[i])
			return true;
	}
	return false;
}

bool Set::get(int i, ItemType& value) const
{
	if (0 <= i && i < size())
	{
		ItemType *list = new ItemType[maximum];
		for (int x = 0; x < maximum; x++)
		{
			list[x] = item_array_[x];
		}
		for (int b = 0; b < size(); b++)
		{
			for (int a = 0; a < size(); a++)
			{
				if (list[a] > list[a + 1] && a + 1 < size())
				{
					ItemType var = list[a];
					list[a] = list[a + 1];
					list[a + 1] = var;
				}
			}
		}

		value = list[i];
		return true;
	}
	return false;
}

void Set::swap(Set& other)
{
	Set *temp = this;
	Set *temp2 = &other;
	Set temp3 = *this;

	*temp = other;
	*temp2 = temp3;
}