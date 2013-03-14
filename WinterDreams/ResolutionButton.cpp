#include "ResolutionButton.h"
#include "ResourceManager.h"
#include "FileStructure.h"
#include "StateManager.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "StateManager.h"
#include "InputManager.h"
#include "WindowManager.h"

class ResolutionButtonSpecs {
public:

	static ResolutionButtonSpecs& get(){ static ResolutionButtonSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;

	std::string mFilename;

private:

	ResolutionButtonSpecs() {
		auto& resolution = PropertyManager::get().getGeneralSettings().get_child("ui.settingsmenu.resolution");
		mXOffset = resolution.get<float>("xoffset");
		mYOffset = resolution.get<float>("yoffset");
		mFilename = resolution.get<std::string>("filename");
	}
};

ResolutionButton::ResolutionButton():
	Button(sf::Vector2f(ResolutionButtonSpecs::get().mXOffset, ResolutionButtonSpecs::get().mYOffset), ResolutionButtonSpecs::get().mFilename),
	mUpdated(false),
	mCurrentVideoMode(0)
{
	mVideoModesVector = sf::VideoMode::getFullscreenModes();
}

void ResolutionButton::activate() {
	
	auto& im = InputManager::get();
	if (mCanSwitchVideoMode){
		if (im.getStick().x<-0.75 && mCurrentVideoMode!=0){
			--mCurrentVideoMode;
			mCanSwitchVideoMode=0;
		} else if (im.getStick().x>0.75 && mCurrentVideoMode!=mVideoModesVector.size()-1){
			++mCurrentVideoMode;
			mCanSwitchVideoMode=0;
		}
	}

	if(mUpdated == false && im.isADown()) {
		mUpdated = true;
		Button::activate();

		auto& stateMgr = StateManager::get();

		WindowManager::get().setVideoMode(mVideoModesVector[mCurrentVideoMode].width,mVideoModesVector[mCurrentVideoMode].height,4);

		onHover(false);
	}
}