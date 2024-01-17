#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iosfwd>
#include <iostream>
#include <algorithm>
#include <fstream>

class DeterministicFiniteAutomaton
{
public:
	DeterministicFiniteAutomaton();
	DeterministicFiniteAutomaton(std::vector<char> states, std::vector<char> alphabet,
		std::unordered_map<char, std::unordered_map<char, char>> delta, char q0, std::vector<char> finalStates);

	bool VerifyAutomaton() const;
	bool CheckWord(std::string w) const;


	friend std::istream& operator>>(std::istream& in, DeterministicFiniteAutomaton& g);
	//PrintAutomaton
	friend std::ostream& operator<<(std::ostream& out, DeterministicFiniteAutomaton& g);


	void addState(char state);
	void addSigma(char letter);
	std::vector<char> getSigma();

	void addToDelta(char q, char a, char p);

	void setQ0(char q0);

	void addFinalState(char c);

	std::vector<char> getFinalState();

	void deleteSelectedFinalState(char c);


private:
	//Q
	std::vector<char> m_states;
	//SIGMA
	std::vector<char> m_alphabet;
	//delta - functia de tranzitie
	std::unordered_map<char, std::unordered_map<char, char>> m_delta;
	//Q0
	char m_q0;
	//Fs
	std::vector<char> m_finalStates;
};