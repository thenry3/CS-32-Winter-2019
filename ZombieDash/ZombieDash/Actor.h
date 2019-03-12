#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "GameWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

// Base Class
class Actor: public GraphObject
{
public:
	Actor(StudentWorld* s, int ID, double x, double y, Direction dir, int depth, double size);
	virtual ~Actor();

	//virtual functions
	virtual void uponDeath() = 0;
	virtual void doSomething() = 0;

	// helping functions
	void setAliveStatus(bool status);
	bool isAlive() const;
	StudentWorld* get_sworld() const;
	bool isAbleToWalkOver() const;
	bool isLiving() const;
	bool isKillable() const;
	bool isZombie() const;

	// functions to infect
	virtual int getInfectCount() const { return -1; }
	virtual bool isInfected() const { return false; }
	virtual void increaseInfection() {}
	virtual void setInfected(bool n) {}
	virtual void setInfectCounter(int n) {}
	
protected:
	void setAbleToWalkOver(bool n);
	void setLiving(bool n);
	void setCanBeDamaged(bool n);
	void setZombie(bool n);

private:
	StudentWorld* sworld;
	bool AliveStatus;       // status of obj
	bool AbleToWalkOver;    // able to walk over
	bool Living;            // is a human?
	bool CanBeDamaged;      // goodies, humans, zombies, mines,
	bool Zombie;
	
};


// Subclasses
class WalkingThing: public Actor
{
public:
	WalkingThing(StudentWorld* s, int ID, double column, double row);
	bool isParalyzed();

private:
	int ParalysisTick;
};

class Human: public WalkingThing
{
public:
	Human(StudentWorld* s, int ID, double column, double row);


	virtual int getInfectCount() const;
	virtual bool isInfected() const;
	virtual void increaseInfection();
	virtual void setInfected(bool n);
	virtual void setInfectCounter(int n);

private:
	bool Infected;
	int InfectCounter;
};

class Zombie: public WalkingThing
{
public:
	Zombie(StudentWorld* s, double column, double row);
	virtual ~Zombie();
	void computeVomitCoordinates(int& x, int& y) const;
	bool ZombieVomit();
	int getMovePlan() const;
	void setMovePlan(int distance);
	void setRandomDir();
	void moveZombie();

private:
	int MovePlanDistance;
};

class Goodie: public Actor
{
public:
	Goodie(StudentWorld* s, int ID, double column, double row);
	virtual void doSomething();
};

class Projectile: public Actor
{
public:
	Projectile(StudentWorld* s, int ID, double column, double row, Direction dir);
	virtual void doSomething();
	void Kill();

private:
	int timer;
};


// Specific Classes
class Penelope: public Human
{
public:
	Penelope(StudentWorld* s, double column, double row);

	virtual void doSomething();
	virtual void uponDeath() {}
};

class Citizen: public Human
{
public:
	Citizen(StudentWorld* s, double column, double row);
	virtual void doSomething();
	virtual void uponDeath() {}
};

class Wall: public Actor
{
public:
	Wall(StudentWorld* s, double column, double row);
	virtual void doSomething() {} // It's a wall so it does nothing.
	virtual void uponDeath() {}
};

class Exit: public Actor
{
public:
	Exit(StudentWorld* s, double column, double row);
	virtual void doSomething();
	virtual void uponDeath() {}
};

class DumbZombie: public Zombie
{
public:
	DumbZombie(StudentWorld* s, double column, double row);
	~DumbZombie();
	virtual void doSomething();
	virtual void uponDeath();
};

class SmartZombie: public Zombie
{
public:
	SmartZombie(StudentWorld* s, double column, double row);
	~SmartZombie();
	virtual void doSomething();
	virtual void uponDeath(){}
};

class Vaccine: public Goodie
{
public:
	Vaccine(StudentWorld* s, double column, double row);
	virtual void doSomething();
	virtual void uponDeath(){}
};

class GasCan: public Goodie
{
public:
	GasCan(StudentWorld* s, double column, double row);
	virtual void doSomething();
	virtual void uponDeath(){}
};

class LandmineGoodie: public Goodie
{
public:
	LandmineGoodie(StudentWorld* s, double column, double row);
	virtual void doSomething();
	virtual void uponDeath() {}
};

class Flame: public Projectile
{
public:
	Flame(StudentWorld* s, int column, int row, Direction dir);
	virtual void doSomething();
	virtual void uponDeath() {}
};

class Landmine: public Actor
{
public:
	Landmine(StudentWorld* s, double column, double row);

	virtual void doSomething();
	virtual void uponDeath();

private:
	bool Active;
	int safetyTicks;
};

class Pit: public Projectile
{
public:
	Pit(StudentWorld* s, int column, int row);
	virtual void doSomething();
	virtual void uponDeath(){}
};

class Vomit: public Projectile
{
public:
	Vomit(StudentWorld* s, double column, double row, Direction dir);
	virtual void doSomething();
	virtual void uponDeath() {}
};


#endif // ACTOR_H_
