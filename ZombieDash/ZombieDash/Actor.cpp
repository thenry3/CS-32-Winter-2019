#include "Actor.h"
#include "StudentWorld.h"

// Base Class
Actor::Actor(StudentWorld* s, int ID, double x, double y, Direction dir, int depth, double size) : GraphObject(ID, x , y, dir, depth, size)
{
	sworld = s;
	AliveStatus = true;
	AbleToWalkOver = false;
	Living = false;
	CanBeDamaged = true;
	Zombie = false;
}

Actor::~Actor() {}

void Actor::setAliveStatus(bool status)
{
	AliveStatus = status;
}

bool Actor::isAlive() const
{
	return AliveStatus;
}

StudentWorld* Actor::get_sworld() const
{
	return sworld;
}

bool Actor::isAbleToWalkOver() const
{
	return AbleToWalkOver;
}

bool Actor::isLiving() const
{
	return Living;
}

bool Actor::isKillable() const
{
	return CanBeDamaged;
}

bool Actor::isZombie() const
{
	return Zombie;
}

void Actor::setAbleToWalkOver(bool n)
{
	AbleToWalkOver = n;
}

void Actor::setLiving(bool n)
{
	Living = n;
}

void Actor::setCanBeDamaged(bool n)
{
	CanBeDamaged = n;
}

void Actor::setZombie(bool n)
{
	Zombie = n;
}


// Subclasses
WalkingThing::WalkingThing(StudentWorld* s, int ID, double column, double row): Actor(s, ID, column, row, right, 0, 1.0)
{
	ParalysisTick = 0;
}

bool WalkingThing::isParalyzed()
{
	ParalysisTick++;

	if (ParalysisTick % 2 == 0)
		return true;
	return false;
}



Human::Human(StudentWorld* s, int ID, double column, double row): WalkingThing(s, ID, column, row)
{
	setAbleToWalkOver(false);
	setLiving(true);
	Infected = false;
	InfectCounter = 0;
}

bool Human::isInfected() const
{
	return Infected;
}

void Human::increaseInfection()
{
	InfectCounter++;
}

int Human::getInfectCount() const
{
	return InfectCounter;
}

void Human::setInfected(bool n)
{
	Infected = n;
}

void Human::setInfectCounter(int n)
{
	InfectCounter = n;
}


Zombie::Zombie(StudentWorld* s, double column, double row): WalkingThing(s, IID_ZOMBIE, column, row)
{
	setZombie(true);
	MovePlanDistance = 0;
}

Zombie::~Zombie()
{
	if (!isAlive())
	{
		StudentWorld* s = get_sworld();
		s->playSound(SOUND_ZOMBIE_DIE);
	}
}

void Zombie::computeVomitCoordinates(int& x, int& y) const
{
	Direction dir = getDirection();

	switch (dir)
	{
	case left:
		x = getX() - SPRITE_WIDTH;
		y = getY();
		break;
	case right:
		x = getX() + SPRITE_WIDTH;
		y = getY();
		break;
	case up:
		x = getX();
		y = getY() + SPRITE_HEIGHT;
		break;
	case down:
		x = getX();
		y = getY() - SPRITE_HEIGHT;
		break;
	}
}

bool Zombie::ZombieVomit()
{
	StudentWorld* tempWorld = get_sworld();
	int Vomit_X = 0;
	int Vomit_Y = 0;
	Direction dir = getDirection();

	computeVomitCoordinates(Vomit_X, Vomit_Y);

	Actor* Target = nullptr;

	Target = tempWorld->doesOverlapCitizen(Vomit_X, Vomit_Y);

	// Does a Human Overlap with vomit coords
	if (Target != nullptr || tempWorld->doesOverlapPenelope(Vomit_X, Vomit_Y))
	{
		// Calculate 1 in 3 chance to vomit
		if (randInt(1, 3) == 1)
		{
			Vomit* tempVomit = new Vomit(tempWorld, Vomit_X, Vomit_Y, dir);
			tempWorld->addActor(tempVomit);
			tempWorld->playSound(SOUND_ZOMBIE_VOMIT);
			return true;
		}
	}
	return false;
}

