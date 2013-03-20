#ifndef INCLUDED_MUSICVOLUMESLIDER
#define INCLUDED_MUSICVOLUMESLIDER

#include "ResourceManager.h"
#include "Button.h"
#include <string>

class MusicVolumeSlider : public Button {
public:
	////////////////////////////////////////////////////////////
	// /Create a sound volume slider "button".
	////////////////////////////////////////////////////////////
	MusicVolumeSlider();

	////////////////////////////////////////////////////////////
	// /Does the following:
	// /changes the sound volume
	////////////////////////////////////////////////////////////
	void activate();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void update();
private:
	
	float mSliderX, mSliderY, mStepSize;
	std::string mFilename;
	std::string mSliderFilename;
	std::shared_ptr<sf::Texture> mTexture;
	std::shared_ptr<sf::Texture> mSliderTexture;
	short mVolume;
	short mFrameCount;
	bool mUpdated;
};

#endif