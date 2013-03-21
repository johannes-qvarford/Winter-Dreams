
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
VideoState(
		PropertyManager::get().getGeneralSettings().get<std::string>("levels." + levelName + ".intro_video"), 
		PropertyManager::get().getGeneralSettings().get<std::string>("levels." + levelName + ".intro_music", "") ),
	mRunMutex(),
	mResoruceMutex(),
	mRunning( true ),
	mMainThreadRunning( true ),
	mDone(false),	

	mLoadingSpecs_p ( new LoadingSpecs(levelName, new LevelState(levelName), &mRunMutex, &mResoruceMutex, &mRunning, &mMainThreadRunning )), 
	mThread ( loadingFunc::loadLevel, *mLoadingSpecs_p) ,

//	mIsLoadingIconTexture( ResourceManager::get().getTexture(FS_DIR_LOADINGSCREEN + "loadingicon.png") ),
	mDoneLoadingIconTexture( ResourceManager::get().getTexture(FS_DIR_LOADINGSCREEN + "doneloadingicon.png") )
	{

//		mIsLoadingIcon.setTexture( *mIsLoadingIconTexture );
		mDoneLoadingIcon.setTexture( *mDoneLoadingIconTexture );
	}

LoadingVideoState::~LoadingVideoState() {

	//mResoruceMutex.lock();
	mRunMutex.lock();
	mMainThreadRunning = false;
	mRunMutex.unlock();

	mThread.wait();	//Not a good solution but it works
	
	//mRunMutex.unlock();
	//mResoruceMutex.unlock();
}

void LoadingVideoState::update() {
	static sf::Context* t = new sf::Context();
	static sf::Context* s = nullptr;

	//sf::Context context;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && t != nullptr) {
		auto b = reinterpret_cast<unsigned int*>(t) + 1;
		//*b = 0;
		delete t;
		t = nullptr;
	}

	

	if(s == nullptr && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		s = new sf::Context();
	}

	if( mInitialized == false )
		VideoState::update();

	mRunMutex.lock();
	if( mRunning ){
		mRunMutex.unlock();
		mVideo->update( mDeltaTime.restart() );
	} else {
		mRunMutex.unlock();
		VideoState::update();
	}

	auto win = WindowManager::get().getRenderWindow()->getSize();
	mDoneLoadingIcon.setPosition( 0.02f * win.x, 0.9f * win.y); 
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
	State::onUnfreeze();
	VideoState::onUnfreeze();
	mThread.launch();
}

void LoadingVideoState::onVideoEnd() {

	auto& sm = StateManager::get();
	sm.freezeState();
	sm.popState();
	sm.pushState( mLoadingSpecs_p->mLoadedLevel_p );
	sm.unfreezeState();

}


	