int Zombie::getMovePlan() const
{
	return MovePlanDistance;
}

void Zombie::setMovePlan(int distance)
{
	MovePlanDistance = distance;
}

void Zombie::setRandomDir()
{
	int random = randInt(1, 4);
	switch (random)
	{
	case 1:
		setDirection(up);
		break;
	case 2:
		setDirection(right);
		break;
	case 3:
		setDirection(down);
		break;
	case 4:
		setDirection(left);
		break;
	}
}

void Zombie::moveZombie()
{
	StudentWorld* tempWorld = get_sworld();
	Direction dir = getDirection();

	switch (dir)
	{
	case up:
		if (getY() < VIEW_WIDTH && tempWorld->canZombieWalk(getX(), getY() + 3, this))
		{
			moveTo(getX(), getY() + 1);
			setMovePlan(getMovePlan() - 1);
		}
		else
			setMovePlan(0);
		break;
	case down:
		if (getY() > 0 && tempWorld->canZombieWalk(getX(), getY() - 3, this))
		{
			moveTo(getX(), getY() - 1);
			setMovePlan(getMovePlan() - 1);
		}
		else
			setMovePlan(0);
		break;
	case right:
		if (getX() < VIEW_WIDTH && tempWorld->canZombieWalk(getX() + 3, getY(), this))
		{
			moveTo(getX() + 1, getY());
			setMovePlan(getMovePlan() - 1);
		}
		else
			setMovePlan(0);
		break;
	case left:
		if (getX() > 0 && tempWorld->canZombieWalk(getX() - 3, getY(), this))
		{
			moveTo(getX() - 1, getY());
			setMovePlan(getMovePlan() - 1);
		}
		else
			setMovePlan(0);
		break;
	}
}



Goodie::Goodie(StudentWorld* s, int ID, double column, double row): Actor(s, ID, column, row, right, 1, 1.0)
{
	setAbleToWalkOver(true);
}

void Goodie::doSomething()
{
	if (!isAlive())
		return;

	StudentWorld* temp = get_sworld();


	if (temp->doesOverlapPenelope(getX(), getY()))
	{
		temp->increaseScore(50);
		setAliveStatus(false);
		temp->playSound(SOUND_GOT_GOODIE);
	}
}


Projectile::Projectile(StudentWorld* s, int ID, double column, double row, Direction dir): Actor(s, ID, column, row, dir, 0, 1.0)
{
	setAbleToWalkOver(true);
	timer = 0;
	setCanBeDamaged(false);
}

void Projectile::doSomething()
{
	timer++;
	if (timer >= 2)
		setAliveStatus(false);
}

void Projectile::Kill()
{
	StudentWorld* tempWorld = get_sworld();
	Actor* tempActor = nullptr;

	tempActor = tempWorld->doesOverlapKillables(this);

	if (tempActor != nullptr)
	{
		if (tempActor->isLiving())
		{
			tempWorld->increaseScore(-1000);
			tempWorld->playSound(SOUND_CITIZEN_DIE);
		}

		tempActor->setAliveStatus(false);
		tempActor->uponDeath();
	}


	if (tempWorld->doesOverlapPenelope(getX(), getY()))
	{
		tempActor = tempWorld->getPenelope();
		tempWorld->playSound(SOUND_PLAYER_DIE);
		tempActor->setAliveStatus(false);
	}
}



// Specific Classes
Penelope::Penelope(StudentWorld* s, double column, double row): Human(s, IID_PLAYER, column, row)
{
}

