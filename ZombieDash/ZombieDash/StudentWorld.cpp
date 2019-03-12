#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Level.h"
#include <iomanip>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}


int StudentWorld::init()
{
	Level lev(assetPath());

	ostringstream level_string;
	level_string << setfill('0') << setw(2) << getLevel();
	string s = level_string.str();

	if (getLevel() > 99)
		return GWSTATUS_PLAYER_WON;


	string levelFile = "level" + s + ".txt";
	Level::LoadResult result = lev.loadLevel(levelFile);

	if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
	if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;
	if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;

		for (int i = 0; i < LEVEL_WIDTH; i++)             
		{
			for (int j = 0; j < LEVEL_HEIGHT; j++)
			{
				Level::MazeEntry ge = lev.getContentsOf(i, j);  // Create new object for each grid spot
				switch (ge) 
				{
					case Level::empty:
						break;
					case Level::player:
						player = new Penelope(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
						break;
					case Level::wall:
						Actors.push_back(new Wall(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j));
						break;
					case Level::exit:
						Actors.push_back(new Exit(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j));
						break;
					case Level::dumb_zombie:
						Actors.push_back(new DumbZombie(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j));
						break;
					case Level::vaccine_goodie:
						Actors.push_back(new Vaccine(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j));
						break;
					case Level::gas_can_goodie:
						Actors.push_back(new GasCan(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j));
						break;
					case Level::landmine_goodie:
						Actors.push_back(new LandmineGoodie(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j));
						break;
					case Level:: citizen:
						Actors.push_back(new Citizen(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j));
						break;
					case Level:: smart_zombie:
						Actors.push_back(new SmartZombie(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j));
						break;
					case Level:: pit:
						Actors.push_back(new Pit(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j));
						break;
				}
			}
		}
	}


	numOfGas = 0;
	numOfLandmines = 0;
	numOfVaccines = 0;
	Finished = false;
	updateGameStat();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	player->doSomething();
	for (auto x : Actors)
	{
		if (x->isAlive())
		{
			x->doSomething();
			if (!player->isAlive())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}

			if (isFinished())
			{
				playSound(SOUND_LEVEL_FINISHED);
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}

	vector<Actor*>::iterator itr = Actors.begin();
		
    while (itr < Actors.end())
    {
		if (!(*itr)->isAlive())
		{
			delete *itr;
			Actors.erase(itr);
			itr = Actors.begin();
		}
		else
			itr++;
    }

	if (!player->isAlive())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	updateGameStat();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete player;

	while (Actors.size() > 0)
	{
		delete Actors.back();
		Actors.pop_back();
	}
}


// Mutators
void StudentWorld::addActor(Actor* obj)
{
	Actors.push_back(obj);
}

void StudentWorld::setFinishedState(bool n)
{
	Finished = n;
}

void StudentWorld::increaseVaccines(int n)
{
	numOfVaccines += n;
}

void StudentWorld::increaseGas(int n)
{
	numOfGas += n;
}

void StudentWorld::increaseLandmines(int n)
{
	numOfLandmines += n;
}


// Accessors
bool StudentWorld::isFinished() const
{
	return Finished;
}

int StudentWorld::getVaccines() const
{
	return numOfVaccines;
}

int StudentWorld::getGas() const
{
	return numOfGas;
}

int StudentWorld::getLandmines() const
{
	return numOfLandmines;
}

Penelope* StudentWorld::getPenelope() const
{
	return player;
}



// Helpers
double StudentWorld::Euclidean_distance(int x1, int y1, int x2, int y2) const
{
	return pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);
}

bool StudentWorld::CitizensGone() const
{
	for (auto x : Actors)
	{
		if (x->isLiving())
			return false;
	}
	return true;
}

