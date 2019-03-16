#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
	string genomeName;
	string genomeDNA;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
	genomeName = nm;
	genomeDNA = sequence;
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
	string line;
	string name;
	string sequence;

	//check if the first line of the file is a '>'
	getline(genomeSource, line);
	if (line[0] == '>')
		name += line.substr(1);
	else
		return false;

	while (getline(genomeSource, line))
	{
		//check for empty lines
		if (line.empty())
			return false;

		bool didnewGenomeappear = false;
		for (char character : line)
		{
			// Cases for each character
			switch (character)
			{
			case '>':
				didnewGenomeappear = true;
				genomes.emplace_back(Genome(name, sequence));
				name = "";
				sequence = "";
				break;
			case 'G':
			case 'g':
			case 'A':
			case 'a':
			case 'T':
			case 't':
			case 'N':
			case 'n':
			case 'C':
			case 'c':
				if (didnewGenomeappear)
					name += character;
				else
					sequence += character;
				break;
			default:
				if (didnewGenomeappear)
					name += character;
				else
					return false;
				break;
			}
		}
	}
	genomes.emplace_back(Genome(name, sequence));
	return true;
}

int GenomeImpl::length() const
{
	return genomeDNA.length();
}

string GenomeImpl::name() const
{
	return genomeName;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	// test if length goes out of bounds
	int DNAlength = genomeDNA.length();
	if (position + length > DNAlength)
		return false;

	if (!fragment.empty())
		fragment = "";

	//add each character until fragment reaches the length
	for (int i = position; i < length + position; i++)
		fragment += genomeDNA[i];

	return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
	m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
	delete m_impl;
}

Genome::Genome(const Genome& other)
{
	m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
	GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
	delete m_impl;
	m_impl = newImpl;
	return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
	return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
	return m_impl->length();
}

string Genome::name() const
{
	return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
	return m_impl->extract(position, length, fragment);
}
