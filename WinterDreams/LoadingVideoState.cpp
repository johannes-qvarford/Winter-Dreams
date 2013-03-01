#include "LoadingVideoState.h"
#include "LoadingFunction.h"
#include "LevelState.h"
#include "PropertyManager.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "InputManager.h"
#include "WindowManager.h"
#include "StateManager.h"
#include "MenuState.h"

#include <sfTheora.h>
#include <SFML/Graphics/Texture.hpp>
#include <cassert>

LoadingVideoState::LoadingVideoState(const std::string& levelName) :
VideoState(PropertyManager::get().getGeneralSettings().get<std::string>(
		"levels." + levelName + ".intro_video")),
	mRunMutex(),
	mResoruceMutex(),
	mRunning( true ),
	mDone(false),	

	mLoadingSpecs_p ( new LoadingSpecs(levelName, new LevelState(levelName), &mRunMutex, &mResoruceMutex, &mRunning )), 
	mThread ( loadingFunc::loadLevel, *mLoadingSpecs_p) ,

	mIsLoadingIconTexture( ResourceManager::get().getTexture(FS_DIR_LOADINGSCREEN + "loadingicon.png") ),
	mDoneLoadingIconTexture( ResourceManager::get().getTexture(FS_DIR_LOADINGSCREEN + "doneloadingicon.png") )
	{

		mIsLoadingIcon.setTexture( *mIsLoadingIconTexture );
		mDoneLoadingIcon.setTexture( *mDoneLoadingIconTexture );

		mIsLoadingIcon.setOrigin(55,55);
		mIsLoadingIcon.setPosition(55,55);
		mDoneLoadingIcon.setOrigin(55,55);
		mDoneLoadingIcon.setPosition(55,55);
	}

LoadingVideoState::~LoadingVideoState() {
	mResoruceMutex.lock();
	mRunMutex.lock();
	mThread.terminate();	//Not a good solution but it works
	mRunMutex.unlock();
	mResoruceMutex.unlock();
}

void LoadingVideoState::update() {
	mRunMutex.lock();
	if( mRunning ){
		mRunMutex.unlock();
		mVideo->update( mDeltaTime.restart() );
		mIsLoadingIcon.rotate(-5);
	} else {
		mRunMutex.unlock();
		VideoState::update();
	}

}

void LoadingVideoState::render() {
	VideoState::render();

	auto& renderWindow = *WindowManager::get().getRenderWindow();

	mRunMutex.lock();
	if( mRunning ){
		mRunMutex.unlock();
		renderWindow.draw( mIsLoadingIcon );
	}
	else{
		mRunMutex.unlock();
		renderWindow.draw( mDoneLoadingIcon );
	}	
}

void LoadingVideoState::onUnfreeze() {	
	VideoState::onUnfreeze();
	mThread.launch();
}

void LoadingVideoState::onVideoEnd() {

	auto& sm = StateManager::get();
	sm.freezeState();
	sm.popState();
	sm.pushState( mLoadingSpecs_p->mLoadedLevel_p );
	sm.unfreezeState(0);

}


	