#ifndef INCLUDED_LEVELSPLASH
#define INCLUDED_LEVELSPLASH

#include "Script.h"

class SubLevel;
////////////////////////////////////////////////////////////
// /A LevelSplash is an image that will be displayed at the
// /beginning of a level. The splash will only be displayed
// /once and will then destroy itself.
////////////////////////////////////////////////////////////
class LevelSplash :	public Script {
public:
	////////////////////////////////////////////////////////////
	// /A levelSplash needs a lifeTime, to know for how long
	// /it should be displayed. LifeTime is measured in frames.
	//
	// /It needs a fadeTime( in frames ) to know how long time it
	// /will take for it to fade away.
	//
	// /Lastly, it needs a level name to know which splash to load
	////////////////////////////////////////////////////////////
	LevelSplash(const std::string& splashFileName, const int lifeTime, const int fadeoutTime, const int fadeinTime, bool startEnabled = true);
	////////////////////////////////////////////////////////////
	// /Destructor
	////////////////////////////////////////////////////////////
	~LevelSplash();
	////////////////////////////////////////////////////////////
	// /While the level splash is drawn it locks the input manager
	// /so that the player cannot move.
	// /It keeps being drawn as long as it's lifetime is greater
	// /then zero.
	////////////////////////////////////////////////////////////
	void draw() const;
	////////////////////////////////////////////////////////////
	// /Reduces the level splash's remaining life time
	////////////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);

private:
	std::shared_ptr<sf::Texture> mLevelSplashTexture;
	sf::Sprite					 mLevelSplash;
	
	std::shared_ptr<sf::Shader>  mBlendShader;

	unsigned int				 mLifeTime;

	float						 mAlpha;

	unsigned int				 mFadeInTime;
	unsigned int				 mFadeInTimeCurrent;
	unsigned int				 mFadeOutTime;
	unsigned int				 mFadeOutTimeCurrent;
};

#endif

