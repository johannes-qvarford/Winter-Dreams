#include "SubLevelFade.h"
#include "WindowManager.h"
#include "InputManager.h"
#include <SFML\Graphics\Vertex.hpp>


SubLevelFade::SubLevelFade(int fadeTime, SubLevelFade::FadeType type):
	Entity(true),
	mFadeTime(fadeTime),
	mType(type),
	mWaitingFrames(0)
{
	if (mType == FADE_IN)
		mAlpha = 1;
	else if (mType == FADE_OUT)
		mAlpha = 0;
}

void SubLevelFade::update(SubLevel* subLevel_p){
	
		if (mWaitingFrames < mFadeTime){
			InputManager::get().lockInput();
			if (mType == FADE_IN)
				mAlpha -= (1.0f/mFadeTime);
			else if (mType == FADE_OUT)
				mAlpha += (1.0f/mFadeTime);

			mWaitingFrames++;
		}
		else{
			InputManager::get().unlockInput();
			setAlive(false);
		}

}

void SubLevelFade::draw() {
	auto& window = *WindowManager::get().getRenderWindow();
	auto& states = *WindowManager::get().getStates();

	//draw a black rectangle

	//turn float alpha to int8 alpha
	auto intAlpha = sf::Uint8();
	if(mAlpha > 1.0)
		intAlpha = 255;
	else if(mAlpha < 0.0)
		intAlpha = 0;
	else
		intAlpha = sf::Uint8(mAlpha * 255);

	sf::Vertex vertices[] = {
		sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color(0, 0, 0, intAlpha)),
		sf::Vertex(sf::Vector2f(0.f, static_cast<float>(VIEW_HEIGHT)), sf::Color(0, 0, 0, intAlpha)),
		sf::Vertex(sf::Vector2f(static_cast<float>(VIEW_WIDTH), static_cast<float>(VIEW_HEIGHT)), sf::Color(0, 0, 0, intAlpha)),
		sf::Vertex(sf::Vector2f(static_cast<float>(VIEW_WIDTH), 0.f), sf::Color(0, 0, 0, intAlpha))
	};

	auto newStates = states;
	newStates.blendMode = sf::BlendAlpha;
	window.draw(vertices, 4, sf::Quads, newStates);
	
}