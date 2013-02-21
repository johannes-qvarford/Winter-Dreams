#ifndef INCLUDED_INPUTMANAGER
#define INCLUDED_INPUTMANAGER
#include <SFML\System\Vector2.hpp>
#include <SFML\Window\Keyboard.hpp>
#include <string>

class InputManager{
public:
	/////////////////////////////////////////////////////////////////////
	// /Get or Create the Singleton
	// /The InputManager is unlocked by default.
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

	/////////////////////////////////////////////////////////////////////
	// /While the InputManager is locked, every button will return false
	// /and the stick will return (0,0).
	/////////////////////////////////////////////////////////////////////
	void lockInput();
	/////////////////////////////////////////////////////////////////////
	// /Allows the InputManager to return the buttons and stick's corrent
	// /values again.
	/////////////////////////////////////////////////////////////////////
	void unlockInput();
private:
	InputManager();
	~InputManager(){};
	InputManager(const InputManager& i){};
	InputManager& operator = (const InputManager&){};

	sf::Keyboard::Key KEY_A, KEY_B, KEY_START, KEY_SELECT;
	bool mIsUnlocked;
};
#endif