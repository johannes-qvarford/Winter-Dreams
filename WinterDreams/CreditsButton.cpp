#include "CreditsButton.h"
#include "PropertyManager.h"
#include "InputManager.h"
#include "StateManager.h"
#include "MenuState.h"
#include "ResourceManager.h"

class CreditsButtonSpecs {
public:

	static CreditsButtonSpecs& get(){ static CreditsButtonSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;

	std::string mFilename;

	std::string mAudioFileName;

private:

	CreditsButtonSpecs() {
		auto& credits = PropertyManager::get().getGeneralSettings().get_child("ui.mainmenu.credits");
		mXOffset = credits.get<float>("xoffset");
		mYOffset = credits.get<float>("yoffset");
		mFilename = credits.get<std::string>("filename");
		mAudioFileName = credits.get<std::string>("audiofile");
	}
};


CreditsButton::CreditsButton():
	Button(sf::Vector2f(CreditsButtonSpecs::get().mXOffset, CreditsButtonSpecs::get().mYOffset), CreditsButtonSpecs::get().mFilename),
	mUpdated(false)
{
	mSoundBuffer = ResourceManager::get().getSoundBuffer( FS_DIR_SOUNDS + CreditsButtonSpecs::get().mAudioFileName );
	mActivationSound.setBuffer( *mSoundBuffer );
	mActivationSound.setVolume(20.f);
}

CreditsButton::~CreditsButton(){
	mActivationSound.stop();
}

void CreditsButton::activate() {

	if((InputManager::get().isADown() ||InputManager::get().isStartDown() ) ){
		
		mActivationSound.play();
		
		auto creditsmenu_p = MenuState::makeCreditsMenuState();
		auto& stateMgr = StateManager::get();

		stateMgr.freezeState();
		stateMgr.pushState( creditsmenu_p );
		stateMgr.unfreezeState();
	}
}