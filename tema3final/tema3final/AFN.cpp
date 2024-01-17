#include "AFN.h"

FiniteAutomaton::FiniteAutomaton()
{
}

void removeElement(std::vector<char>& charVector, char elementToRemove)
{
	// Use std::remove to move the elements to be removed to the end of the vector
	auto removeIt = std::remove(charVector.begin(), charVector.end(), elementToRemove);

	// Erase the elements that were moved to the end
	charVector.erase(removeIt, charVector.end());
}

FiniteAutomaton::FiniteAutomaton(std::vector<char> states, std::vector<char> alphabet, std::unordered_map<char, std::unordered_map<char, std::unordered_set<char>>> delta, char q0, std::vector<char> finalStates)
	:m_q0{ q0 }
{
	m_states = states;
	m_alphabet = alphabet;
	m_delta = delta;
	m_finalStates = finalStates;

}

bool FiniteAutomaton::VerifyAutomaton() const
{
	if (m_states.size() == 0)return false;
	if (m_alphabet.size() == 0)return false;
	auto it = std::find(m_states.begin(), m_states.end(), m_q0);
	if (it == m_states.end())return false;
	if (m_finalStates.size() == 0)return false;
	for (int i = 0; i < m_finalStates.size(); i++)
	{
		auto it = std::find(m_states.begin(), m_states.end(), m_finalStates[i]);
		if (it == m_states.end())return false;
	}
	return true;
}

bool FiniteAutomaton::CheckWord(std::string w) const
{
	int ok = 1;
	std::vector<char> currentStates;
	std::vector<char> auxCurrentStates;
	currentStates.push_back(m_q0);
	while (w.length() > 0 && ok == 1)
	{
		ok = 0;
		for (int i = 0; i < currentStates.size(); i++)
		{
			for (auto& outerPair : m_delta)
			{
				char outerKey = outerPair.first;
				auto& innerMap = outerPair.second;

				for (auto& innerPair : innerMap)
				{
					char innerKey = innerPair.first;
					auto& set = innerPair.second;
					if (currentStates[i] == outerKey && innerKey == w[0])
					{
						ok = 1;
						for (auto& it : set)
						{
							auxCurrentStates.push_back(it);
						}
					}
				}
			}
		}
		if (ok == 1)w.erase(0, 1);
		currentStates = auxCurrentStates;
		auxCurrentStates.clear();
	}
	if (w.length() != 0)return false;
	for (int i = 0; i < currentStates.size(); i++)
	{
		auto it = std::find(m_finalStates.begin(), m_finalStates.end(), currentStates[i]);
		if (it != m_finalStates.end())return true;
	}
	return false;
}

bool FiniteAutomaton::IsDeterministic() const
{
	for (auto& outerPair : m_delta)
	{
		char outerKey = outerPair.first;
		auto& innerMap = outerPair.second;

		for (auto& innerPair : innerMap)
		{
			char innerKey = innerPair.first;
			auto& set = innerPair.second;
			if (set.size() > 1)return false;

		}
	}
	return true;
}

void FiniteAutomaton::addState(char state)
{
	m_states.push_back(state);
}

void FiniteAutomaton::addSigma(char letter)
{
	m_alphabet.push_back(letter);
}

void FiniteAutomaton::addFinalState(char state)
{
	m_finalStates.push_back(state);
}

void FiniteAutomaton::addToDelta(char q, char a, char p)
{
	m_delta[q][a].insert(p);
}

void FiniteAutomaton::setInitialState(char q0)
{
	m_q0 = q0;
}

std::vector<char> FiniteAutomaton::getStates()
{
	return m_states;
}

std::vector<char> FiniteAutomaton::getAlphabet()
{
	return m_alphabet;
}

std::unordered_map<char, std::unordered_map<char, std::unordered_set<char>>> FiniteAutomaton::getDelta()
{
	return m_delta;
}

char FiniteAutomaton::getInitialState()
{
	return m_q0;
}

std::vector<char> FiniteAutomaton::getFinalStates()
{
	return m_finalStates;
}

void FiniteAutomaton::modifyFinalState(char c)
{
	m_finalStates[0] = c;
}

void FiniteAutomaton::deleteSelectedfinalState(char c)
{
	removeElement(m_finalStates, c);
}

