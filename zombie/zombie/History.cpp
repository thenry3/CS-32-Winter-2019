#include <iostream>
#include "History.h"
#include "globals.h"

using namespace std;

History::History(int nRows, int nCols)
{
	h_rows = nRows;
	h_cols = nCols;
	int a, b;
	for (a = 0; a < h_rows; a++)
	{
		for (b = 0; b < h_cols; b++)
		{
			h_grid[a][b] = '.';
		}
	};
}

bool History::record(int r, int c)
{
	if (r < 1 || r > h_rows || c < 1 || c > h_cols)
	{
		return false;
	}

	switch (h_grid[r-1][c-1])
	{
	default: h_grid[r-1][c-1]++; break;
	case '.': h_grid[r-1][c-1] = 'A'; break;
	case 'Z': h_grid[r-1][c-1] = 'Z'; break;
	};

	return true;


}
	
void History::display() const
{
	int r, c;
	for (r = 0; r < h_rows; r++)
	{
		for (c = 0; c < h_cols; c++)
		{
			cout << h_grid[r][c];
		}
		cout << endl;
	}
	cout << endl;
}




