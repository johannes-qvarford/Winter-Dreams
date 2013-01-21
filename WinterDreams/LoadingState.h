#ifndef INCLUDED_LOADINGSTATE
#define INCLUDED_LOADINGSTATE

#include "State.h"

class State;

class LoadingState : public State {
public:
	/*
	//	LoadingState has to be told what level to load when it
	//	is created.
	*/
	LoadingState(unsigned int levelToLoad);
	/*
	// LoadingStates destructor DOES NOT delete mLoadedState.
	*/
	~LoadingState();

	/*
	//	?
	*/
	virtual void update();

private:
	State* mLoadedLevel;	//A pointer to a GameState with the argument level loaded into it.
};

#endif