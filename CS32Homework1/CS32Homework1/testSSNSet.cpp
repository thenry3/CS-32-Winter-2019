#include "SSNSet.h"
#include <iostream>

using namespace std;

int main()
{
	SSNSet s;
	s.add(10);
	s.add(20);
	cout << s.size() << endl;
	s.print();
}