#include "Set.h"
#include <algorithm>
#include <iostream>

using namespace std;


Set::Set()
{
	ItemType item_array_;
	insert_count = 0;
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
	if (contains(value) || size() == DEFAULT_MAX_ITEMS)
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
		ItemType *list = new ItemType[DEFAULT_MAX_ITEMS];
		for (int x = 0; x < DEFAULT_MAX_ITEMS; x++)
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
		delete[]list;
		return true;
	}
	return false;
}

void Set::swap(Set& other)
{
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
	{
		ItemType *temp = &this->item_array_[i];
		ItemType *temp2 = &other.item_array_[i];
		ItemType temp3 = this->item_array_[i];

		*temp = other.item_array_[i];
		*temp2 = temp3;
	}
	int *temp4 = &this->insert_count;
	int *temp5 = &other.insert_count;
	int temp6 = this->insert_count;

	*temp4 = other.insert_count;
	*temp5 = temp6;
}







