//
// COMP345_PROJECT_GAMEENGINE_H GameEngine.h
//

#ifndef GameEngine_H
#define GameEngine_H

#include <string>
    using std::string;
#include <vector>
    using std::vector;
#include <ostream>
    using std::ostream;


// ---------------------------------------------
// ------------ Transition Section -------------
// ---------------------------------------------

class Transition
{
public:
	Transition(string currentState, string command, int nextStateIndex);
	Transition(const Transition&);
	Transition& operator=(const Transition&);

	string getCurrentState();
	string getCommand();
	int getNextStateIndex();


private:
	string _currentState;
	string _command;
	int _nextStateIndex;
	friend ostream& operator<<(ostream&, const Transition&);
};


// ---------------------------------------------
// --------------- State Section ---------------
// ---------------------------------------------

class State
{
public:
	State(string name, vector<Transition*> transition);
	State(const State&);
	State& operator=(const State&);

	string getName();
	vector<Transition*> getTransition();

private:
	string _name;
	vector<Transition*> _transition;
	friend ostream& operator<<(ostream&, const State&);
};


#endif
