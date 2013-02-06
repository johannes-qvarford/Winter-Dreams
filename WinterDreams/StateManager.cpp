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
	const int FRAMES_PER_SECOND = 65;
	const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
	/*
		Since LoadingState's update is called int he game loop,
		we cannot have a frame skip higher then 1. If we increase
		frameskip higher then 1, LoadingState will run one
		additional time for each frameskip higher then 1.
	*/
	const int MAX_FRAMESKIP = 1;	
	
	sf::Time next_game_tick( sf::microseconds(0) );
	sf::Clock GetTickCount;
	sf::Clock frameTime;
	int loops = 0;

	while(true)
	{	
		loops = 0;	

		if(pollEvents() == false)
			return;

		while(GetTickCount.getElapsedTime() > next_game_tick && loops < MAX_FRAMESKIP ) { 
			mStates.top()->update();  

			next_game_tick += sf::Time( sf::milliseconds(SKIP_TICKS) );
			++loops;
			std::cout << 1000 / (frameTime.getElapsedTime().asMilliseconds()+1) <<"\t";	//+1 to not get a "divided by zero" error
			frameTime.restart();
		}	

		mStates.top()->render();

		if(mPopNextFrame) {
			mStates.pop();
			mPopNextFrame = false;

			next_game_tick = sf::microseconds(0);
			GetTickCount.restart();
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