#include "AFD.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton()
	:m_q0{ '@' }
{
}

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::vector<char> states, std::vector<char> alphabet,
	std::unordered_map<char, std::unordered_map<char, char>> delta, char q0, std::vector<char> finalStates)
	:m_q0{ q0 }
{
	m_states = states;
	m_alphabet = alphabet;
	m_delta = delta;
	m_finalStates = finalStates;
}

void removeElement1(std::vector<char>& charVector, char elementToRemove)
{
	// Use std::remove to move the elements to be removed to the end of the vector
	auto removeIt = std::remove(charVector.begin(), charVector.end(), elementToRemove);

	// Erase the elements that were moved to the end
	charVector.erase(removeIt, charVector.end());
}

bool DeterministicFiniteAutomaton::VerifyAutomaton() const
{
	//CHECK IF M_STATES HAS STATES
	if (m_states.size() == 0)return false;
	//CHECK IF M_ALPHABET HAS SYMBOLS
	if (m_alphabet.size() == 0)return false;
	//CHECK IF Q0 IS IN STATES
	auto it = std::find(m_states.begin(), m_states.end(), m_q0);
	if (it == m_states.end())return false;
	//CHECK IF FINAL STATES HAS STATES AND THAT ALL STATES ARE INCLUDED IN M_STATES
	if (m_finalStates.size() == 0)return false;
	for (int i = 0; i < m_finalStates.size(); i++)
	{
		auto it = std::find(m_states.begin(), m_states.end(), m_finalStates[i]);
		if (it == m_states.end())return false;
	}
	//CHECK IF DELTA (functia de tranzitie) IS VALID
	for (auto d = m_delta.begin(); d != m_delta.end(); ++d)
	{
		if (std::find(m_states.begin(), m_states.end(), d->first) == m_states.end())return false;
		for (auto D = d->second.begin(); D != d->second.end(); ++D)
		{
			if (std::find(m_alphabet.begin(), m_alphabet.end(), D->first) == m_alphabet.end())return false;
			if (std::find(m_states.begin(), m_states.end(), D->second) == m_states.end())return false;
		}

	}
	return true;

	//functia de tranzitie functioneaza doar pe comportamentul unui AFD, astfel ca nu este nevoie sa se verifice
	//daca dintr-o stare, cu acelasi simbol, se poate ajunge in mai multe stari diferite
}

bool DeterministicFiniteAutomaton::CheckWord(std::string w) const
{
	int cnt = 0;
	int ok = 1;
	char currentState = m_q0;
	while (w.length() > 0 && ok)
	{
		ok = 0;
		for (const auto& outerPair : m_delta) {
			for (const auto& innerPair : outerPair.second) {
				if (outerPair.first == currentState && innerPair.first == w[0])
				{
					ok = 1;
					currentState = innerPair.second;
					break;
				}
			}
			if (ok)break;
		}
		if (ok == 1)w.erase(0, 1);
	}
	if (w.length() != 0)return false;
	auto it = std::find(m_finalStates.begin(), m_finalStates.end(), currentState);
	if (it != m_finalStates.end())return true;

	return false;
}

void DeterministicFiniteAutomaton::addState(char state)
{
	m_states.push_back(state);
}

void DeterministicFiniteAutomaton::addSigma(char letter)
{
	m_alphabet.push_back(letter);
}

std::vector<char> DeterministicFiniteAutomaton::getSigma()
{
	return m_alphabet;
}

void DeterministicFiniteAutomaton::addToDelta(char q, char a, char p)
{
	m_delta[q][a] = p;
}

void DeterministicFiniteAutomaton::setQ0(char q0)
{
	m_q0 = q0;
}

void DeterministicFiniteAutomaton::addFinalState(char c)
{
	m_finalStates.push_back(c);
}

std::vector<char> DeterministicFiniteAutomaton::getFinalState()
{
	return m_finalStates;
}

void DeterministicFiniteAutomaton::deleteSelectedFinalState(char c)
{
	removeElement1(m_finalStates, c);
}

std::istream& operator>>(std::istream& in, DeterministicFiniteAutomaton& g)
{
	int sizeQ, sizeSigma, sizeDelta, sizeF;
	char c;
	char q, a, p;
	in >> sizeQ;
	for (int i = 0; i < sizeQ; i++)
	{
		in >> c;
		g.m_states.push_back(c);
	}
	in >> sizeSigma;
	for (int i = 0; i < sizeSigma; i++)
	{
		in >> c;
		g.m_alphabet.push_back(c);
	}
	in >> sizeDelta;
	for (int i = 0; i < sizeDelta; i++)
	{
		in >> q >> a >> p;
		g.m_delta[q][a] = p;
	}
	in >> c;
	g.m_q0 = c;
	in >> sizeF;
	for (int i = 0; i < sizeF; i++)
	{
		in >> c;
		g.m_finalStates.push_back(c);
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, DeterministicFiniteAutomaton& g)
{
	out << "Fie Automatomatul Determinist M = ({ ";
	for (int i = 0; i < g.m_states.size(); i++)
	{
		out << g.m_states[i] << ' ';
	}
	out << " },{ ";
	for (int i = 0; i < g.m_alphabet.size(); i++)
	{
		out << g.m_alphabet[i] << ' ';
	}
	out << " }, DELTA, ";
	out << g.m_q0 << ", { ";
	for (int i = 0; i < g.m_finalStates.size(); i++)
	{
		out << g.m_finalStates[i] << ' ';
	}
	out << " })" << std::endl;
	out << "Cu DELTA fiind: " << std::endl;
	for (auto& outerPair : g.m_delta)
	{
		char outerKey = outerPair.first;
		auto& innerMap = outerPair.second;

		for (auto& innerPair : innerMap)
		{
			char innerKey = innerPair.first;
			char innerValue = innerPair.second;
			out << "(" << outerKey << "," << innerKey << ") = " << innerValue << std::endl;
		}

	}
	return out;
}
