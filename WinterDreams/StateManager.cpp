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
	auto& window = *WindowManager::get().getRenderWindow();
	sf::Event ev;

	//poll events. for now, end game if window is closed.
	while(window.pollEvent(ev))
	{
		switch(ev.type) {
				//if the window's X is pressed
			case sf::Event::Closed:
				return false;
				break;
			case sf::Event::Resized:
				WindowManager::get().resizeTexture(ev.size.width, ev.size.height);
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
	const int FRAMES_PER_SECOND = 62;
	const unsigned long advances = 1000000 / FRAMES_PER_SECOND;
	/*
		MAX_FRAMESKIP may not be 0 or 1, since that
		will reuslt in a unlimited frame rate.
	*/
	const unsigned int MAX_FRAMESKIP = 5;	
	
	sf::Time next_game_tick( sf::microseconds(0) );
	sf::Clock GetTickCount;
	sf::Clock frameTime;
	sf::Clock rendTime;
	int loops = 0;

	frameTime.restart();
	rendTime.restart();

	while(true)
	{	
		if(loops == MAX_FRAMESKIP){
			next_game_tick = sf::microseconds(0);
			GetTickCount.restart();
		}
		loops = 0;

		if(mPopNextFrame) {
			mStates.pop();
			mPopNextFrame = false;

			next_game_tick = sf::microseconds(0);
			GetTickCount.restart();
		}
			
		if(pollEvents() == false)
			return;

		while(GetTickCount.getElapsedTime() > next_game_tick && loops < MAX_FRAMESKIP ) { 
			next_game_tick += sf::Time( sf::microseconds(advances) );
			++loops;

			mStates.top()->update();  
			std::cout << 1000 / (frameTime.getElapsedTime().asMilliseconds()+1)  <<" ";	//+1 to not get a "divided by zero" error
			frameTime.restart();

			mStates.top()->render();			
			std::cout<< "[" <<rendTime.getElapsedTime().asMicroseconds() <<"]\t";
			rendTime.restart();

			if( !mStates.top()->isRepeatable() )
				break;
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