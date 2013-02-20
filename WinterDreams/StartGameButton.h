#ifndef INCLUDED_STARTGAMEBUTTON
#define INCLUDED_STARTGAMEBUTTON

#include "Button.h"

class StartGameButton : public Button {
public:
	////////////////////////////////////////////////////////////
	// /Create a start game button.
	////////////////////////////////////////////////////////////
	StartGameButton(const sf::Vector2f& initialPosition);

	////////////////////////////////////////////////////////////
	// /Does the following:
	// /fadeout, pop state, push loading state, fadein.
	////////////////////////////////////////////////////////////
	void activate();

private:

	bool mUpdated;
};

#endif