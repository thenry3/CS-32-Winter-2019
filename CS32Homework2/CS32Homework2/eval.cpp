#include "Set.h"  // with ItemType being a type alias for char
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

using namespace std;

string in_to_post(string infix);
bool solve_post(string post, const Set& trues, const Set& falses);
bool is_valid(string infix);

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
	if (!is_valid(infix))
		return 1;

	postfix = in_to_post(infix);

	for (char x: postfix)
	{
		if (x != '!' && x != '&' && x != '|' && x != ' ')
		{
			if (!trueValues.contains(x) && !falseValues.contains(x))
				return 2;
			if (trueValues.contains(x) && falseValues.contains(x))
				return 3;
		}
	}

	result = solve_post(postfix, trueValues, falseValues);

	return 0;
}

bool is_valid(string infix)
{
	if (infix.empty())
		return false;

	string infix_nospaces;
	for (char x : infix)
	{
		if (x != ' ')
			infix_nospaces += x;
	}
	int size = infix_nospaces.size();

	for (int i = 0; i < size; i++)
	{
		if (isupper(infix_nospaces[i]) && infix_nospaces[i] != '!' && infix_nospaces[i] != '&' && infix_nospaces[i] != '|' && infix_nospaces[i] != '(' && infix_nospaces[i] != ')')
			return false;
		if (i == size - 1)
		{
			if (infix_nospaces[i] == '!' || infix_nospaces[i] == '&' || infix_nospaces[i] == '|')
			{
				return false;
			}
		}
	}

	int count_parenthesis_open = 0;
	int count_parenthesis_close = 0;
	for (char x: infix_nospaces)
	{
		if (x == '(')
			count_parenthesis_open++;
		if (x == ')')
			count_parenthesis_close++;
	}
	if (count_parenthesis_open != count_parenthesis_close)
		return false;

	int count_char = 0;
	int count_operand = 0;
	for (char x : infix_nospaces)
	{
		if (x != '!' && x != '&' && x != '|' && x != '(' && x != ')')
			count_char++;
		if (x == '!' || x == '&' || x == '|')
			count_operand++;
	}
	if (count_operand < count_char - 1)
		return false;

	if (count_operand == 0 && count_char == 1)
		return true;

	for (int i = 0; i < size; i++)
	{
		if (infix_nospaces[i] == '(')
		{
			int a = i + 1;
			string temp;
			while (infix_nospaces[a] != ')')
			{
				temp += infix_nospaces[a];
				a++;
			}
			if (!is_valid(temp))
				return false;
		}
		if (infix_nospaces[i] != '!' && infix_nospaces[i] != '&' && infix_nospaces[i] != '|' && infix_nospaces[i] != '(' && infix_nospaces[i] != ')')
		{
			if (i == 1)
			{
				if (infix_nospaces[i - 1] != '!')
					return false;
			}
			else if (i != 0)
			{
				if (infix_nospaces[i - 1] != '!' && infix_nospaces[i - 1] != '&' && infix_nospaces[i - 1] != '|' && infix_nospaces[i - 1] != '(')
					return false;
			}
				
		}
	}

	return true;
}


string in_to_post(string infix)
{
	string output;
	stack<char> operator_stack;

	for (char n: infix)
	{
		switch (n)
		{
		case '(':
			operator_stack.push(n);
			break;
		case '&':
			if (operator_stack.empty())
				operator_stack.push(n);
			else
			{
				while (operator_stack.top() == '!' || operator_stack.top() == '&')
				{
					char temp = operator_stack.top();
					operator_stack.pop();
					output += temp;
				}
				operator_stack.push(n);
			}
			break;
		case '!':
			if (operator_stack.empty())
				operator_stack.push(n);
			else
			{
				while (operator_stack.top() == '!')
				{
					char temp = operator_stack.top();
					operator_stack.pop();
					output += temp;
				}
				operator_stack.push(n);
			}
			break;
		case '|':
			if (operator_stack.empty())
				operator_stack.push(n);
			else
			{
				while (operator_stack.top() == '|' || operator_stack.top() == '!' || operator_stack.top() == '&')
				{
					char temp = operator_stack.top();
					operator_stack.pop();
					output += temp;
					if (operator_stack.empty())
						break;
				}
				operator_stack.push(n);
			}
			break;
		case ')':
			while (!operator_stack.empty())
			{
				if (operator_stack.top() == '(')
				{
					operator_stack.pop();
					break;
				}
				char temp = operator_stack.top();
				operator_stack.pop();
				output += temp;
			}
		case ' ':
			break;
		default:
			output += n;
			break;
		}
	}

	while (!operator_stack.empty())
	{
		char temp = operator_stack.top();
		operator_stack.pop();
		output += temp;
	}

	return output;
}

bool solve_post(string post, const Set& trues, const Set& falses)
{
	stack<bool> bool_stack;
	bool a = true;
	bool b = false;
	bool v1;
	bool v2;
	for (char x: post)
	{
		if (x != '!' && x != '&' && x != '|')
		{
			if (trues.contains(x))
				bool_stack.push(a);
			if (falses.contains(x))
				bool_stack.push(b);
		}
			
		else
		{
			switch (x)
			{
			case '!':
				v1 = bool_stack.top();
				bool_stack.pop();

				if (!v1)
					bool_stack.push(a);
				else
					bool_stack.push(b);
				break;
			case '&':
				v2 = bool_stack.top();
				bool_stack.pop();
				v1 = bool_stack.top();
				bool_stack.pop();

				if (v1 & v2)
					bool_stack.push(a);
				else
					bool_stack.push(b);
				break;
			case '|':
				v2 = bool_stack.top();
				bool_stack.pop();
				v1 = bool_stack.top();
				bool_stack.pop();

				if (v2 | v1)
					bool_stack.push(a);
				else
					bool_stack.push(b);
				break;
			}
		}

	}

	return bool_stack.top();
	
}


int main()
{
	/*string trueChars = "tywz";
	string falseChars = "fnx";*/

	string trueChars = "aeiou";
	string falseChars = "bgkp";
	Set trues;
	Set falses;

	int x = trueChars.size();
	int y = falseChars.size();
	for (int k = 0; k < x; k++)
		trues.insert(trueChars[k]);
	for (int k = 0; k < y; k++)
		falses.insert(falseChars[k]);

	string pf;
	bool answer;

	cout << evaluate("(i&((b|i)&(i)))", trues, falses, pf, answer) << endl;
	cout << in_to_post("(i&((b|i)&(i)))") << endl;
	cout << answer << endl;
	/*assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" &&  answer);
	assert(evaluate("y|", trues, falses, pf, answer) == 1);
	assert(evaluate("n t", trues, falses, pf, answer) == 1);
	assert(evaluate("nt", trues, falses, pf, answer) == 1);
	assert(evaluate("()", trues, falses, pf, answer) == 1);
	assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
	assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
	assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
	assert(evaluate("", trues, falses, pf, answer) == 1);
	assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
		&& pf == "ff!tn&&|" && !answer);
	assert(evaluate(" x  ", trues, falses, pf, answer) == 0 && pf == "x" && !answer);
	trues.insert('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 3);
	falses.erase('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 0 && pf == "x"  &&  answer);
	trues.erase('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 2);
	falses.insert('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" && !answer);*/
	cout << "Passed all tests" << endl;
}
