/*#include <string>
#include <queue>
#include <iostream>


using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> coord_queue;
	Coord start(sr, sc);
	Coord end(er, ec);

	int rows = maze->size();
	if (rows != nRows)
		return false;

	for (int i = 0; i < nRows; i++)
	{
		int cols = maze[i].size();
		if (cols != nCols)
			return false;
	}

	coord_queue.push(start);
	maze[sr][sc] = 'D';

	while (!coord_queue.empty())
	{
		Coord p = coord_queue.front();
		coord_queue.pop();

		if (p.r() == end.r() && p.c() == end.c())
			return true;

		if (maze[p.r() + 1][p.c()] != 'X' && maze[p.r() + 1][p.c()] != 'D') //SOUTH
		{
			maze[p.r() + 1][p.c()] = 'D';
			int new_r = p.r() + 1;
			int new_c = p.c();
			coord_queue.push(Coord(new_r, new_c));
		}

		if (maze[p.r()][p.c() - 1] != 'X' && maze[p.r()][p.c() - 1] != 'D') // WEST
		{
			maze[p.r()][p.c() - 1] = 'D';
			int new_r = p.r();
			int new_c = p.c() - 1;
			coord_queue.push(Coord(new_r, new_c));
		}

		if (maze[p.r() - 1][p.c()] != 'X' && maze[p.r() - 1][p.c()] != 'D') //NORTH
		{
			maze[p.r() - 1][p.c()] = 'D';
			int new_r = p.r() - 1;
			int new_c = p.c();
			coord_queue.push(Coord(new_r, new_c));
		}

		if (maze[p.r()][p.c() + 1] != 'X' && maze[p.r()][p.c() + 1] != 'D') // EAST
		{
			maze[p.r()][p.c() + 1] = 'D';
			int new_r = p.r();
			int new_c = p.c() + 1;
			coord_queue.push(Coord(new_r, new_c));
		}
	}
	return false;
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X....X...X",
		"X.XX.XX..X",
		"XXX....X.X",
		"X.XXX.XXXX",
		"X.X...X..X",
		"X...X.X..X",
		"XXXXX.X.XX",
		"X........X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 3, 5, 8, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}

*/