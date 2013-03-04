#include "CreditsButton.h"
#include "PropertyManager.h"
#include "InputManager.h"
#include "StateManager.h"

#include <SFML/Audio/Music.hpp>

class CreditsButtonSpecs {
public:

	static CreditsButtonSpecs& get(){ static CreditsButtonSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;

	std::string mFilename;

private:

	CreditsButtonSpecs() {
		auto& credits = PropertyManager::get().getGeneralSettings().get_child("ui.mainmenu.credits");
		mXOffset = credits.get<float>("xoffset");
		mYOffset = credits.get<float>("yoffset");
		mFilename = credits.get<std::string>("filename");
	}
};


CreditsButton::CreditsButton():
	Button(sf::Vector2f(CreditsButtonSpecs::get().mXOffset, CreditsButtonSpecs::get().mYOffset), CreditsButtonSpecs::get().mFilename),
	mUpdated(false)
{
}

void CreditsButton::activate() {

	//TODO: goto credits.
	if(mUpdated == false && InputManager::get().isADown()) {
		mUpdated = true;

		auto first_level_name = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_name");

//		auto loadingState_p = new LoadingState(first_level_name);
		auto& stateMgr = StateManager::get();

//		stateMgr.freezeState();
//		stateMgr.popState();
//		stateMgr.pushState(loadingState_p);
//		stateMgr.unfreezeState();
	}
}