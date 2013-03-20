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

	std::string mAudioFileName;

private:

	PlayButtonSpecs() {
		auto& play = PropertyManager::get().getGeneralSettings().get_child("ui.mainmenu.play");
		mAudioFileName = play.get<std::string>("audiofile");
		mXOffset = play.get<float>("xoffset");
		mYOffset = play.get<float>("yoffset");
		mFilename = play.get<std::string>("filename");
	}
};

PlayButton::PlayButton():
	Button(sf::Vector2f(PlayButtonSpecs::get().mXOffset, PlayButtonSpecs::get().mYOffset), PlayButtonSpecs::get().mFilename),
	mUpdated(false)
{
	mSoundBuffer = ResourceManager::get().getSoundBuffer( FS_DIR_SOUNDS + PlayButtonSpecs::get().mAudioFileName );
	mActivationSound.setBuffer( *mSoundBuffer );
	mActivationSound.setVolume(20.f);
}

PlayButton::~PlayButton(){
	mActivationSound.stop();
}

int counter = 0;

void PlayButton::activate() {
	
	if((InputManager::get().isADown() ||InputManager::get().isStartDown() ) ){

		mActivationSound.play();

		auto first_level_name = PropertyManager::get().getGeneralSettings().get<std::string>("first_level_name");
		auto intromovie = PropertyManager::get().getGeneralSettings().get<std::string>("levels.onetime_intro_video");
		auto introaudio = PropertyManager::get().getGeneralSettings().get<std::string>("levels.onetime_intro_music");

		auto loadingState_p = new LoadingVideoState(first_level_name);
#ifdef SHIPPING
		auto videoState_p = new VideoState(intromovie, introaudio, true);
#else
		auto videoState_p = new VideoState(intromovie, introaudio, true);
#endif
		auto& stateMgr = StateManager::get();

		stateMgr.freezeState(100);
		stateMgr.popState();
		stateMgr.pushState(loadingState_p);
		stateMgr.pushState(videoState_p);
		stateMgr.unfreezeState(100);
	}
}
