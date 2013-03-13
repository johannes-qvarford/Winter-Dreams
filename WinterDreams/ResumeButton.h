#ifndef INCLUDED_RESUMEBUTTON
#define INCLUDED_RESUMEBUTTON

#include "Button.h"

class ResumeButton : public Button {
public:
	////////////////////////////////////////////////////////////
	// /Create a resume button.
	// /If no arguments is passed, the ResumeButton will use
	// /it's default texture and default fade times.
	////////////////////////////////////////////////////////////
	ResumeButton(const std::string& textureFileName = "", const unsigned int& fadeInTime = 90, const unsigned int& fadeOutTime = 90);

	////////////////////////////////////////////////////////////
	// /Does the following:
	// /pop state
	////////////////////////////////////////////////////////////
	void activate();

private:
	unsigned int mFadeInTime;
	unsigned int mFadeOutTime;
	bool mUpdated;
};

#endif