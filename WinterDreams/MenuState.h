#ifndef INCLUDED_MENUSTATE
#define INCLUDED_MENUSTATE

#include "State.h"
#include <string>
#include <list>
#include <SFML/Graphics/Texture.hpp>

class Widget;
namespace sf{
	class Sound;
	class SoundBuffer;
};
////////////////////////////////////////////////////////////
// /MenuState represents the a menu.
// /MenuStates can only be created with its static member functions.
////////////////////////////////////////////////////////////
class MenuState : public State{
public:

	////////////////////////////////////////////////////////////
	// /Make a menu state used at the title screen.
	////////////////////////////////////////////////////////////
	static MenuState* makeMainMenuState();

	////////////////////////////////////////////////////////////
	// /Make a menu state where the games settings can be altered.
	// /TODO: implement.
	////////////////////////////////////////////////////////////
	static MenuState* makeSettingsMenuState();

	////////////////////////////////////////////////////////////
	// /Make a menu state that appears when the user pauses the
	// /game.
	// /TODO: implment.
	////////////////////////////////////////////////////////////
	static MenuState* makeIngameMenuState();

public:	

	////////////////////////////////////////////////////////////
	// /Each loop, if MenuState is the active state, update is called
	////////////////////////////////////////////////////////////
	void update();
	
	////////////////////////////////////////////////////////////
	// /Render the content of the MenuState. 
	////////////////////////////////////////////////////////////
	void render();

private:

	std::list<std::shared_ptr<Widget> > mWidgets;

	std::shared_ptr<sf::Texture>	 mBackground_sp;

	std::shared_ptr<sf::SoundBuffer>	mMusicBuffer;
	std::shared_ptr<sf::Sound>			mMusic;

	////////////////////////////////////////////////////////////
	// /MenuStates constructor performs no actions
	////////////////////////////////////////////////////////////
	MenuState();
	
	////////////////////////////////////////////////////////////
	// /MenuStates destructor performs no actions
	////////////////////////////////////////////////////////////
	~MenuState();

	////////////////////////////////////////////////////////////
	// /Set the background image for the MenuState.
	////////////////////////////////////////////////////////////
	void setBackground(std::shared_ptr<sf::Texture> texture_sp);

	////////////////////////////////////////////////////////////
	// /Add a widget to the MenuState.
	////////////////////////////////////////////////////////////
	void addWidget(std::shared_ptr<Widget> widget_sp);

	////////////////////////////////////////////////////////////
	// /Add some music to the MenuState.
	////////////////////////////////////////////////////////////
	void addMusic(std::shared_ptr<sf::SoundBuffer> music_sp);

	////////////////////////////////////////////////////////////
	// /No copies
	////////////////////////////////////////////////////////////
	MenuState(const MenuState& menu);

	////////////////////////////////////////////////////////////
	// /No copies
	////////////////////////////////////////////////////////////
	MenuState& operator=(const MenuState& menu);
};

#endif