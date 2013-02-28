#ifndef INCLUDED_WIDGET
#define INCLUDED_WIDGET

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Keyboard.hpp>

class MenuState;


namespace sf {
	class RenderTarget;
};

////////////////////////////////////////////////////////////
// /Widget is a graphical and intelligent thing 
// /that takes up a part of the screen.
////////////////////////////////////////////////////////////
class Widget : public sf::Drawable {
public:
	enum Key{ KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT, KEY_A, KEY_B, KEY_START, KEY_SELECT };

	////////////////////////////////////////////////////////////
	// /"Activate" the widget. 
	// /Activating a button when pressing the KEY_A key could take
	// /you to a sub menu for example.
	////////////////////////////////////////////////////////////
	virtual void activate(){};

	////////////////////////////////////////////////////////////
	// /Method that is called when something hovers(or stops hovering) over the widget.
	////////////////////////////////////////////////////////////
	virtual void onHover(bool doesHover){};

	////////////////////////////////////////////////////////////
	// /Update the state of the widget.
	////////////////////////////////////////////////////////////
	virtual void update(MenuState* state_p){};

	////////////////////////////////////////////////////////////
	// /Get the graphical bounds of the widget(0 to 1).
	////////////////////////////////////////////////////////////
	virtual const sf::FloatRect& getBounds() const = 0;

	////////////////////////////////////////////////////////////
	// /Get the graphical bounds of the widget(0 to 1).
	////////////////////////////////////////////////////////////
	virtual sf::FloatRect& getBounds() = 0;

	////////////////////////////////////////////////////////////
	// /Set the bounds of the widget (0 to 1).
	////////////////////////////////////////////////////////////
	virtual void setBounds(const sf::FloatRect& bounds) = 0;
};

#endif