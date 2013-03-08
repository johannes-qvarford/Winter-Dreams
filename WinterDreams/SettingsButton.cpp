#include "SettingsButton.h"
#include "PropertyManager.h"
#include "InputManager.h"
#include "StateManager.h"
#include "ResourceManager.h"

#include <SFML/Audio/Music.hpp>

class SettingsButtonSpecs {
public:

	static SettingsButtonSpecs& get(){ static SettingsButtonSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;

	std::string mFilename;

	std::string mAudioFileName;

private:

	SettingsButtonSpecs() {
		auto& settings = PropertyManager::get().getGeneralSettings().get_child("ui.mainmenu.settings");
		mAudioFileName = settings.get<std::string>("audiofile");
		mXOffset = settings.get<float>("xoffset");
		mYOffset = settings.get<float>("yoffset");
		mFilename = settings.get<std::string>("filename");
	}
};


SettingsButton::SettingsButton():
	Button(sf::Vector2f(SettingsButtonSpecs::get().mXOffset, SettingsButtonSpecs::get().mYOffset), SettingsButtonSpecs::get().mFilename),
	mUpdated(false)
{
	mSoundBuffer = ResourceManager::get().getSoundBuffer( FS_DIR_SOUNDS + SettingsButtonSpecs::get().mAudioFileName );
	mActivationSound.setBuffer( *mSoundBuffer );
	mActivationSound.setVolume(50.f);
}

SettingsButton::~SettingsButton(){
	mActivationSound.stop();
}

void SettingsButton::activate() {

	//TODO: goto settings menu.
	if(mUpdated == false && InputManager::get().isADown()) {
		mUpdated = true;
		mActivationSound.play();

		auto first_level_name = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_name");

//		auto loadingState_p = new LoadingState(first_level_name);
		auto& stateMgr = StateManager::get();

//		stateMgr.freezeState();
//		stateMgr.popState();
//		stateMgr.pushState(loadingState_p);
//		stateMgr.unfreezeState();
	}
}