#ifndef INCLUDED_WINDOWMANAGER
#define INCLUDED_WINDOWMANAGER

namespace sf {
	class RenderStates;
	class RenderTarget;
};

static const int VIEW_WIDTH = 1920;
static const int VIEW_HEIGHT = 1080;

////////////////////////////////////////////////////////////
// /WindowManager is a singleton of the only window in the game.
////////////////////////////////////////////////////////////
class WindowManager {
public:

	////////////////////////////////////////////////////////////
	// /Get the singleton.
	////////////////////////////////////////////////////////////
	static WindowManager& get();

	////////////////////////////////////////////////////////////
	// /check that the correct video mode is used, if user has changed
	// /desktop video mode.
	////////////////////////////////////////////////////////////
	bool update();

	////////////////////////////////////////////////////////////
	// /Set the video mode height and width.
	// /Check to make sure that the current desktop mode can support it.
	// /Use the best possible if not.
	////////////////////////////////////////////////////////////
	void setVideoMode(int width, int height, int style);

	////////////////////////////////////////////////////////////
	// /Set video mode to fullscreen or not.
	////////////////////////////////////////////////////////////
	void setFullscreenMode(bool fullscreen);

	////////////////////////////////////////////////////////////
	// /Get the texture for drawing.
	////////////////////////////////////////////////////////////
	sf::RenderTexture* getWindow();

	////////////////////////////////////////////////////////////
	// /Get the window for drawing.
	////////////////////////////////////////////////////////////
	sf::RenderTexture* getRenderWindow();

	////////////////////////////////////////////////////////////
	// /Get the RenderStates for drawing.
	////////////////////////////////////////////////////////////
	sf::RenderStates* getStates();

	////////////////////////////////////////////////////////////
	// /Resets the counter for light IDs
	////////////////////////////////////////////////////////////
	void resetLightIDs();

	////////////////////////////////////////////////////////////
	// /Returns the lowest avalible light ID. Every time this 
	// /function is called, the returned ID is concidered used
	// /and will not be availbile again untill resetLightIDs is
	// /called.
	////////////////////////////////////////////////////////////
	unsigned int getNextLightID();

private:

	void onDesktopModeChanged();

	WindowManager();
	
	WindowManager(const WindowManager&);//no copy
	
	WindowManager& operator=(const WindowManager&);//no copy

	int mNextLightID;

	bool mFullscreen;
	sf::VideoMode mMode;
	sf::VideoMode mLastDesktopMode;

	sf::RenderTexture mTexture;
	sf::RenderTexture mTexture2;
	sf::RenderWindow mWindow;
	sf::RenderStates mRenderStates;
};

#endif