#ifndef INCLUDED_RESOLUTIONBUTTON
#define INCLUDED_RESOLUTIONBUTTON

#include "Button.h"
#include <vector>
#include <SFML\Window\VideoMode.hpp>

class ResolutionButton : public Button {
public:
	////////////////////////////////////////////////////////////
	// /Create a resolution "slider-y" button.
	////////////////////////////////////////////////////////////
	ResolutionButton();

	////////////////////////////////////////////////////////////
	// /Does the following:
	// /changes the resolution
	////////////////////////////////////////////////////////////
	void activate();

private:
	std::vector<sf::VideoMode> mVideoModesVector;
	size_t mCurrentVideoMode;
	bool mCanSwitchVideoMode;
	bool mUpdated;
};

#endif