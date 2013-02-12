#ifndef INCLUDED_LOADINGSTATE
#define INCLUDED_LOADINGSTATE

#include "State.h"

#include <string>
#include <SFML\System.hpp>
#include <boost/property_tree/ptree.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Sprite.hpp>


class GameState;
struct LoadingSpecs;

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
	// /
	// /LoadingState is more of an interface since the loading is done
	// /by another class while LoadingState displays the loadingscreen.
	////////////////////////////////////////////////////////////
	LoadingState(GameState* gameState_p, std::string levelToLoad);

	////////////////////////////////////////////////////////////
	// /LoadingStates destructor DOES NOT delete mLoadedState.
	// /
	// /It does, however, delete it's associated loading thread.
	////////////////////////////////////////////////////////////
	~LoadingState();

	////////////////////////////////////////////////////////////
	// /Start loading the argument level. 
	// /Will eventually display the loading screen aswell.
	////////////////////////////////////////////////////////////
	virtual void update();
	////////////////////////////////////////////////////////////
	// /Draws the loadingscreen
	////////////////////////////////////////////////////////////
	void render();

private:
	GameState* mLoadedLevel;	//A pointer to a GameState with the argument level loaded into it.

	LoadingSpecs* mLoadingSpecs;	//A pointer to the class responsible for the thread loading the level
	sf::Thread mThread;				//The thread that loads the level
	sf::Mutex mMutex;				//A mutex for avoiding that different threads access the mRunning bool 
	bool mRunning;					//Keeps track of whether the loading thread is finished or not	

	std::shared_ptr<sf::Texture> mLoadingScreenTexture;
	std::shared_ptr<sf::Texture> mLoadingIconTexture;
	sf::Sprite mLoadingScreen;
	sf::Sprite mLoadingIcon;

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