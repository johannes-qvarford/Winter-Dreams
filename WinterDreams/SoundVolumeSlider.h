#ifndef INCLUDED_SOUNDVOLUMESLIDER
#define INCLUDED_SOUNDVOLUMESLIDER

#include "ResourceManager.h"
#include "Button.h"
#include <string>

class SoundVolumeSlider : public Button {
public:
	////////////////////////////////////////////////////////////
	// /Create a sound volume slider "button".
	////////////////////////////////////////////////////////////
	SoundVolumeSlider();

	////////////////////////////////////////////////////////////
	// /Does the following:
	// /changes the sound volume
	////////////////////////////////////////////////////////////
	void activate();

	void update();
private:
	
	float mSliderX, mSliderY, mStepSize;
	std::string mSliderFilename;
	std::shared_ptr<sf::Texture> mTexture;
	short mVolume;
	short mFrameCount;
	bool mUpdated;
};

#endif