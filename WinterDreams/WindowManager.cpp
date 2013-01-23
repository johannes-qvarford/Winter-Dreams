#include "WindowManager.h"
#include <SFML\Graphics\RenderWindow.hpp>

WindowManager::WindowManager() :
	mWindow_p( new sf::RenderWindow(sf::VideoMode(800,600), "Winter Dreams") )
{}
	//Returns the static instance of WindowManager
WindowManager& WindowManager::get(){
	static WindowManager window;
	return window;
}
	//Returns a pointer to the static instance' RenderWindow
sf::RenderWindow* WindowManager.getWindow(){
	return WindowManager::get().mWindow_p;
}