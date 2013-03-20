#include "NarratorVolumeSlider.h"
#include "FileStructure.h"
#include "StateManager.h"
#include "LoadingState.h"
#include "PropertyManager.h"
#include "StateManager.h"
#include "InputManager.h"
#include "WindowManager.h"
#include <SFML/Graphics/Sprite.hpp>

class NarratorVolumeSliderSpecs {
public:

	static NarratorVolumeSliderSpecs& get(){ static NarratorVolumeSliderSpecs sSpecs; return sSpecs; }

	float mXOffset;

	float mYOffset;
	
	float mSliderX, mSliderY, mStepSize;

	std::string mFilename;

	std::string mSliderFilename;

private:

	NarratorVolumeSliderSpecs() {
		auto& mscvol = PropertyManager::get().getGeneralSettings().get_child("ui.settings.narratorvolume");
		auto& general = PropertyManager::get().getGeneralSettings().get_child("ui.settings.general.slider");
		mXOffset = mscvol.get<float>("xoffset");
		mYOffset = mscvol.get<float>("yoffset");
		mSliderX = mscvol.get<float>("sliderx");
		mSliderY = mscvol.get<float>("slidery");
		mStepSize = general.get<float>("stepsize");

		mFilename = mscvol.get<std::string>("filename");
		mSliderFilename = general.get<std::string>("filename");
	}
};

NarratorVolumeSlider::NarratorVolumeSlider():
	Button(sf::Vector2f(NarratorVolumeSliderSpecs::get().mXOffset, NarratorVolumeSliderSpecs::get().mYOffset), NarratorVolumeSliderSpecs::get().mSliderFilename),
	mUpdated(false)
{
	mTexture = ResourceManager::get().getTexture(FS_DIR_UI + NarratorVolumeSliderSpecs::get().mSliderFilename);
	mVolume = PropertyManager::get().getUserSettings()->get_child("volumes").get<short>("narratorVolume");
	mFrameCount=0;
}

void NarratorVolumeSlider::update(){
}

void NarratorVolumeSlider::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	Button::draw(target,states);
	sf::Sprite sprite(*mTexture);
	sprite.setPosition(NarratorVolumeSliderSpecs::get().mSliderX*target.getSize().x+(mVolume*NarratorVolumeSliderSpecs::get().mStepSize),NarratorVolumeSliderSpecs::get().mSliderY*target.getSize().y);
	WindowManager::get().getRenderWindow()->draw(sprite);
}

void NarratorVolumeSlider::activate() {
	
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
	usr.put<short>("narratorVolume",mVolume);
}