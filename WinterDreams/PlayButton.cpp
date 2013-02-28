#include "PlayButton.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "StateManager.h"
#include "LoadingVideoState.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "StateManager.h"
#include "InputManager.h"

class PlayButtonSpecs {
public:

	static PlayButtonSpecs& get(){ static PlayButtonSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;

	std::string mFilename;

private:

	PlayButtonSpecs() {
		auto& play = PropertyManager::get().getGeneralSettings().get_child("ui.mainmenu.play");
		mXOffset = play.get<float>("xoffset");
		mYOffset = play.get<float>("yoffset");
		mFilename = play.get<std::string>("filename");
	}
};

PlayButton::PlayButton():
	Button(sf::Vector2f(PlayButtonSpecs::get().mXOffset, PlayButtonSpecs::get().mYOffset), PlayButtonSpecs::get().mFilename),
	mUpdated(false)
{
}

void PlayButton::activate() {
	
	if(mUpdated == false && InputManager::get().isADown()) {
		mUpdated = true;

		auto first_level_name = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_name");

		auto loadingState_p = new LoadingVideoState(first_level_name);
		auto& stateMgr = StateManager::get();

		stateMgr.freezeState(100);
		stateMgr.popState();
		stateMgr.pushState(loadingState_p);
		stateMgr.unfreezeState(100);
	}
}
