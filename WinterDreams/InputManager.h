#ifndef INCLUDED_INPUTMANAGER
#define INCLUDED_INPUTMANAGER
#include <SFML\System\Vector2.hpp>
#include <SFML\Window\Keyboard.hpp>
#include <string>

class InputManager{
public:
	/////////////////////////////////////////////////////////////////////
	// /Get or Create the Singleton
	/////////////////////////////////////////////////////////////////////
	static InputManager& get();
	/////////////////////////////////////////////////////////////////////
	// /Returns the state of the virtual Joystick as a normalized vec2f
	/////////////////////////////////////////////////////////////////////
	sf::Vector2f getStick();
	/////////////////////////////////////////////////////////////////////
	// /Selfexplanatory. Returns true if KEY_A is down
	/////////////////////////////////////////////////////////////////////
	bool isADown();
	/////////////////////////////////////////////////////////////////////
	bool isBDown();
	bool isStartDown();
	bool isSelectDown();
	/////////////////////////////////////////////////////////////////////
	// /Sets an internal keyboard binding to whatever sf::Keyboard::Key
	// /You want. The internal bindings are:
	// /KEY_A
	// /KEY_B
	// /KEY_START
	// /KEY_SELECT
	// /Use the target as the first argument, and an sf::Keyboard::Key to
	// /whatever key you want to bind as the second.
	/////////////////////////////////////////////////////////////////////
	void setKeyBinding(std::string binding, sf::Keyboard::Key key);
private:
	InputManager();
	~InputManager(){};
	InputManager(const InputManager& i){};
	InputManager& operator = (const InputManager&){};

	sf::Keyboard::Key KEY_A, KEY_B, KEY_START, KEY_SELECT;
};
#endif