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

VideoState::VideoState(const std::string& videoFileName) :
	mRequestPerformed( false )
{
	auto& rm = ResourceManager::get();
		//Get a shared_ptr to the video
	mVideo = rm.getVideo( FS_DIR_VIDEO + videoFileName );
		//Check the video for errors
	assert( !mVideo->hasError() );
		//Save the videos lenght
	auto lenght = mVideo->getDuration();
	mVidLenght += lenght;
}

VideoState::~VideoState() {}

void VideoState::update() {
/////////////////////////////////////////////
//Restart the delta time clock each frame.
//The video will use the return value from
//restart to calculate how many frames iut
//should advance
//////////////////////////////////////////////
	mVideo->update( mDeltaTime.restart() );

	if( mRunTime.getElapsedTime() + sf::seconds(2) > mVidLenght || InputManager::get().isStartDown() )
		reqestVideoEnd();
}

void VideoState::render() {
	auto& rendTex = *WindowManager::get().getRenderWindow();
	rendTex.draw( *mVideo );
}

void VideoState::onUnfreeze() {
	mRunTime.restart();
}
//////////////////////////////   PROTECTED  ////////////////////////////////////////////////

void VideoState::reqestVideoEnd() {
	if(!mRequestPerformed)
		onVideoEnd();
}

//////////////////////////////   PRIVATE  //////////////////////////////////////////////////

void VideoState::onVideoEnd() {
	if( mRequestPerformed )
		return;

	auto& sm = StateManager::get();
	sm.freezeState();
	sm.popState();
	sm.pushState( MenuState::makeMainMenuState() );
	sm.unfreezeState();

	mRequestPerformed = true;
}

