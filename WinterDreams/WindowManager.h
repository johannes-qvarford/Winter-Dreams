#ifndef INCLUDED_WINDOWMANAGER
#define INCLUDED_WINDOWMANAGER

namespace sf {
	class RenderWindow;
	class RenderStates;
};

/*
//	WindowManager is a singleton of the only window in the game.
*/
class WindowManager {
public:

	/*
	//	Get the singleton.
	*/
	static WindowManager& get();

	/*
	//	Get the window for drawing.
	*/
	sf::RenderWindow* getWindow();

	/*
	//	Get the RenderStates for drawing.
	*/
	sf::RenderStates* getStates();

private:

	WindowManager();
	
	WindowManager(const WindowManager&);//no copy
	
	WindowManager& operator=(const WindowManager&);//no copy

	sf::RenderWindow* mWindow_p;
	sf::RenderStates* mRenderStates;
};

#endif