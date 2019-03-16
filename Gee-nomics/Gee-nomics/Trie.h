#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
#include <iostream>

using namespace std;

template<typename ValueType>
class Trie
{
public:
    Trie()
    {
		root = new Node();
    }

    ~Trie()
    {
		Cleanup(root);
    }

    void reset()
    {
		this->~Trie();
		this = new Trie;
    }

    void insert(const std::string& key, const ValueType& value)
    {
		Node* temp = root;

		for (char letter : key)
		{
			if (temp->children.empty())
			{
				Node* tempNode = new Node;
				tempNode->key = letter;
				temp->children.push_back(tempNode);
				temp = tempNode;
				continue;
			}

			typename vector<Node*>::iterator itr = temp->children.begin();
			while (itr < temp->children.end())
			{
				if ((*itr)->key[0] == letter)
				{
					temp = (*itr);
					break;
				}
				itr++;
				// if no node has the key of the current letter
				if (itr >= temp->children.end())
				{
					Node* tempNode = new Node;
					tempNode->key = letter;
					temp->children.push_back(tempNode);
					temp = tempNode;
					break;
				}
			}
		}

		temp->data.push_back(value);
    }

    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const
    {
		//check to make sure the first  letter in the key matches
		typename vector<Node*>::iterator itr = root->children.begin();
		for (; itr < root->children.end(); itr++)
		{
			if (key[0] == (*itr)->key[0])
				break;
			if (itr == root->children.end() - 1)
				return {};
		}

		return findhelper(key.substr(1), exactMatchOnly, (*itr));
    }

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
	// the node that stores data and vector of pointers to next nodes
	struct Node
	{
		string key;
		vector<ValueType> data;
		vector<Node*> children;
	};

	// helping function for the destructor
	void Cleanup(Node* node)
	{
		if (node != nullptr)
		{
			if (node->children.empty())
			{
				delete node;
				return;
			}

			for (Node* x : node->children)
				Cleanup(x);
		}
	}

	Node* root; // points to root node of the trie

	//helps with the public function find
	vector<ValueType> findhelper(const string& key, bool exactMatchOnly, Node* start) const
	{
		if (exactMatchOnly) // find exact matches only
		{
			Node* temp = start;
			for (char x : key)
			{
				//iterate through every child in the node
				if (temp->children.size() > 0)
				{
					for (typename vector<Node*>::iterator itr = temp->children.begin(); itr < temp->children.end(); itr++)
					{
						if ((*itr)->key[0] == x)
						{
							temp = (*itr);
							break;
						}

						// if last child of node & no match
						if (itr == temp->children.end() - 1)
							return {};
					}
				}
			}
			// if there is a match b/c the loop went all the way to the end
			return temp->data;
		}

		// if not finding exact matches
		vector<ValueType> matches;
		Node* temp = start;
		for (typename vector<Node*>::iterator itr = temp->children.begin(); itr < temp->children.end(); itr++)
		{
			if (key[0] == (*itr)->key[0])
			{
				string tempkey = key;
				int keylength = key.length();
				if (keylength == 1)
				{
					for (ValueType x : (*itr)->data)
						matches.push_back(x);
					continue;
				}
				tempkey.erase(0, 1);
				for (ValueType x : findhelper(tempkey, false, (*itr)))
					matches.push_back(x);
			}
			else
			{
				string tempkey = key;
				tempkey.erase(0, 1);
				for (ValueType x : findhelper(tempkey, true, (*itr)))
					matches.push_back(x);
			}
		}

		return matches;
	}

};

#endif // TRIE_INCLUDED