void Penelope::doSomething()
{
	if (!isAlive())
		return;

	// Get some data!
	StudentWorld* temp = get_sworld();
	Direction dir = getDirection();
	double column = getX();
	double row = getY();

	// Manage infection
	if (isInfected())
		increaseInfection();

	if (getInfectCount() >= 500) 
	{
		setAliveStatus(false);
		temp->playSound(SOUND_PLAYER_DIE);
		return;
	}

	// User input
	int ch;
	if (temp->getKey(ch))
	{
		// user hit a key during this tick!
		switch (ch)
		{
		case KEY_PRESS_TAB:
			if (temp->getLandmines() > 0)
			{
				Landmine* tempLandmine = new Landmine(temp, getX(), getY());
				temp->addActor(tempLandmine);

				temp->increaseLandmines(-1);
			}
			break;
		case KEY_PRESS_ENTER:
			if (temp->getVaccines() > 0)
			{
				setInfected(false);
				temp->increaseVaccines(-1);
				setInfectCounter(0);
			}
			break;
		case KEY_PRESS_SPACE:
			if (temp->getGas() > 0)
			{
				temp->increaseGas(-1);
				temp->playSound(SOUND_PLAYER_FIRE);

				for (int i = 1; i < 4; i++)
				{

					Flame* temp_flame = nullptr;
					switch(dir)
					{
					case up:
						if (!temp->doesOverlapWall(getX(), getY() + i * SPRITE_HEIGHT))
							temp_flame = new Flame(temp, getX(), getY() + i * SPRITE_HEIGHT, dir);
						break;
					case down:
						if (!temp->doesOverlapWall(getX(), getY() - i * SPRITE_HEIGHT))
							temp_flame = new Flame(temp, getX(), getY() - i * SPRITE_HEIGHT, dir);
						break;
					case left:
						if (!temp->doesOverlapWall(getX() - i * SPRITE_WIDTH, getY()))
							temp_flame = new Flame(temp, getX() - i * SPRITE_WIDTH, getY(), dir);
						break;
					case right:
						if (!temp->doesOverlapWall(getX() + i * SPRITE_WIDTH, getY()))
							temp_flame = new Flame(temp, getX() + i * SPRITE_WIDTH, getY(), dir);
						break;
					}

					if (temp_flame == nullptr)
						break;
					
					temp->addActor(temp_flame);
				}
			}
			break;
		case KEY_PRESS_LEFT:
			if (dir != left)
				setDirection(left);
			if (column > 0 && temp->canHumanWalk(column - 4, row, this))
				moveTo(column - 4, row);
			break;

		case KEY_PRESS_RIGHT:
			if (dir != right)
				setDirection(right);
			if (column < VIEW_WIDTH && temp->canHumanWalk(column + 4, row, this))
				moveTo(column + 4, row);
			break;

		case KEY_PRESS_UP:
			if (dir != up)
				setDirection(up);
			if (row < VIEW_WIDTH && temp->canHumanWalk(column, row + 4, this))
				moveTo(column, row + 4);
			break;

		case KEY_PRESS_DOWN:
			if (dir != down)
				setDirection(down);
			if (row > 0 && temp->canHumanWalk(column, row - 4, this))
				moveTo(column, row - 4);
			break;
		}
	}
}


Citizen::Citizen(StudentWorld* s, double column, double row): Human(s, IID_CITIZEN, column, row)
{	
}

