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
		auto& sndvol = PropertyManager::get().getGeneralSettings().get_child("ui.settings.soundvolume");
		auto& general = PropertyManager::get().getGeneralSettings().get_child("ui.settings.general.slider");
			mXOffset = sndvol.get<float>("xoffset");
		mYOffset = sndvol.get<float>("yoffset");
		mSliderX = sndvol.get<float>("sliderx");
		mSliderY = sndvol.get<float>("slidery");
		mStepSize = general.get<float>("stepsize");

		mFilename = sndvol.get<std::string>("filename");
		mSliderFilename = general.get<std::string>("filename");
	}
};

SoundVolumeSlider::SoundVolumeSlider():
	Button(sf::Vector2f(SoundVolumeSliderSpecs::get().mXOffset, SoundVolumeSliderSpecs::get().mYOffset), SoundVolumeSliderSpecs::get().mSliderFilename),
	mUpdated(false)
{
	mTexture = ResourceManager::get().getTexture(FS_DIR_UI + SoundVolumeSliderSpecs::get().mSliderFilename);
	mVolume = PropertyManager::get().getUserSettings()->get_child("volumes").get<short>("volumes.soundVolume");
	mFrameCount=0;
}

void SoundVolumeSlider::update(){
}

void SoundVolumeSlider::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	Button::draw(target,states);
	sf::Sprite sprite(*mTexture);
	sprite.setPosition(SoundVolumeSliderSpecs::get().mSliderX*target.getSize().x+(mVolume*SoundVolumeSliderSpecs::get().mStepSize),SoundVolumeSliderSpecs::get().mSliderY*target.getSize().y);
	WindowManager::get().getRenderWindow()->draw(sprite);
}

void SoundVolumeSlider::activate() {
	
	auto& im = InputManager::get();
	if (im.getStick().x<-0.70){
		mVolume-=mFrameCount*0.05f;
		mFrameCount+=2;
	} else if (im.getStick().x>0.70){
		mVolume+=mFrameCount*0.05f;
		mFrameCount+=2;
	}
	if (mFrameCount>0){
		--mFrameCount;
	}
	if (mFrameCount>40){
		--mFrameCount;
	}
	if (mVolume<0){
		mVolume=0;
	}
	if (mVolume>100){
		mVolume=100;
	}
	if(mUpdated == false && im.isADown()) {
		mUpdated = true;
		Button::activate();

		auto& stateMgr = StateManager::get();

		onHover(false);
	}
	auto& usr = PropertyManager::get().getUserSettings()->get_child("volumes");
	usr.put<short>("soundVolume",mVolume);
}