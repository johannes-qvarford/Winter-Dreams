#ifndef INCLUDED_SUBLEVELFADE
#define INCLUDED_SUBLEVELFADE

#include "Entity.h"
#include "Drawable.h"
#include "BaseHitBoxHaveable.h"

class SubLevelFade: public Entity, public Drawable, public BaseHitBoxHaveable{
public:
	enum FadeType{
		FADE_IN,
		FADE_OUT
	};

	SubLevelFade(int fadeTime, FadeType type);

	void update(SubLevel* subLevel_p);

	void draw();

private:
	int mFadeTime;
	FadeType mType;
	float mAlpha;
	int mWaitingFrames;
};



#endif