void Citizen::doSomething()
{
	if (!isAlive())
		return;

	StudentWorld* tempWorld = get_sworld();

	if (isInfected())
		increaseInfection();

	if (getInfectCount() >= 500)
	{
		tempWorld->playSound(SOUND_ZOMBIE_BORN);
		tempWorld->increaseScore(-1000);

		// on death, 30% chance for smart 70% chance for dumb zombie to spawn
		switch (randInt(1, 10))
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{DumbZombie* tempdumb = new DumbZombie(tempWorld, getX(), getY());
		tempWorld->addActor(tempdumb);
		break; }
		case 8:
		case 9:
		case 10:
		{SmartZombie* tempsmart = new SmartZombie(tempWorld, getX(), getY());
		tempWorld->addActor(tempsmart);
		break; }
		}
		setAliveStatus(false);

		return;
	}

	// Paralysis ticks
	if (isParalyzed())
		return;

	double dist_p = tempWorld->distanceToPenelope(this);
	double dist_z = tempWorld->distanceToNearZombie(getX(), getY());

	if ((dist_p < dist_z || dist_z == -1) && dist_p <= 80)
	{
		// if citizen on same column
		if (getX() == tempWorld->getPenelope()->getX())
		{
			if (getY() > tempWorld->getPenelope()->getY())      // if above
			{
				if (tempWorld->canHumanWalk(getX(), getY() - 2, this) && !tempWorld->doesOverlapPenelope(getX(), getY() - 2))
				{
					setDirection(down);
					moveTo(getX(), getY() - 2);
					return;
				}
			}	
			else
			{
				if (tempWorld->canHumanWalk(getX(), getY() + 2, this) && !tempWorld->doesOverlapPenelope(getX(), getY() + 2))   // if below
				{
					setDirection(up);
					moveTo(getX(), getY() + 2);
					return;
				}
			}
		}
		// if citizen on same row
		else if (getY() == tempWorld->getPenelope()->getY())
		{
			if (getX() > tempWorld->getPenelope()->getX())   // if to the right
			{
				if (tempWorld->canHumanWalk(getX() - 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() - 2, getY()))
				{
					setDirection(left);
					moveTo(getX() - 2, getY());
					return;
				}
			}
			else
			{
				if (tempWorld->canHumanWalk(getX() + 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() + 2, getY()))  // if to the left
				{
					setDirection(right);
					moveTo(getX() + 2, getY());
					return;
				}
			}
		}
		// if citizen not on same column/row
		else
		{
			if (getX() < tempWorld->getPenelope()->getX())
			{
				if (getY() < tempWorld->getPenelope()->getY())
				{
					switch (randInt(1, 2))
					{
					case 1:
						if (tempWorld->canHumanWalk(getX() + 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() + 2, getY()))
						{
							setDirection(right);
							moveTo(getX() + 2, getY());
							return;
						}
						if (tempWorld->canHumanWalk(getX(), getY() + 2, this) && !tempWorld->doesOverlapPenelope(getX(), getY() + 2))
						{
							setDirection(up);
							moveTo(getX(), getY() + 2);
							return;
						}
						break;
					case 2:
						if (tempWorld->canHumanWalk(getX(), getY() + 2, this) && !tempWorld->doesOverlapPenelope(getX(), getY() + 2))
						{
							setDirection(up);
							moveTo(getX(), getY() + 2);
							return;
						}
						if (tempWorld->canHumanWalk(getX() + 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() + 2, getY()))
						{
							setDirection(right);
							moveTo(getX() + 2, getY());
							return;
						}
						break;
					}
				}
				else
				{
					switch (randInt(1, 2))
					{
					case 1:
						if (tempWorld->canHumanWalk(getX() + 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() + 2, getY()))
						{
							setDirection(right);
							moveTo(getX() + 2, getY());
							return;
						}
						if (tempWorld->canHumanWalk(getX(), getY() - 2, this) || !tempWorld->doesOverlapPenelope(getX(), getY() - 2))
						{
							setDirection(down);
							moveTo(getX(), getY() - 2);
							return;
						}
						break;
					case 2:
						if (tempWorld->canHumanWalk(getX(), getY() - 2, this) || !tempWorld->doesOverlapPenelope(getX(), getY() - 2))
						{
							setDirection(down);
							moveTo(getX(), getY() - 2);
							return;
						}
						if (tempWorld->canHumanWalk(getX() + 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() + 2, getY()))
						{
							setDirection(right);
							moveTo(getX() + 2, getY());
							return;
						}
						break;
					}
				}
			}
			else
			{
				if (getY() < tempWorld->getPenelope()->getY())
				{
					switch (randInt(1, 2))
					{
					case 1:
						if (tempWorld->canHumanWalk(getX() - 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() - 2, getY()))
						{
							setDirection(left);
							moveTo(getX() - 2, getY());
							return;
						}
						if (tempWorld->canHumanWalk(getX(), getY() + 2, this) && !tempWorld->doesOverlapPenelope(getX(), getY() + 2))
						{
							setDirection(up);
							moveTo(getX(), getY() + 2);
							return;
						}
						break;
					case 2:
						if (tempWorld->canHumanWalk(getX(), getY() + 2, this) && !tempWorld->doesOverlapPenelope(getX(), getY() + 2))
						{
							setDirection(up);
							moveTo(getX(), getY() + 2);
							return;
						}
						if (tempWorld->canHumanWalk(getX() - 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() - 2, getY()))
						{
							setDirection(left);
							moveTo(getX() - 2, getY());
							return;
						}
						break;
					}
				}
				else
				{
					switch (randInt(1, 2))
					{
					case 1:
						if (tempWorld->canHumanWalk(getX() - 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() - 2, getY()))
						{
							setDirection(left);
							moveTo(getX() - 2, getY());
							return;
						}
						if (tempWorld->canHumanWalk(getX(), getY() - 2, this) && !tempWorld->doesOverlapPenelope(getX(), getY() - 2))
						{
							setDirection(down);
							moveTo(getX(), getY() - 2);
							return;
						}
						break;
					case 2:
						if (tempWorld->canHumanWalk(getX(), getY() - 2, this) && !tempWorld->doesOverlapPenelope(getX(), getY() - 2))
						{
							setDirection(down);
							moveTo(getX(), getY() - 2);
							return;
						}
						if (tempWorld->canHumanWalk(getX() - 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() - 2, getY()))
						{
							setDirection(left);
							moveTo(getX() - 2, getY());
							return;
						}
						break;
					}
				}
			}
		}
	}

	if (dist_z <= 80 && dist_z > 0)
	{
		double dis_up = 0;
		double dis_down = 0;
		double dis_right = 0;
		double dis_left = 0;


		// Calculate new distances to nearest zombie if not blocked by anything
		if (tempWorld->canHumanWalk(getX(), getY() + 2, this) && !tempWorld->doesOverlapPenelope(getX(), getY() + 2))
			dis_up = tempWorld->distanceToNearZombie(getX(), getY() + 2);
		if (tempWorld->canHumanWalk(getX(), getY() - 2, this) && !tempWorld->doesOverlapPenelope(getX(), getY() - 2))
			dis_down = tempWorld->distanceToNearZombie(getX(), getY() - 2);
		if (tempWorld->canHumanWalk(getX() + 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() + 2, getY()))
			dis_right = tempWorld->distanceToNearZombie(getX() + 2, getY());
		if (tempWorld->canHumanWalk(getX() - 2, getY(), this) && !tempWorld->doesOverlapPenelope(getX() - 2, getY()))
			dis_left = tempWorld->distanceToNearZombie(getX() - 2, getY());

		// if every direction brings closer to a zombie, return
		if (tempWorld->distanceToNearZombie(getX(), getY()) > dis_up && tempWorld->distanceToNearZombie(getX(), getY()) > dis_down && tempWorld->distanceToNearZombie(getX(), getY()) > dis_right && tempWorld->distanceToNearZombie(getX(), getY()) > dis_left)
			return;

		// Determine best direction to go and go in that direction
		if (dis_up > dis_down && dis_up > dis_right && dis_up > dis_left && tempWorld->canHumanWalk(getX(), getY() + 2, this))
		{
			setDirection(up);
			moveTo(getX(), getY() + 2);
			return;
		}
		if (dis_down > dis_up && dis_down > dis_right && dis_down > dis_left && tempWorld->canHumanWalk(getX(), getY() - 2, this))
		{
			setDirection(down);
			moveTo(getX(), getY() - 2);
			return;
		}
		if (dis_right > dis_up && dis_right > dis_down && dis_right > dis_left && tempWorld->canHumanWalk(getX() + 2, getY(), this))
		{
			setDirection(right);
			moveTo(getX() + 2, getY());
			return;
		}
		if (dis_left > dis_up && dis_left > dis_down && dis_left > dis_right && tempWorld->canHumanWalk(getX() - 2, getY(), this))
		{
			setDirection(left);
			moveTo(getX() - 2, getY());
			return;
		}
	}
}


