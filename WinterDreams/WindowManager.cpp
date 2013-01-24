#include "WindowManager.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML/Graphics/RenderStates.hpp>

WindowManager::WindowManager() :
	mWindow_p( new sf::RenderWindow(sf::VideoMode(800,600), "Winter Dreams") ),
	mRenderStates_p(new sf::RenderStates())
{}
	//Returns the static instance of WindowManager
WindowManager& WindowManager::get(){
	static WindowManager window;
	return window;
}
	//Returns a pointer to the static instance' RenderWindow
sf::RenderWindow* WindowManager::getWindow(){
	return mWindow_p;
}

sf::RenderStates* WindowManager::getStates(){
	return mRenderStates_p;
}