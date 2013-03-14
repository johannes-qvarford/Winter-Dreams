#include "VideoState.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "InputManager.h"
#include "WindowManager.h"
#include "StateManager.h"
#include "MenuState.h"

#include <sfTheora.h>
#include <SFML\Audio\SoundBuffer.hpp>
#include <SFML\Audio\Sound.hpp>
#include <cassert>

VideoState::VideoState(const std::string& videoFileName, const std::string& musicFileName, const bool isSkippable) :
	mRequestPerformed( false ),
	mInitialized( false ),
	mIsSkippable( isSkippable ),
	mVideoFileName( videoFileName ),
	mMusicFileName( musicFileName )
{
}

VideoState::~VideoState() { 
	mMusic.stop();
}

void VideoState::update() {
	if( mInitialized == false ){
			auto& rm = ResourceManager::get();
		//Get a shared_ptr to the video
		mVideo = rm.getVideo( FS_DIR_VIDEO + mVideoFileName );

			//Check the video for errors
		assert( !mVideo->hasError() );
			//Save the videos lenght
		auto lenght = mVideo->getDuration();

		if( mMusicFileName != "" ){
			mMusic.openFromFile( FS_DIR_MUSIC + mMusicFileName );
			mMusic.play();
		}

		mInitialized = true;
	}

/////////////////////////////////////////////
//Restart the delta time clock each frame.
//The video will use the return value from
//restart to calculate how many frames iut
//should advance
//////////////////////////////////////////////
	mVideo->update( mDeltaTime.restart() );

	if( mVideo->isDone() || (InputManager::get().isStartDown() && mIsSkippable == true ) )
		reqestVideoEnd();
}

void VideoState::render() {
	auto& rendWin = *WindowManager::get().getRenderWindow();
	
	//scale video to fit window.
	auto bounds = mVideo->getLocalBounds();
	auto scale = sf::Vector2f(float(rendWin.getSize().x) / bounds.width, float(rendWin.getSize().y) / bounds.height);
	mVideo->setScale(scale);
	
	rendWin.draw( *mVideo );
}

void VideoState::onUnfreeze() {
	State::onUnfreeze();
	mDeltaTime.restart();
}
//////////////////////////////   PROTECTED  ////////////////////////////////////////////////

void VideoState::reqestVideoEnd() {
	if(!mRequestPerformed){
		mRequestPerformed = true;
		onVideoEnd();
	}
}

//////////////////////////////   PRIVATE  //////////////////////////////////////////////////

void VideoState::onVideoEnd() {

	auto& sm = StateManager::get();
	sm.freezeState();
	sm.popState();
	sm.unfreezeState();
}

