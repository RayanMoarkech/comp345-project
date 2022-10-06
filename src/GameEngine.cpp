#include <string>
#include <vector>
#include <ostream>
#include <ostream>
#include "../include/GameEngine.h"
using namespace std;


using namespace std;

Transition::Transition(string currentState, string command, int nextStateIndex) {
	_currentState = currentState;
	_command = command;
	_nextStateIndex = nextStateIndex;

}
Transition::Transition(const Transition &transition)
{
	_currentState = transition._currentState;
	_command = transition._command;
	_nextStateIndex = transition._nextStateIndex;
}
Transition& Transition::operator=(const Transition & transition)
{
	_currentState = transition._currentState;
	_command = transition._command;
	_nextStateIndex = transition._nextStateIndex;
	return *this;
}
;

string Transition::getCurrentState() {
	return _currentState;
};

int Transition::getNextStateIndex() {
	return _nextStateIndex;
};

string Transition::getCommand() {
	return _command;
}

ostream& operator<<(ostream &strm, const Transition &transition) {
	return strm << "Use the \"" + transition._command + "\" command to trigger a state transitions from the \"" + transition._currentState + "\" state.";
}


State::State(string name, vector<Transition*> transition) {
	_name = name;
	_transition = transition;
}
State::State(const State &state)
{
	_name = state._name;
	_transition = vector<Transition*>();
	for (auto const& transition : state._transition) {
		_transition.push_back(new Transition(*transition));
	}

}
State& State::operator=(const State &state)
{
	_name = state._name;
	_transition = state._transition;
	return *this;
}
;

string State::getName() {
	return _name;
}

vector<Transition*> State::getTransition() {
	return _transition;
}
ostream& operator<<(ostream& strm, const State& state) {
	strm << "The current state is \"" + state._name + "\", and has those/this valid transition/s:" << endl;
	for (auto& transition : state._transition)
		 strm <<  *transition << endl;
	return strm;
}
