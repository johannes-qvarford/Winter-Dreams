#include "StateManager.h"
#include "State.h"
#include "WindowManager.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <gl/GLU.h>

#include <cmath>
#include <cassert>

static const float FADE_OUT_SPEED = 0.01f;
static const float FADE_IN_SPEED = 0.01f;

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
//				WindowManager::get().resizeTexture(ev.size.width, ev.size.height);

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
	mStateOfManager(NORMAL),
	mStates(),
	mActions(),
	mAlpha(0)
{
}

StateManager::~StateManager() {
	while(mStates.empty() == false) {
		mStates.pop();
	}
}

void StateManager::run() {
		
	const int FRAMES_PER_SECOND = 62;
	const unsigned long advances = 1000000 / FRAMES_PER_SECOND;
	/*
		MAX_FRAMESKIP may not be 0 or 1, since that
		will reuslt in a unlimited frame rate.
	*/
	const unsigned int MAX_FRAMESKIP = 5;	
	
	//time from the start of the state manager creation, to the next game_tick.
	sf::Time next_game_tick( sf::microseconds(0) );
	
	sf::Clock GetTickCount;
	sf::Clock frameTime;
	int loops = 0;

	//time per frame.
	frameTime.restart();

	while(true)
	{	
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			auto& window = *WindowManager::get().getRenderWindow();
			static bool swap = false;
			unsigned int style;
			if(swap)
				style = sf::Style::Titlebar | sf::Style::Close;
			else
				style = sf::Style::Fullscreen;
			swap = !swap;

			window.create(sf::VideoMode(WindowManager::MAX_WIDTH, WindowManager::MAX_HEIGHT), "Winter Dreams", style); 
		}





		if(pollEvents() == false)
			return;

		//try to catch up, by doing a maximun of MAX_FRAMESKIP updates.
		while(GetTickCount.getElapsedTime() > next_game_tick && loops < MAX_FRAMESKIP ) { 
			WindowManager::get().resetLightIDs();

			next_game_tick += sf::Time( sf::microseconds(advances) );
			++loops;

			updateFrame();

			std::cout << 1000 / (frameTime.getElapsedTime().asMilliseconds()+1)  <<" ";	//+1 to not get a "divided by zero" error
			std::cout<< "[" <<frameTime.getElapsedTime().asMicroseconds() <<"]\t";
			
			frameTime.restart();
		}
		
		//if we performed the maximum number of frames to catch up, reset the clock. 
		if(loops == MAX_FRAMESKIP){
			next_game_tick = sf::microseconds(0);
			GetTickCount.restart();
		}
		loops = 0;
	}
}

void StateManager::pushState(State* state_p) {
	Action action;
	action.mTag = Action::PUSH;
	action.mData.mState_p = state_p;
	mActions.push(action);
}

void StateManager::unfreezeState() {
	Action action;
	action.mTag = Action::UNFREEZE;
	action.mData.mNull = nullptr;
	mActions.push(action);
}

void StateManager::popState() {
	Action action;
	action.mTag = Action::POP;
	action.mData.mNull = nullptr;
	mActions.push(action);
}

void StateManager::freezeState() {
	Action action;
	action.mTag = Action::FREEZE;
	action.mData.mNull = nullptr;
	mActions.push(action);
}


void StateManager::updateFrame() {

	switch(mStateOfManager) {
	case FADING_OUT:
		updateFadingOut();
		break;
	case FADING_IN:
		updateFadingIn();
		break;
	case NORMAL:
		updateNormal();
		break;
	default:
		assert(0);
		break;
	}
}

void StateManager::darkenWindow(float alpha) {
	
	auto& window = *WindowManager::get().getRenderWindow();
	auto& states = *WindowManager::get().getStates();

	//draw a black rectangle
	static const float WIDTH = WindowManager::MAX_WIDTH;
	static const float HEIGHT = WindowManager::MAX_HEIGHT;
	
	//turn float alpha to int8 alpha
	auto intAlpha = sf::Uint8();
	if(alpha > 1.0)
		intAlpha = 255;
	else if(alpha < 0.0)
		intAlpha = 0;
	else
		intAlpha = sf::Uint8(alpha * 255);

	sf::Vertex vertices[] = {
		sf::Vertex(sf::Vector2f(0, 0), sf::Color(1, 1, 1, intAlpha)),
		sf::Vertex(sf::Vector2f(0, HEIGHT), sf::Color(1, 1, 1, intAlpha)),
		sf::Vertex(sf::Vector2f(WIDTH, HEIGHT), sf::Color(1, 1, 1, intAlpha)),
		sf::Vertex(sf::Vector2f(WIDTH, 0), sf::Color(1, 1, 1, intAlpha))
	};

	auto newStates = states;
	newStates.blendMode = sf::BlendAlpha;
	window.draw(vertices, 4, sf::Quads, newStates);
	
	window.display();
}

void StateManager::prepareWindow() {
	auto& window = *WindowManager::get().getRenderWindow();
	window.clear();
}

void StateManager::updateNormal() {
	//process queue until going into fade mode
	while(mStateOfManager == NORMAL && mActions.empty() == false) {
		auto action = mActions.front();
		mActions.pop();

		switch(action.mTag) {
		case Action::POP:
			//using unique_ptr, no need to delete.
			mStates.pop();
			break;
		case Action::PUSH:
			mStates.push(std::unique_ptr<State>(action.mData.mState_p));
			break;
		case Action::FREEZE:
			mStateOfManager = FADING_OUT;
			mAlpha = 0.f;
			mStates.top()->onFreeze();
			break;
		case Action::UNFREEZE:
			mStateOfManager = FADING_IN;
			mAlpha = 1.f;
			mStates.top()->onUnfreeze();
			break;
		default:
			assert(0);
			break;
		}
	}

	if(mStateOfManager != NORMAL) {
		updateFrame();
		return;
	}

	mStates.top()->update();
	prepareWindow();
	mStates.top()->render();
	darkenWindow(0.f);
	return;
}

void StateManager::updateFadingIn() {
	//fade linearly, continue as usual after that.
	mAlpha -= FADE_IN_SPEED;
	if(mAlpha <= 0.f)
		mStateOfManager = NORMAL;

	mStates.top()->update();
	prepareWindow();
	mStates.top()->render();
	darkenWindow(mAlpha);
//	darkenWindow(0.f);
	return;
}

void StateManager::updateFadingOut() {
	//fade linearly, continue as usual after that.
	mAlpha += FADE_OUT_SPEED;
	if(mAlpha >= 1.f)
		mStateOfManager = NORMAL;

	mStates.top()->update();
	prepareWindow();
	mStates.top()->render();
	darkenWindow(mAlpha);
//	darkenWindow(0.f);
	return;
}