#include "WindowManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

static const int LIGHT_ID_MAX = 10;

const unsigned int WINDOW_WINDOWED = sf::Style::Titlebar | sf::Style::Close;
//const unsigned int WINDOW_WINDOWED = sf::Style::None;
const unsigned int WINDOW_FULLSCREEN = sf::Style::None;

//Returns the static instance of WindowManager
WindowManager& WindowManager::get(){
	static WindowManager window;
	return window;
}

WindowManager::WindowManager() :
	mWindow(),
	mTexture(),
	mTexture2(),
	mRenderStates(),
	mNextLightID(0),
	mMode()
{
#ifdef SHIPPING
	mFullscreen = true;
#else
	mFullscreen = false;
#endif
	mWindow.setActive(true);
	mMode = sf::VideoMode::getFullscreenModes()[0];

	//update video mode first.
	onDesktopModeChanged();
}

bool WindowManager::update() {
	mWindow.clear();
	//we have to adjust if desktop mode changed last frame.
	auto desktop = sf::VideoMode::getDesktopMode();
	if(desktop != mLastDesktopMode)
		onDesktopModeChanged();

	mTexture2.display();
	mWindow.draw( sf::Sprite(mTexture2.getTexture()) );
	mWindow.display();

/*
//	Poll sfml window events.
//	Returns whether or not the game should end.
*/
	sf::Event ev;

	//poll events. for now, end game if window is closed.
	while(mWindow.pollEvent(ev))
	{
		switch(ev.type) {
				//if the window's X is pressed
			case sf::Event::Closed:
				return false;
				break;
			case sf::Event::Resized:
//				WindowManager::get().resizeTexture(ev.size.width, ev.size.height);

				break;
			default:
				break;
		}
			//if Esc is pressed
		if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape){
#ifndef SHIPPING_REAL
			return false;
#endif
		}
	}

	mTexture.clear();
	mTexture2.clear();

	return true;
}


void WindowManager::setFullscreenMode(bool fullscreen) {
	//don't do anything if in same mode already.
	if(mFullscreen == fullscreen)
		return;

	mFullscreen = fullscreen;

	auto style = (mFullscreen ? WINDOW_FULLSCREEN : WINDOW_WINDOWED);
	setVideoMode(mMode.width, mMode.height, style);
}

//Returns a pointer to the static instance' RenderTexture
sf::RenderTexture* WindowManager::getWindow(){
	return &mTexture;
}

	//Returns a pointer to the static instance's RenderWindow
sf::RenderTexture* WindowManager::getRenderWindow(){
	return &mTexture2;
}

sf::RenderStates* WindowManager::getStates(){
	return &mRenderStates;
}

void WindowManager::resetLightIDs(){
	mNextLightID = 0;
}

unsigned int WindowManager::getNextLightID(){
	int value = mNextLightID;

	++mNextLightID;

	//assert( mNextLightID <= LIGHT_ID_MAX );

	return value;
}

void WindowManager::onDesktopModeChanged() {
	mLastDesktopMode = sf::VideoMode::getDesktopMode();

	//check if desktop mode is worse than current mode.
	//we have to adjust to desktop mode if so.
	int deski;
	int curi;
	auto modes = sf::VideoMode::getFullscreenModes();
	for(size_t i = 0; i < modes.size(); ++i) {
		if(modes[i] == mLastDesktopMode) {
			deski = i;
		}
		if(modes[i] == mMode) {
			curi = i;
		}
	}

	if(curi < deski)
		mMode = mLastDesktopMode;

	setVideoMode(mMode.width, mMode.height, (mFullscreen ? WINDOW_FULLSCREEN : WINDOW_WINDOWED));
}

void WindowManager::setVideoMode(int width, int height, int style) {
	mWindow.create(sf::VideoMode(width, height), "Winter Dreams", style);
	mTexture.create(width, height);
	mTexture2.create(width, height);
}
