#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Actor.h"
#include <vector>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(string assetPath);
	virtual ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();

	// Mutators
	void addActor(Actor* obj);
	void setFinishedState(bool n);
	void increaseVaccines(int n);
	void increaseGas(int n);
	void increaseLandmines(int n);

	// Accessors
	bool isFinished() const;
	int getVaccines() const;
	int getGas() const;
	int getLandmines() const;
	Penelope* getPenelope() const;

	// Helpers
	double Euclidean_distance(int x1, int y1, int x2, int y2) const;
	bool CitizensGone() const;
	Actor* doesOverlapCitizen(int x, int y) const;
	Actor* doesOverlapKillables(Actor *a) const;
	bool OverlapKillables(Actor* a) const;
	bool doesOverlapPenelope(int x, int y) const;
	bool doesOverlapWall(int column, int row) const;
	bool canZombieWalk(int column, int row, Actor* zombie);
	bool canHumanWalk(int column, int row, Actor* human);
	Actor* nearestHuman(Actor* zombie);
	double distanceToPenelope(Actor* a);
	double distanceToNearZombie(int column, int row);
	void updateGameStat();

private:
	Penelope* player;
	vector<Actor*> Actors;
	bool Finished;
	int numOfVaccines;
	int numOfGas;
	int numOfLandmines;
};

#endif // STUDENTWORLD_H_
