/*// Return true if the somePredicate function returns true for at
	 // least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n)
{
	if (n <= 0)
		return false;

	if (somePredicate(a[n - 1]))
		return true;

	if (n - 1 <= 0)
		return false;

	return anyTrue(a, n - 1);
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
	if (n <= 0)
		return 0;

	int count = 0;

	if (somePredicate(a[n - 1]))
		count++;

	if (n - 1 > 0)
		count += countTrue(a, n - 1);

	return count;
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
	int index = 0;

	if (!somePredicate(a[0]) && n != 0)
	{
		index = 1;
		index += firstTrue(a + 1, n);
	}
	if (index == n)
		index = index - (n + 1);
	return index;
}

// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told to examine no 
// elements, return -1.
int positionOfMin(const double a[], int n)
{
	if (n <= 0)
		return -1;

	if (n == 1)
		return 0;

	if (n > 1)
	{
		double x = a[positionOfMin(a, n - 1)];
		if (a[n - 1] < x)
			return n - 1;
	}

	return positionOfMin(a, n - 1);

}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 == 0)
		return true;

	if (n1 > 0)
	{
		if (a2[n2 - 1] != a1[n1 - 1])
			return includes(a1, n1 - 1, a2, n2);

		if (n2 == 1)
			return true;

		return includes(a1, n1 - 1, a2, n2 - 1);
	}

	return false;

}

*/