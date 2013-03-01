#ifndef INCLUDED_SUBLEVELFADE
#define INCLUDED_SUBLEVELFADE

#include "Script.h"

class SubLevelFade: public Script{
public:
	enum FadeType{
		FADE_IN,
		FADE_OUT
	};

	SubLevelFade(int fadeTime, FadeType type);

	void update(SubLevel* subLevel_p);

	void draw() const;

private:
	int mFadeTime;
	FadeType mType;
	float mAlpha;
	int mWaitingFrames;
};



#endif