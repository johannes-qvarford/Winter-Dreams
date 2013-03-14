#include "SoundVolumeSlider.h"
#include "FileStructure.h"
#include "StateManager.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "StateManager.h"
#include "InputManager.h"
#include "WindowManager.h"
#include <SFML/Graphics/Sprite.hpp>

class SoundVolumeSliderSpecs {
public:

	static SoundVolumeSliderSpecs& get(){ static SoundVolumeSliderSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;
	
	float mSliderX, mSliderY, mStepSize;

	std::string mFilename;

	std::string mSliderFilename;

private:

	SoundVolumeSliderSpecs() {
		auto& sndvol = PropertyManager::get().getGeneralSettings().get_child("ui.settingsmenu.soundvolume");
		mXOffset = sndvol.get<float>("xoffset");
		mYOffset = sndvol.get<float>("yoffset");
		mSliderX = sndvol.get<float>("sliderx");
		mSliderY = sndvol.get<float>("slidery");
		mStepSize = sndvol.get<float>("stepsize");

		mFilename = sndvol.get<std::string>("filename");
		mSliderFilename = sndvol.get<std::string>("sliderfilename");
	}
};

SoundVolumeSlider::SoundVolumeSlider():
	Button(sf::Vector2f(SoundVolumeSliderSpecs::get().mXOffset, SoundVolumeSliderSpecs::get().mYOffset), SoundVolumeSliderSpecs::get().mFilename),
	mUpdated(false)
{
	mTexture = ResourceManager::get().getTexture(FS_DIR_UI + mSliderFilename);
}

void SoundVolumeSlider::update(){
	sf::Sprite sprite(*mTexture);
	sprite.setPosition(mSliderX+mVolume*mStepSize,mSliderY);
	WindowManager::get().getRenderWindow()->draw(sprite);
}

void SoundVolumeSlider::activate() {
	
	auto& im = InputManager::get();
	if (im.getStick().x<-0.75 && mVolume>0){
		mVolume-=mFrameCount*0.1f;
		mFrameCount+=2;
	} else if (im.getStick().x>0.75 && mVolume<100){
		mVolume+=mFrameCount*0.1f;
		mFrameCount+=2;
	}
	if (mFrameCount>0){
		--mFrameCount;
	}

	if(mUpdated == false && im.isADown()) {
		mUpdated = true;
		Button::activate();

		auto& stateMgr = StateManager::get();

		onHover(false);
	}
}