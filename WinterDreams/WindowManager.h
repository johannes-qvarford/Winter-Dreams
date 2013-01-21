#ifndef INCLUDED_WINDOWMANAGER
#define INCLUDED_WINDOWMANAGER

namespace sf {
	class RenderWindow;
};

/*
	WindowManager is a singleton of the only window in the game.
*/
class WindowManager {
public:

	/*
		Get the singleton.
	*/
	static WindowManager& get();

	/*
		Get the window for drawing.
	*/
	sf::RenderWindow* getWindow();

private:

	WindowManager();
	
	WindowManager(const WindowManager&);//not implemented
	
	WindowManager& operator=(const WindowManager&);//not implemented

	sf::RenderWindow* mWindow_p;
};

#endif