Wall::Wall(StudentWorld* s, double column, double row): Actor(s, IID_WALL, column, row, right, 0, 1.0)
{
	setCanBeDamaged(false);
}


Exit::Exit(StudentWorld* s, double column, double row): Actor(s, IID_EXIT, column, row, right, 1, 1.0)
{
	setAbleToWalkOver(true);
	setCanBeDamaged(false);
}

void Exit::doSomething()
{
	StudentWorld* tempWorld = get_sworld();
	Actor* temp = nullptr;
	
	temp = tempWorld->doesOverlapCitizen(getX(), getY());

	if (temp != nullptr)
	{
		tempWorld->increaseScore(500);
		temp->setAliveStatus(false);
		tempWorld->playSound(SOUND_CITIZEN_SAVED);
	}

	if (tempWorld->CitizensGone() && tempWorld->doesOverlapPenelope(getX(), getY()))
		tempWorld->setFinishedState(true);
}


DumbZombie::DumbZombie(StudentWorld* s, double column, double row): Zombie(s, column, row)
{
}

DumbZombie::~DumbZombie()
{
	if (!isAlive())
	{
		Zombie::~Zombie();
		StudentWorld* tempWorld = get_sworld();

		tempWorld->increaseScore(1000);
	}
}

void DumbZombie::doSomething()
{
	if (!isAlive())
		return;

	// paralysis ticks 
	if (isParalyzed())
		return;

	// If the dumb zombie vomited, return
	if (ZombieVomit())
		return;

	
	// Do movement for zombie
	if (getMovePlan() <= 0)
	{
		// set distance
		setMovePlan(randInt(3, 10));

		// set random direction
		setRandomDir();
	}

	// Move Zombie
	moveZombie();
}

