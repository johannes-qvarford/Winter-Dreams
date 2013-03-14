#ifndef INCLUDED_RETURNTOMAINBUTTON
#define INCLUDED_RETURNTOMAINBUTTON

#include "Button.h"
namespace sf{
	class Texture;
}
class PromptToMainButton : public Button {
public:
	////////////////////////////////////////////////////////////
	// /Create a button that will ask the user if he really wishes
	// /to return to main menu.
	// /Not to be confused with the GoToMainMenuButton
	////////////////////////////////////////////////////////////
	PromptToMainButton(const sf::Vector2f& position, std::shared_ptr<sf::Texture> bgTexture);

	~PromptToMainButton();

	////////////////////////////////////////////////////////////
	// /Does the following:
	// /-Add a prompt-quit message.
	////////////////////////////////////////////////////////////
	void activate();

private:

	bool mUpdated;
	std::shared_ptr<sf::Texture> mBgTexture;
};

#endif