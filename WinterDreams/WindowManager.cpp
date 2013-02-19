#include "WindowManager.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

static const int LIGHT_ID_MAX = 10;

WindowManager::WindowManager() :
	mWindow_p( new sf::RenderWindow(sf::VideoMode(800,600), "Winter Dreams") ),
	mTexture_p( new sf::RenderTexture()),
	mRenderStates_p(new sf::RenderStates()),
	mNextLightID( 0 )
{
	mTexture_p->create(800, 600);
	mWindow_p->setVerticalSyncEnabled( true );
}
	//Returns the static instance of WindowManager
WindowManager& WindowManager::get(){
	static WindowManager window;
	return window;
}
	//Returns a pointer to the static instance' RenderTexture
sf::RenderTexture* WindowManager::getWindow(){
	return mTexture_p;
}

	//Returns a pointer to the static instance's RenderWindow
sf::RenderWindow* WindowManager::getRenderWindow(){
	return mWindow_p;
}

sf::RenderStates* WindowManager::getStates(){
	return mRenderStates_p;
}

void WindowManager::resizeTexture(unsigned int x, unsigned int y){
	mTexture_p->create(x,y);
	sf::View view(sf::FloatRect(0, 0, static_cast<float>(x), static_cast<float>(y) ) );
	mWindow_p->setView(view);
}

void WindowManager::resetLightIDs(){
	mNextLightID = 0;
}

unsigned int WindowManager::getNextLightID(){
	int value = mNextLightID;

	++mNextLightID;

	assert( mNextLightID <= LIGHT_ID_MAX );

	return value;
}