#include "StateManager.h"
#include "State.h"
#include "WindowManager.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

/*
//	Poll sfml window events.
//	Returns whether or not the game should end.
*/
static bool pollEvents() {
	auto& window = *WindowManager::get().getWindow();
	sf::Event ev;

	//poll events. for now, end game if window is closed.
	while(window.pollEvent(ev))
	{
		switch(ev.type) {
				//if the window's X is pressed
			case sf::Event::Closed:
				return false;
				break;
			default:
				break;
		}
			//if Esc is pressed
		if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape){
			return false;
		}
	}
	return true;
}

StateManager::StateManager():
	mStates(),
	mPopNextFrame(false)
{
}

void StateManager::run() {
	sf::Clock clock;
	sf::Time limit = sf::milliseconds( static_cast<int>(16) );
	
	//sf::Time newTime;
	//sf::Time oldTime;

	//newTime = clock.getElapsedTime();
	
	while(true)
	{	
		clock.restart();
		//end game if pollEvents returns zero.
		if(pollEvents() == false)
			return;

		//oldTime = newTime;
		//newTime = clock.getElapsedTime();

		////time between frames
		//sf::Time deltaTime = newTime - oldTime;

		//update top state.
		mStates.top()->update();

		if(mPopNextFrame) {
			mStates.pop();
			mPopNextFrame = false;
		}

		if(limit > clock.getElapsedTime()) {
			sf::sleep( limit - sf::milliseconds( clock.getElapsedTime().asMilliseconds() ) );
		}
	}
}

void StateManager::pushState(State* state) {
	mStates.push(state);
}

void StateManager::popState() {
	//delay popping until next frame.
	mPopNextFrame = true;
}