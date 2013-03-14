#ifndef INCLUDED_FOOTSTEP
#define INCLUDED_FOOTSTEP

#include "Script.h"

class FootStep : public Script {
public:

	FootStep(sf::Vector2f position, sf::Vector2i direction, std::string groundName, int lifeLength);

	void update(SubLevel* subLevel_p);

	void draw() const;

private:
	sf::Rect<float>						mHitBox;
/*
	sf::Vector2f						mPosition;*/
	//sf::Vector2i						mDirection;
	//std::shared_ptr<sf::Texture>		mTexture;
	std::shared_ptr<sf::SoundBuffer>	mBuffer_sp;
	std::shared_ptr<sf::Sound>			mSound_sp;
	bool								mICanHasInit;


};



#endif