Actor* StudentWorld::doesOverlapCitizen(int x, int y) const
{
	int x1 = x + SPRITE_WIDTH / 2;
	int y1 = y + SPRITE_HEIGHT / 2;

	for (auto a : Actors)
	{
		if (a->isLiving())
		{
			int x2 = a->getX() + SPRITE_WIDTH / 2;
			int y2 = a->getY() + SPRITE_HEIGHT / 2;

			if (Euclidean_distance(x1, y1, x2, y2) <= 10)
			{
				return a;
			}
		}
	}

	return nullptr;
}

Actor* StudentWorld::doesOverlapKillables(Actor* a) const
{
	int x1 = a->getX() + SPRITE_WIDTH / 2;
	int y1 = a->getY() + SPRITE_HEIGHT / 2;

	for (auto x : Actors)
	{
		if (x->isKillable())
		{
			int x2 = x->getX() + SPRITE_WIDTH / 2;
			int y2 = x->getY() + SPRITE_HEIGHT / 2;

			if (Euclidean_distance(x1, y1, x2, y2) <= 10)
			{
				return x;
			}
		}
	}

	return nullptr;
}

bool StudentWorld::OverlapKillables(Actor* a) const
{
	int x1 = a->getX() + SPRITE_WIDTH / 2;
	int y1 = a->getY() + SPRITE_HEIGHT / 2;

	for (auto x : Actors)
	{
		if (x->isKillable() && x != a)
		{
			int x2 = x->getX() + SPRITE_WIDTH / 2;
			int y2 = x->getY() + SPRITE_HEIGHT / 2;

			if (Euclidean_distance(x1, y1, x2, y2) <= 10)
			{
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::doesOverlapPenelope(int x, int y) const
{
	int x1 = x + SPRITE_WIDTH / 2;
	int y1 = y + SPRITE_HEIGHT / 2;

	int x2 = player->getX() + SPRITE_WIDTH / 2;
	int y2 = player->getY() + SPRITE_WIDTH / 2;

	if (Euclidean_distance(x1, y1, x2, y2) <= 10)
	{
		return true;
	}

	return false;
}

bool StudentWorld::doesOverlapWall(int column, int row) const
{
	int x1 = column + SPRITE_WIDTH / 2;
	int y1 = row + SPRITE_HEIGHT / 2;

	for (auto x : Actors)
	{
		if (!x->isKillable())
		{
			int x2 = x->getX() + SPRITE_WIDTH / 2;
			int y2 = x->getY() + SPRITE_HEIGHT / 2;

			if (Euclidean_distance(x1, y1, x2, y2) <= 10)
			{
				return true;
			}
		}
	}

	return false;
}

bool StudentWorld::canZombieWalk(int column, int row, Actor* zombie)
{
	int min_x = column;
	int max_x = column + SPRITE_WIDTH - 1;
	int min_y = row;
	int max_y = row + SPRITE_HEIGHT - 1;

	for (auto x : Actors)
	{
		if (!x->isAbleToWalkOver() && zombie != x)
		{
			int min_x1 = x->getX();
			int max_x1 = x->getX() + SPRITE_WIDTH - 1;
			int min_y1 = x->getY();
			int max_y1 = x->getY() + SPRITE_HEIGHT - 1;

			if (((min_x <= min_x1 && min_x1 <= max_x) || (max_x >= max_x1 && max_x1 >= min_x)) && ((min_y <= min_y1 && min_y1 <= max_y) || (max_y >= max_y1 && max_y1 >= min_y)))
				return false;
		}
	}

	int p_min_x1 = player->getX();
	int p_max_x1 = player->getX() + SPRITE_WIDTH - 1;
	int p_min_y1 = player->getY();
	int p_max_y1 = player->getY() + SPRITE_HEIGHT - 1;

	if (((min_x < p_min_x1 && p_min_x1 < max_x) || (max_x > p_max_x1 && p_max_x1 > min_x)) && ((min_y < p_min_y1 && p_min_y1 < max_y) || (max_y > p_max_y1 && p_max_y1 > min_y)))
		return false;

	return true;
}

bool StudentWorld::canHumanWalk(int column, int row, Actor* human)
{
	int min_x = column;
	int max_x = column + SPRITE_WIDTH - 1;
	int min_y = row;
	int max_y = row + SPRITE_HEIGHT - 1;

	for (auto x : Actors)
	{
		if (!x->isAbleToWalkOver() && human != x)
		{
			int min_x1 = x->getX();
			int max_x1 = x->getX() + SPRITE_WIDTH - 1;
			int min_y1 = x->getY();
			int max_y1 = x->getY() + SPRITE_HEIGHT - 1;

			if (((min_x <= min_x1 && min_x1 <= max_x) || (max_x >= max_x1 && max_x1 >= min_x)) && ((min_y <= min_y1 && min_y1 <= max_y) || (max_y >= max_y1 && max_y1 >= min_y)))
				return false;
		}
	}

	if (human != player)
	{
		int p_min_x1 = player->getX();
		int p_max_x1 = player->getX() + SPRITE_WIDTH - 1;
		int p_min_y1 = player->getY();
		int p_max_y1 = player->getY() + SPRITE_HEIGHT - 1;

		if (((min_x < p_min_x1 && p_min_x1 < max_x) || (max_x > p_max_x1 && p_max_x1 > min_x)) && ((min_y < p_min_y1 && p_min_y1 < max_y) || (max_y > p_max_y1 && p_max_y1 > min_y)))
			return false;
	}
	return true;
}

Actor* StudentWorld::nearestHuman(Actor* zombie)
{
	Actor* closeHuman = nullptr;
	int humancount = 0;

	int zombie_x = zombie->getX();
	int zombie_y = zombie->getY();

	for (auto x : Actors)
	{
		if (x->isLiving())
		{
			if (humancount == 0)
				closeHuman = x;
			else if (Euclidean_distance(zombie_x, zombie_y, x->getX(), x->getY()) <= Euclidean_distance(zombie_x, zombie_y, closeHuman->getX(), closeHuman->getY()))
				closeHuman = x;
			humancount++;
		}
	}

	if (closeHuman == nullptr)
		closeHuman = player;
	else if (Euclidean_distance(zombie_x, zombie_y, player->getX(), player->getY()) <= Euclidean_distance(zombie_x, zombie_y, closeHuman->getX(), closeHuman->getY()))
		closeHuman = player;

	return closeHuman;
}

double StudentWorld::distanceToPenelope(Actor* a)
{
	return Euclidean_distance(a->getX(), a->getY(), player->getX(), player->getY());
}

double StudentWorld::distanceToNearZombie(int column, int row)
{
	double dis = -1;
	bool zombieinActors = false;
	for (auto x : Actors)
	{
		if (x->isZombie())
		{
			if (!zombieinActors)
			{
				dis = Euclidean_distance(column, row, x->getX(), x->getY());
				zombieinActors = true;
			}
			else if (Euclidean_distance(column, row, x->getX(), x->getY()) <= dis)
				dis = Euclidean_distance(column, row, x->getX(), x->getY());
		}
	}

	return dis;
}

void StudentWorld::updateGameStat()
{
	ostringstream score;

	// Format score correctly if negative
	if (getScore() < 0)
	{
		int newScore = abs(getScore());
		score << "Score: -" << setfill('0') << setw(6) << newScore;
	}
	else
		score << "Score: " << setfill('0') << setw(6) << getScore();
	string score_component = score.str();

	// Format everything needed
	string level = "Level: " + to_string(getLevel());
	string lives = "Lives: " + to_string(getLives());
	string vaccines = "Vaccines: " + to_string(getVaccines());
	string flames = "Flames: " + to_string(getGas());
	string mines = "Mines: " + to_string(getLandmines());
	string infected = "Infected: " + to_string(player->getInfectCount());

	// Make the whole stat one string
	string stat = score_component + "  " + level + "  " + lives + "  " + vaccines + "  " + flames + "  " + mines + "  " + infected;

	setGameStatText(stat);
}