void DumbZombie::uponDeath()
{
	StudentWorld* tempWorld = get_sworld();
	// 1 in 10 chance to spawn vaccine
	Vaccine* tempVacc = nullptr;
	switch (randInt(1, 10))
	{
	case 1:
		switch (randInt(1, 4))
		{
		case 1:
			if (!tempWorld->doesOverlapPenelope(getX(), getY() + SPRITE_HEIGHT) && !tempWorld->doesOverlapCitizen(getX(), getY() + SPRITE_HEIGHT) && !tempWorld->doesOverlapWall(getX(), getY() + SPRITE_HEIGHT))
			{
				tempVacc = new Vaccine(tempWorld, getX(), getY() + SPRITE_HEIGHT);
				tempWorld->addActor(tempVacc);
			}
			break;
		case 2:
			if (!tempWorld->doesOverlapPenelope(getX(), getY() - SPRITE_HEIGHT) && !tempWorld->doesOverlapCitizen(getX(), getY() - SPRITE_HEIGHT) && !tempWorld->doesOverlapWall(getX(), getY() - SPRITE_HEIGHT))
			{
				tempVacc = new Vaccine(tempWorld, getX(), getY() - SPRITE_HEIGHT);
				tempWorld->addActor(tempVacc);
			}
			break;
		case 3:
			if (!tempWorld->doesOverlapPenelope(getX() + SPRITE_WIDTH, getY()) && !tempWorld->doesOverlapCitizen(getX() + SPRITE_WIDTH, getY()) && !tempWorld->doesOverlapWall(getX() + SPRITE_WIDTH, getY()))
			{
				tempVacc = new Vaccine(tempWorld, getX() + SPRITE_WIDTH, getY());
				tempWorld->addActor(tempVacc);
			}
			break;
		case 4:
			if (!tempWorld->doesOverlapPenelope(getX() - SPRITE_WIDTH, getY()) && !tempWorld->doesOverlapCitizen(getX() - SPRITE_WIDTH, getY()) && !tempWorld->doesOverlapWall(getX() - SPRITE_WIDTH, getY()))
			{
				tempVacc = new Vaccine(tempWorld, getX() - SPRITE_WIDTH, getY());
				tempWorld->addActor(tempVacc);
			}
			break;
		}
	default:
		break;
	}
}



