#ifndef INCLUDED_INPUTMANAGER
#define INCLUDED_INPUTMANAGER

#include <SFML/Keyboard.h>

/*
	Singleton that manages which buttons have been pressed.
	Its supposed to be filled by another object, that handles a windows event.
	Every time(frame) the object reads the events from the queue, it should call the InputManagers reset method. 
*/
class InputManager {

public:

	/*
		Get the singleton.
	*/
	static InputManager& get();

	/*
		Reset the "was pressed this frame" state.
	*/
	void reset();

	/*
		Press the 'key' key.
	*/
	void pressKey(sf::Keyboard::Key key);

	/*
		Release the 'key' key.
	*/
	void releaseKey(sf::Keyboard::Key key);

	/*
		Query if 'key' is down.
	*/
	bool isKeyDown(sf::Keyboard::Key key);

	/*
		Query whether or not the key was pressed this frame.
	*/
	bool wasKeyPressed(sf::Keyboard::Key key);

private:

	InputManager();

	InputManager(const InputManager&);//not implemented
	
	InputManager& operator=(const InputManager&);//not implemented

	bool mIsKeyDown[sf::Keyboard::KeyCount];
	
	bool mWasKeyPressed[sf::Keyboard::KeyCount];
};

#endif