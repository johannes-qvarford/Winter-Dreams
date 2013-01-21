#ifndef INCLUDED_STATEMANAGER
#define INCLUDED_STATEMANAGER

#include <stack>

class State;

/*
	The StateManager is a singleton that manages the different states of the game.
*/
class StateManager {
public:

	/*
		Get the singleton.
	*/
	static StateManager& get();

	/*
		Run the game.
	*/
	void run();

	/*
		Get the current State.
	*/
	State* getState();

	/*
		Push a new State. Only this state will be updated the next frame.
	*/
	void pushState(State* state);

	/*
		Pop and destroy the current State.
	*/
	void popState();

private:

	StateManager();

	StateManager(const StateManager&);//not implemented

	StateManager& operator=(const StateManager&);// not implemented

	std::stack<State*> mStates;
};

#endif