SmartZombie::SmartZombie(StudentWorld* s, double column, double row): Zombie(s, column, row)
{
}

SmartZombie::~SmartZombie()
{
	if (!isAlive())
	{
		Zombie::~Zombie();
		StudentWorld* tempWorld = get_sworld();

		tempWorld->increaseScore(2000);
	}
}

void SmartZombie::doSomething()
{
	if (!isAlive())
		return;

	if (isParalyzed())
		return;

	if (ZombieVomit())
		return;

	StudentWorld* tempWorld = get_sworld();

	// Smart Zombie movement
	if (getMovePlan() <= 0)
	{
		setMovePlan(randInt(3, 10));

		if (tempWorld->Euclidean_distance(getX(), getY(), tempWorld->nearestHuman(this)->getX(), tempWorld->nearestHuman(this)->getY()) > 80)
			setRandomDir();
		else  // if closer than 80 pixels
		{
			if (getX() == tempWorld->nearestHuman(this)->getX())
			{
				if (getY() > tempWorld->nearestHuman(this)->getY())
					setDirection(down);
				else
					setDirection(up);
			}
			else if (getY() == tempWorld->nearestHuman(this)->getY())
			{
				if (getX() > tempWorld->nearestHuman(this)->getX())
					setDirection(left);
				else
					setDirection(right);
			}
			else
			{
				if (getX() < tempWorld->nearestHuman(this)->getX())
				{
					if (getY() < tempWorld->nearestHuman(this)->getY())
					{
						switch (randInt(1,2))
						{
						case 1:
							setDirection(right);
							break;
						case 2:
							setDirection(up);
						}
					}
					else
					{
						switch (randInt(1, 2))
						{
						case 1: 
							setDirection(right);
							break;
						case 2:
							setDirection(down);
							break;
						}
					}
				}
				else
				{
					if (getY() < tempWorld->nearestHuman(this)->getY())
					{
						switch (randInt(1, 2))
						{
						case 1:
							setDirection(left);
							break;
						case 2:
							setDirection(up);
							break;
						}
					}
					else
					{
						switch (randInt(1, 2))
						{
						case 1:
							setDirection(left);
							break;
						case 2:
							setDirection(down);
							break;
						}
					}
				}
			}
		}
	}

	moveZombie();
}


Vaccine::Vaccine(StudentWorld* s, double column, double row): Goodie(s, IID_VACCINE_GOODIE, column, row)
{	
}

void Vaccine::doSomething()
{
	Goodie::doSomething();

	StudentWorld* temp = get_sworld();
	if (temp->doesOverlapPenelope(getX(), getY()))
		temp->increaseVaccines(1);
}


GasCan::GasCan(StudentWorld* s, double column, double row): Goodie(s, IID_GAS_CAN_GOODIE, column, row)
{
}

void GasCan::doSomething()
{
	Goodie::doSomething();

	StudentWorld* temp = get_sworld();
	if (temp->doesOverlapPenelope(getX(), getY()))
		temp->increaseGas(5);
}


LandmineGoodie::LandmineGoodie(StudentWorld* s, double column, double row): Goodie(s, IID_LANDMINE_GOODIE, column, row)
{
}

void LandmineGoodie::doSomething()
{
	Goodie::doSomething();

	StudentWorld* temp = get_sworld();
	if (temp->doesOverlapPenelope(getX(), getY()))
		temp->increaseLandmines(2);
}


Flame::Flame(StudentWorld* s, int column, int row, Direction dir): Projectile(s, IID_FLAME, column, row, dir)
{
}

void Flame::doSomething()
{
	if (!isAlive())
		return;

	Projectile::doSomething();

	if (!isAlive())
		return;

	Kill();
}


Landmine::Landmine(StudentWorld* s, double column, double row): Actor(s, IID_LANDMINE, column, row, right, 1, 1.0)
{
	Active = false;
	safetyTicks = 30;
	setAbleToWalkOver(true);
}

