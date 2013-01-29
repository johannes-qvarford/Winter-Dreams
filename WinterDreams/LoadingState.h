#ifndef INCLUDED_LOADINGSTATE
#define INCLUDED_LOADINGSTATE

#include "State.h"

#include <string>
#include <boost/property_tree/ptree.hpp>

class GameState;

////////////////////////////////////////////////////////////
// /A LoadingState is a state which purpouse it to parse the data file
// /descibing a specific level and load the appropriate data into an
// /instance of GameState
////////////////////////////////////////////////////////////
class LoadingState : public State {
public:
	////////////////////////////////////////////////////////////
	// /LoadingState has to be told what GameState it is supposed
	// /to load a level into. It also has to be told what level to load.
	////////////////////////////////////////////////////////////
	LoadingState(GameState* gameState_p, std::string levelToLoad);

	////////////////////////////////////////////////////////////
	// /LoadingStates destructor DOES NOT delete mLoadedState.
	////////////////////////////////////////////////////////////
	~LoadingState();

	////////////////////////////////////////////////////////////
	// /Start loading the argument level. 
	// /Will eventually display the loading screen aswell.
	////////////////////////////////////////////////////////////
	virtual void update(int milliseconds);

private:
	GameState* mLoadedLevel;	//A pointer to a GameState with the argument level loaded into it.
	boost::property_tree::ptree mLevelData;
	std::string mLevelName;

	////////////////////////////////////////////////////////////
	// /No copies
	////////////////////////////////////////////////////////////
	LoadingState(const LoadingState& loading);
	////////////////////////////////////////////////////////////
	// /No copies
	////////////////////////////////////////////////////////////
	LoadingState& operator=(const LoadingState& loading);
};

#endif