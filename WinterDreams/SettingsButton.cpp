#include "SettingsButton.h"
#include "PropertyManager.h"
#include "InputManager.h"
#include "StateManager.h"
#include "ResourceManager.h"
#include "MenuState.h"
#include "WindowManager.h"

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
	mActivationSound.setVolume(20.f);
}

SettingsButton::~SettingsButton(){
	mActivationSound.stop();
}

void SettingsButton::activate() {
	if( (InputManager::get().isADown() ||InputManager::get().isStartDown() ) ){

		mActivationSound.play();

		auto texture = ResourceManager::get().getTexture(FS_DIR_UI + "mainmenu2.png");

	//	auto menustate = MenuState::makeSettingsMenuState(*texture);

		auto& stateMgr = StateManager::get();

		stateMgr.freezeState();
	//	stateMgr.pushState(menustate);
		stateMgr.unfreezeState();
	}
}