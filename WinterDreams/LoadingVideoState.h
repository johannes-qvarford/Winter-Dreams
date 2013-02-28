#ifndef INCLUDED_LOADINGVIDEOSTATE
#define INCLUDED_LOADINGVIDEOSTATE

#include "VideoState.h"

#include <string>
#include <SFML/System.hpp>
#include <SFML/Graphics/Sprite.hpp>

struct LoadingSpecs;
namespace sf{
	class Texture;
};

class LoadingVideoState : public VideoState {
public:
	////////////////////////////////////////////////////////////
	// /LoadingVideoState has to be told what level to load.
	// /
	// /LoadingVideoState is more of an interface since the loading is done
	// /by another thread while LoadingVideoState displays the loadingvideo.
	////////////////////////////////////////////////////////////
	LoadingVideoState(const std::string& levelName);

	////////////////////////////////////////////////////////////
	// /LoadingStates destructor DOES NOT delete mLoadedState.
	// /
	// /It does, however, delete it's associated loading thread.
	////////////////////////////////////////////////////////////
	~LoadingVideoState();

	////////////////////////////////////////////////////////////
	// /LoadingVideoStates calls it's base class' update, and
	// /then adds a few functions related to the second thread.
	////////////////////////////////////////////////////////////
	void update();

	////////////////////////////////////////////////////////////
	// /LoadingVideoStates calls it's base class' render, and
	// /then adds a few functions related to the second thread.
	// /
	// /It also adds a "loading" icon as long as the level loads,
	// /and afterwards adds a "press X to skip" when the level
	// /is loaded.
	////////////////////////////////////////////////////////////
	void render();

	void onUnfreeze();

private:
	////////////////////////////////////////////////////////////
	// /LoadingVideoState has it's own implementation of
	// /onVideoEnd. This function can only be accessed via 
	// /requestVideoEnd().
	////////////////////////////////////////////////////////////
	void onVideoEnd();

	LoadingSpecs* mLoadingSpecs_p;	//A pointer to a controll-class responsible for the thread loading the level
	sf::Thread mThread;				//The thread that loads the level
	sf::Mutex mMutex;				//A mutex for avoiding that different threads access the mRunning bool 
	bool mRunning;					//Keeps track of whether the loading thread is finished or not	
	bool mDone;						//Has already queued actions for StateManager, is waiting to be destroyed.

	std::shared_ptr<sf::Texture>	mIsLoadingIconTexture;
	std::shared_ptr<sf::Texture>	mDoneLoadingIconTexture;

	sf::Sprite mIsLoadingIcon;
	sf::Sprite mDoneLoadingIcon;

///////////////// No copies ///////////////////////////////////////////
/**/LoadingVideoState(const VideoState& v);				// No copies //
/**/LoadingVideoState& operator=(const VideoState& v);	// No copies //
///////////////// No copies ///////////////////////////////////////////
};

#endif