void Landmine::doSomething()
{
	if (!isAlive())
		return;

	if (!Active)
	{
		safetyTicks--;
		if (safetyTicks == 0)
			Active = true;

		return;
	}

	StudentWorld* tempWorld = get_sworld();
	Actor* tempActor = nullptr;

	// See if Something steps on a landmine
	tempActor = tempWorld->doesOverlapKillables(this);

	if (tempWorld->OverlapKillables(this) || tempWorld->doesOverlapPenelope(getX(), getY()))
	{
		setAliveStatus(false);
		uponDeath();
	}
}

void Landmine::uponDeath()
{
	StudentWorld* tempWorld = get_sworld();
	tempWorld->playSound(SOUND_LANDMINE_EXPLODE);

	Flame* initial = new Flame(tempWorld, getX(), getY(), up);
	tempWorld->addActor(initial);
	
	// check top for wall
	if (!tempWorld->doesOverlapWall(getX(), getY() + SPRITE_HEIGHT))
	{
		Flame* top = new Flame(tempWorld, getX(), getY() + SPRITE_HEIGHT, up);
		tempWorld->addActor(top);
	}

	// check top right for wall
	if (!tempWorld->doesOverlapWall(getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT))
	{
		Flame* top_right = new Flame(tempWorld, getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
		tempWorld->addActor(top_right);
	}

	// check right for wall
	if (!tempWorld->doesOverlapWall(getX() + SPRITE_WIDTH, getY()))
	{
		Flame* right = new Flame(tempWorld, getX() + SPRITE_WIDTH, getY(), up);
		tempWorld->addActor(right);
	}

	//check bottom right for wall
	if (!tempWorld->doesOverlapWall(getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT))
	{
		Flame* bottom_right = new Flame(tempWorld, getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
		tempWorld->addActor(bottom_right);
	}

	// check bottom for wall
	if (!tempWorld->doesOverlapWall(getX(), getY() - SPRITE_HEIGHT))
	{
		Flame* bottom = new Flame(tempWorld, getX(), getY() - SPRITE_HEIGHT, up);
		tempWorld->addActor(bottom);
	}

	// check bottom left
	if (!tempWorld->doesOverlapWall(getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT))
	{

		Flame* bottom_left = new Flame(tempWorld, getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
		tempWorld->addActor(bottom_left);
	}

	// check left for wall
	if (!tempWorld->doesOverlapWall(getX() - SPRITE_WIDTH, getY()))
	{
		Flame* left = new Flame(tempWorld, getX() - SPRITE_WIDTH, getY(), up);
		tempWorld->addActor(left);
	}

	// check top left for wall
	if (!tempWorld->doesOverlapWall(getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT))
	{
		Flame* top_left = new Flame(tempWorld, getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
		tempWorld->addActor(top_left);
	}

	Pit* hole = new Pit(tempWorld, getX(), getY());
	tempWorld->addActor(hole);
}


Pit::Pit(StudentWorld* s, int column, int row): Projectile(s, IID_PIT, column, row, right)
{
	setCanBeDamaged(false);
	setAbleToWalkOver(true);
}

void Pit::doSomething()
{
	Kill();
}


Vomit::Vomit(StudentWorld* s, double column, double row, Direction dir): Projectile(s, IID_VOMIT, column, row, dir)
{
}

void Vomit::doSomething()
{
	if (!isAlive())
		return;

	Projectile::doSomething();

	if (!isAlive())
		return;

	StudentWorld* tempWorld = get_sworld();
	Actor* tempActor = nullptr;

	tempActor = tempWorld->doesOverlapCitizen(getX(), getY());

	if (tempActor != nullptr)
	{
		if (!tempActor->isInfected())
			tempWorld->playSound(SOUND_CITIZEN_INFECTED);
		tempActor->setInfected(true);
	}


	if (tempWorld->doesOverlapPenelope(getX(), getY()))
	{
		tempActor = tempWorld->getPenelope();
		if (!tempActor->isInfected())
			tempWorld->playSound(SOUND_CITIZEN_INFECTED);
		tempActor->setInfected(true);
	}
}







