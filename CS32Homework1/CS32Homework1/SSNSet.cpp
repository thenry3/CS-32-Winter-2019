#include "SSNSet.h"
#include <iostream>
#include "Set.h"

class Set;

using namespace std;

SSNSet::SSNSet()
{
	Set s = Set();
}

bool SSNSet::add(unsigned long ssn)
{
	if (s.contains(ssn))
		return false;
	s.insert(ssn);
	return true;
}

int SSNSet::size() const
{
	return s.size();
}

void SSNSet::print() const
{
	for (int i = 0; i < size(); i++)
	{
		unsigned long x;
		s.get(i, x);
		cout << x << endl;
	}
}