DeterministicFiniteAutomaton FiniteAutomaton::makeAFDFromLambda()
{

	int lambdaCounter = 0;
	std::unordered_map<char, std::set<char>> lambdaCloses;
	std::unordered_map<char, std::unordered_map<char, char>> newDelta;
	std::set<char> currentStates;
	std::set<char> currentLambdaStates;
	std::stack<char> lambdaStack;
	char stackTop;
	//algoritmul de lambda inchidere
	lambdaStack.push(m_q0);
	while (!lambdaStack.empty())
	{
		stackTop = lambdaStack.top();
		lambdaStack.pop();
		for (auto& outerPair : m_delta)
		{
			char outerKey = outerPair.first;
			auto& innerMap = outerPair.second;

			for (auto& innerPair : innerMap)
			{
				char innerKey = innerPair.first;
				auto& set = innerPair.second;
				for (auto& element : set)
					if (outerKey == stackTop && innerKey == '^')lambdaStack.push(element);
			}
		}
		currentLambdaStates.insert(stackTop);
	}
	//
	//introduc noua stare in multimea de stari noi
	lambdaCloses[char('0' + lambdaCounter)] = currentLambdaStates;

	//tin minte starea initiala

	char q0 = char('0' + lambdaCounter);
	//maresc lambdaCounter
	lambdaCounter++;
	//resetez currentLambdaStates
	currentLambdaStates.clear();

	//acum incep de la al doilea rand din indicatii, practic generez noua functie lambda

	//iterez prin noile stari
	std::sort(m_alphabet.begin(), m_alphabet.end());
	for (auto& newState : lambdaCloses)
	{
		//iterez prin fiecare symbol din alfabet
		for (auto& symbol : m_alphabet)
		{
			//iterez prin fiecare stare salvata in noile stari
			//de asemenea eliberez currentStates in care retin starile unde pot ajunge din noua stare cu symbol
			currentStates.clear();
			for (auto& storedState : newState.second)
			{
				for (auto& outerPair : m_delta)
				{
					char outerKey = outerPair.first;
					auto& innerMap = outerPair.second;

					for (auto& innerPair : innerMap)
					{
						char innerKey = innerPair.first;
						auto& set = innerPair.second;
						for (auto& element : set)
							if (outerKey == storedState && innerKey == symbol)currentStates.insert(element);
					}
				}
			}
			//acyn ca am multimea de currentStates, le lambda inchid
			//resetez currentLambdaStates
			currentLambdaStates.clear();
			for (auto& currentState : currentStates)
			{
				lambdaStack.push(currentState);
				while (!lambdaStack.empty())
				{
					stackTop = lambdaStack.top();
					lambdaStack.pop();
					for (auto& outerPair : m_delta)
					{
						char outerKey = outerPair.first;
						auto& innerMap = outerPair.second;

						for (auto& innerPair : innerMap)
						{
							char innerKey = innerPair.first;
							auto& set = innerPair.second;
							for (auto& element : set)
								if (outerKey == stackTop && innerKey == '^')lambdaStack.push(element);
						}
					}
					currentLambdaStates.insert(stackTop);
				}
			}


			//verific daca lambda inchiderea este goala

			if (currentLambdaStates.size() == 0)
			{
				newDelta[newState.first][symbol] = '@';
				//@ inseamna multimea vida	
			}
			else
			{
				//acum ca am lambda inchiderea, verific daca deja mai exista aceasta lambdainchidere
				int ok = 0;
				for (auto& analizedLambda : lambdaCloses)
				{
					if (analizedLambda.second == currentLambdaStates)
					{
						//caz in care mai exista deja
						newDelta[newState.first][symbol] = analizedLambda.first;
						ok = 1;
						break;
					}
				}
				if (ok == 0)
				{
					//caz in care nu mai exista
					lambdaCloses[char('0' + lambdaCounter)] = currentLambdaStates;
					//o adaug
					//fac o noua lege de tranzitie
					newDelta[newState.first][symbol] = char('0' + lambdaCounter);
					//maresc lambdaCounter
					lambdaCounter++;
				}
			}


		}
	}



	//verificare stari noi
	/*for (auto& elMap : lambdaCloses)
	{
		std::cout << elMap.first << '-';
		for (auto& elSet : elMap.second)
			std::cout << elSet << ' ';
		std::cout << std::endl;
	}*/

	//verificare noul delta
	//for (const auto& outerPair : newDelta) {
	//	char outerKey = outerPair.first;
	//	const auto& innerMap = outerPair.second;

	//	for (const auto& innerPair : innerMap) {
	//		char innerKey = innerPair.first;
	//		char value = innerPair.second;

	//		// Now you can access each individual element
	//		std::cout << "Outer Key: " << outerKey << ", Inner Key: " << innerKey
	//			<< ", Value: " << value << std::endl;
	//	}
	//}

	//pregatesc elementele noului automat determinist
	std::vector<char> states;
	for (auto& elMap : lambdaCloses)
	{
		states.push_back(elMap.first);
	}
	//alfabetul este acelasi
	//noul delta l-am pregatit mai sus
	//si q0 a fost pregatit tot mai sus
	//pregatesc starea finala
	std::vector<char> finalStates;
	for (auto& elMap : lambdaCloses)
	{

		for (auto& elSet : elMap.second)
		{
			auto it = std::find(m_finalStates.begin(), m_finalStates.end(), elSet);
			if (it != m_finalStates.end())finalStates.push_back(elMap.first);
		}
	}

	// toate elementele sunt pregatite, produc automatul finit determinist
	DeterministicFiniteAutomaton automatDeterminist(states, m_alphabet, newDelta, q0, finalStates);
	return automatDeterminist;
}

std::istream& operator>>(std::istream& in, FiniteAutomaton& g)
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
		g.m_delta[q][a].insert(p);
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

std::ostream& operator<<(std::ostream& out, FiniteAutomaton& g)
{
	out << "Fie Automatomatul Nedeterminist M = ({ ";
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
			auto& set = innerPair.second;
			for (auto& it : set)
			{
				out << "(" << outerKey << "," << innerKey << ") = " << it;
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	return out;
}
