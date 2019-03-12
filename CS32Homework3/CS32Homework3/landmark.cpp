/*class Landmark
{
public:
	Landmark(string name)
	{
		landmark_name = name;
	}
	virtual ~Landmark() = 0;

	string name() const
	{
		return landmark_name;
	}
	virtual string color() const
	{
		return "yellow";
	}
	virtual string icon() const = 0;

private:
	string landmark_name;
};

Landmark::~Landmark() {}


class Hotel : public Landmark
{
public:
	Hotel(string name) : Landmark(name)
	{
	}
	virtual ~Hotel()
	{
		cout << "Destroying the hotel " << name() << "." << endl;
	}
	virtual string icon() const
	{
		return "bed";
	}
};

class Restaurant : public Landmark
{
public:
	Restaurant(string name, int cap) : Landmark(name)
	{
		seating_cap = cap;
	}
	virtual ~Restaurant()
	{
		cout << "Destroying the restaurant " << name() << "." << endl;
	}
	virtual string icon() const
	{
		if (seating_cap < 40)
			return "small knife/fork";
		return "large knife/fork";
	}

private:
	int seating_cap;
};

class Hospital : public Landmark
{
public:
	Hospital(string name) : Landmark(name)
	{

	}
	virtual ~Hospital()
	{
		cout << "Destroying the hospital " << name() << "." << endl;
	}
	virtual string color() const
	{
		return "blue";
	}
	virtual string icon() const
	{
		return "H";
	}
};

void display(const Landmark* lm)
{
    cout << "Display a " << lm->color() << " " << lm->icon() << " icon for "
	 << lm->name() << "." << endl;
}*/