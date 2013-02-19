#ifndef INCLUDED_STARTGAMEBUTTON
#define INCLUDED_STARTGAMEBUTTON

#include "Button.h"

class StartGameButton : public Button {
	////////////////////////////////////////////////////////////
	// /Create a start game button.
	////////////////////////////////////////////////////////////
	StartGameButton(const sf::Vector2f& initialPosition, std::shared_ptr<sf::Font> font_sp, const sf::Text& text);

	////////////////////////////////////////////////////////////
	// /Does the following:
	// /fadeout, pop state, push loading state, fadein.
	////////////////////////////////////////////////////////////
	void activate();
};

#endif