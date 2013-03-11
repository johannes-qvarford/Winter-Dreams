#include "ExitButton.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "StateManager.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "StateManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
class ExitButtonSpecs {
public:

	static ExitButtonSpecs& get(){ static ExitButtonSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;

	std::string mFilename;

	std::string mAudioFileName;

private:

	ExitButtonSpecs() {
		//exit(int) is a c function. use some other name
		auto& exit_ = PropertyManager::get().getGeneralSettings().get_child("ui.mainmenu.exit");
		mXOffset = exit_.get<float>("xoffset");
		mYOffset = exit_.get<float>("yoffset");
		mFilename = exit_.get<std::string>("filename");
		mAudioFileName = exit_.get<std::string>("audiofile");
	}
};

ExitButton::ExitButton():
	Button(sf::Vector2f(ExitButtonSpecs::get().mXOffset, ExitButtonSpecs::get().mYOffset), ExitButtonSpecs::get().mFilename),
	mUpdated(false)
{
	mSoundBuffer = ResourceManager::get().getSoundBuffer( FS_DIR_SOUNDS + ExitButtonSpecs::get().mAudioFileName );
	mActivationSound.setBuffer( *mSoundBuffer );
	mActivationSound.setVolume(20.f);
}

ExitButton::~ExitButton(){
	mActivationSound.stop();
}

void ExitButton::activate() {
	
	if(mUpdated == false && (InputManager::get().isADown() ||InputManager::get().isStartDown() ) ){
		mUpdated = true;
		mActivationSound.play();

		auto& stateMgr = StateManager::get();

		stateMgr.freezeState();
		stateMgr.popState();
	}
}
