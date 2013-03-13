#ifndef INCLUDED_FOOTSTEP
#define INCLUDED_FOOTSTEP

#include "GraphicalEntity.h"


class FootStep : public GraphicalEntity {
public:

	FootStep(sf::Vector2f position, sf::Vector2i direction, std::string groundName, int lifeLength);

	void update(SubLevel* subLevel_p);

	void drawSelf();

	int getLayer();

	int getMinorLayer();

	sf::Rect<float>& getHitBox(); 

	void onCollision(PhysicalEntity * ent, const sf::Rect<float>& intersection);

private:
	sf::Rect<float>						mHitBox;
/*
	sf::Vector2f						mPosition;*/
	sf::Vector2i						mDirection;
	std::shared_ptr<sf::Texture>		mTexture;
	std::shared_ptr<sf::SoundBuffer>	mBuffer;
	std::shared_ptr<sf::Sound>			mSound;
	int									mLifeLength;
	int									mLifeLengthCounter;
	bool								mICanHasInit;


};



#endif