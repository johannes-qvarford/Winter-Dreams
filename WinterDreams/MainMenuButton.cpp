#include "MainMenuButton.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "StateManager.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "StateManager.h"
#include "InputManager.h"
#include "MenuState.h"

class MainMenuButtonSpecs {
public:

	static MainMenuButtonSpecs& get(){ static MainMenuButtonSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;

	std::string mFilename;

private:

	MainMenuButtonSpecs() {
		auto& MainMenu = PropertyManager::get().getGeneralSettings().get_child("ui.ingamemenu.mainmenu");
		mXOffset = MainMenu.get<float>("xoffset");
		mYOffset = MainMenu.get<float>("yoffset");
		mFilename = MainMenu.get<std::string>("filename");
	}
};

MainMenuButton::MainMenuButton():
	Button(sf::Vector2f(MainMenuButtonSpecs::get().mXOffset, MainMenuButtonSpecs::get().mYOffset), MainMenuButtonSpecs::get().mFilename),
	mUpdated(false)
{
}

void MainMenuButton::activate() {
	
	if(mUpdated == false && (InputManager::get().isADown() ||InputManager::get().isStartDown() ) ){
		mUpdated = true;
		Button::activate();

		auto& stateMgr = StateManager::get();
		auto mms = MenuState::makeMainMenuState();

		stateMgr.popState();
		stateMgr.popState();
		stateMgr.pushState(mms);

		onHover(false);
	}
}
