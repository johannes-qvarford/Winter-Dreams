#ifndef INCLUDED_KEYBINDBUTTON
#define INCLUDED_KEYBINDBUTTON

#include "ResourceManager.h"
#include "Button.h"
#include <string>

class KeyBindButton : public Button {
public:
	////////////////////////////////////////////////////////////
	// /Create a sound volume slider "button".
	////////////////////////////////////////////////////////////
	KeyBindButton();

	////////////////////////////////////////////////////////////
	// /Does the following:
	// /changes the sound volume
	////////////////////////////////////////////////////////////
	void activate();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void update();
private:

	void bindKey(int ei);

	std::shared_ptr<sf::Font> mFont;
	sf::Keyboard::Key mKeyCode;
	std::string mFilename;
	std::string mKeyname;
	std::shared_ptr<sf::Texture> mTexture;
	bool mUpdated;
};

#endif