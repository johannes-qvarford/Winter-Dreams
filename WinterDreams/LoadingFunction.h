#ifndef INCLUDED_LOADINGFUNCTION
#define INCLUDED_LOADINGFUNCTION

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#include "LevelState.h"

struct LoadingSpecs{
	LoadingSpecs(const std::string& levelName, LevelState* level_p, sf::Mutex* boolRunningMutex_p, sf::Mutex* boolResourceMutex_p, bool* running_p, bool* mainRunning_p) :
		mLoadedLevel_p( level_p ),
		mLevelName( levelName ),
		mRunMutex_p( boolRunningMutex_p ),
		mResourceMutex_p( boolResourceMutex_p ),
		mRunning_p( running_p ),
		mMainThreadRunning_p(mainRunning_p)
	{
	}

	LoadingSpecs(const LoadingSpecs& l);
	~LoadingSpecs() {}

	LevelState* mLoadedLevel_p;	//Pointer to the gamestate into which the level is to be loaded
	
	sf::Mutex* mRunMutex_p;		//Keeps track so that several threads does not access the same bool mRunning
	sf::Mutex* mResourceMutex_p;	//Keeps track so that the loading thread does not access resoruces at shutdown
	bool* mRunning_p;			//Keeps track of whether the thread is running
	bool* mMainThreadRunning_p;
	std::string mLevelName;
	
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class loadingFunc{
public:
	static void loadLevel(LoadingSpecs& specs);
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#endif
