#include "SubLevelFade.h"
#include "WindowManager.h"
#include <SFML\Graphics\Vertex.hpp>


SubLevelFade::SubLevelFade(int fadeTime, SubLevelFade::FadeType type):
	Script(true),
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
			if (mType == FADE_IN)
				mAlpha -= (1.0f/mFadeTime);
			else if (mType == FADE_OUT)
				mAlpha += (1.0f/mFadeTime);

			mWaitingFrames++;
		}
		else
			setAlive(false);

}

void SubLevelFade::draw() const{
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
		sf::Vertex(sf::Vector2f(0, 0), sf::Color(0, 0, 0, intAlpha)),
		sf::Vertex(sf::Vector2f(0, VIEW_HEIGHT), sf::Color(0, 0, 0, intAlpha)),
		sf::Vertex(sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT), sf::Color(0, 0, 0, intAlpha)),
		sf::Vertex(sf::Vector2f(VIEW_WIDTH, 0), sf::Color(0, 0, 0, intAlpha))
	};

	auto newStates = states;
	newStates.blendMode = sf::BlendAlpha;
	window.draw(vertices, 4, sf::Quads, newStates);
	
}