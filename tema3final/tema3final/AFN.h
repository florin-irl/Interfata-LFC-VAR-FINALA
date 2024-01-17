#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <iosfwd>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stack>
#include "AFD.h"
class FiniteAutomaton
{
public:
	FiniteAutomaton();

	FiniteAutomaton(std::vector<char> states, std::vector<char> alphabet,
		std::unordered_map<char, std::unordered_map<char, std::unordered_set<char>>> delta
		, char q0, std::vector<char> finalStates);

	bool VerifyAutomaton() const;

	bool CheckWord(std::string w) const;

	bool IsDeterministic() const;

	void addState(char state);

	void addSigma(char letter);

	void addFinalState(char state);

	void addToDelta(char q, char a, char p);

	void setInitialState(char q0);

	std::vector<char> getStates();
	std::vector<char> getAlphabet();
	std::unordered_map<char, std::unordered_map<char, std::unordered_set<char>>> getDelta();
	char getInitialState();
	std::vector<char> getFinalStates();
	void modifyFinalState(char c);

	void deleteSelectedfinalState(char c);

	DeterministicFiniteAutomaton makeAFDFromLambda();

	friend std::istream& operator>>(std::istream& in, FiniteAutomaton& g);
	//PrintAutomaton
	friend std::ostream& operator<<(std::ostream& out, FiniteAutomaton& g);

private:
	//Q
	std::vector<char> m_states;
	//Sigma
	std::vector<char> m_alphabet;
	//delta
	std::unordered_map<char, std::unordered_map<char, std::unordered_set<char>>> m_delta;
	//q0
	char m_q0;
	//Fs
	std::vector<char> m_finalStates;
};

