#ifndef INCLUDED_RESUMEBUTTON
#define INCLUDED_RESUMEBUTTON

#include "Button.h"

class ResumeButton : public Button {
public:
	////////////////////////////////////////////////////////////
	// /Create a resume button.
	////////////////////////////////////////////////////////////
	ResumeButton();

	////////////////////////////////////////////////////////////
	// /Does the following:
	// /pop state
	////////////////////////////////////////////////////////////
	void activate();

private:

	bool mUpdated;
};

#endif