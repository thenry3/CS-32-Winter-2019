#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Trie.h"
#include <map>
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	int minLength;
	vector<Genome> GenomeLibrary;
	Trie<string> GenomeTrie;

	bool fragmentCheck(bool status, string frag1, string frag2) const;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
	minLength = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	GenomeLibrary.push_back(genome); // add genome to library

	int pos = 0; // position of substring
	string subsequence; // store the sub string with this
	while (genome.extract(pos, minimumSearchLength(), subsequence))
	{
		string str = genome.name() + ", position " + to_string(pos);
		GenomeTrie.insert(subsequence, str);
		subsequence = "";
		pos++;
	}
}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return minLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	//check if meets length requirements
	int fraglength = fragment.length();
	if (fraglength < minimumLength || minimumLength < minimumSearchLength())
		return false;

	//get the fragment from the fragment for min search length
	string minfragment = fragment.substr(0, minimumSearchLength());

	string genomefragment;

	for (string x : GenomeTrie.find(minfragment, exactMatchOnly))
	{
		//extract genome name and position from the trie
		int endstringindex = x.find(", position ");
		string getname = x;
		getname.erase(endstringindex);
		string posstring = x.substr(endstringindex + 11);
		int pos = stoi(posstring);

		for (Genome genome : GenomeLibrary)
		{
			if (genome.name() == getname)
			{
				int searchlength = minimumLength;
				string compare;

				//check if potential matches meet the min length req parameter
				if (genome.extract(pos, minimumLength, genomefragment))
				{
					compare = fragment.substr(0, minimumLength);
					if (!fragmentCheck(exactMatchOnly, compare, genomefragment))
						break;
				}

				//get the length of the fragment that matches
				while (genome.extract(pos, searchlength, genomefragment))
				{
					compare = fragment.substr(0, searchlength);
					if (!fragmentCheck(exactMatchOnly, compare, genomefragment))
					{
						searchlength--;
						break;
					}
					if (fragmentCheck(exactMatchOnly, compare, genomefragment))
						searchlength++;
				}
				DNAMatch match;
				match.genomeName = getname;
				match.length = searchlength;
				match.position = pos;

				//for genomes that repeat twice, compare the two fragments and see 
				//which one is longer and if same length, which one came first
				if (!matches.empty())
				{
					vector<DNAMatch>::iterator itr = matches.begin();
					while (itr < matches.end())
					{
						if (match.genomeName == (*itr).genomeName)
						{
							if (match.length > (*itr).length)
							{
								matches.erase(itr);
								itr = matches.begin();
								matches.push_back(match);
								itr = matches.begin();
								break;
							}
							if (match.length < (*itr).length)
							{
								break;
							}

							if (match.position < (*itr).position)
							{
								matches.erase(itr);
								itr = matches.begin();
								matches.push_back(match);
								itr = matches.begin();
								break;
							}

							break;
						}
						itr++;
					}
					if (itr == matches.end())
						matches.push_back(match);
				}
				else
					matches.push_back(match);
			}
		}
	}

	if (matches.empty())
		return false;
	return true;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	//check if fragment match length is at least equal to the min search length
	if (fragmentMatchLength < minimumSearchLength())
		return false;

	// extract sequences from query 
	int numOfSequences = query.length() / fragmentMatchLength;
	string querySequence;
	int i = 0;

	//for each fragment from the query find matches and count the number of matches for each query fragment
	map<string, int> GenomeTrack;
	while(query.extract(i * fragmentMatchLength, fragmentMatchLength, querySequence))
	{
		vector<DNAMatch> matches;
		if (findGenomesWithThisDNA(querySequence, fragmentMatchLength, exactMatchOnly, matches))
		{
			for (DNAMatch Match : matches)
			{
				if (GenomeTrack.count(Match.genomeName) == 0)
					GenomeTrack[Match.genomeName] = 1;
				else
					GenomeTrack[Match.genomeName]++;
			}
		}
		i++;
	}

	//for each genome that has a match, calculate the % of frags matched
	for (map<string, int>::iterator itr = GenomeTrack.begin(); itr != GenomeTrack.end(); itr++)
	{
		double p = (static_cast<double>((*itr).second) / static_cast<double>(numOfSequences)) * 100;
		if (p >= matchPercentThreshold)
		{
			GenomeMatch temp;
			temp.genomeName = (*itr).first;
			temp.percentMatch = p;

			results.push_back(temp);
		}
	}

	// if there's nothing in results, nothing met the % threshold
	if (results.empty())
		return false;

	return true;
}

bool GenomeMatcherImpl::fragmentCheck(bool status, string frag1, string frag2) const
{
	int size = frag1.size();
	int size1 = frag2.size();
	if (size != size1)
		return false;

	if (status)
	{
		for (int i = 0; i < size; i++)
		{
			if (frag1[i] != frag2[i])
				return false;
		}
		return true;
	}
	if (size == 1)
		return true;
	if (frag1[0] == frag2[0])
	{
		return fragmentCheck(false, frag1.substr(1), frag2.substr(1));
	}
	return fragmentCheck(true, frag1.substr(1), frag2.substr(1));

}


//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
