#ifndef INCLUDED_LIGHTPOINT
#define INCLUDED_LIGHTPOINT

#include "GraphicalEntity.h"
#include "Animation.h"

#include <SFML/Graphics/Rect.hpp>

////////////////////////////////////////////////////////////
// /LightPoint is a graphical light that the player can
// /collide with, and by doing so, the LightPoint may be destroyed.
// /The player's light-level will be increased
// /to a certain level, unless the player's light-level is higher than the LightPoints.
// /No interactions with the player can happen if the LightPoint is not enabled.
////////////////////////////////////////////////////////////
class LightPoint : public GraphicalEntity {
public:

	////////////////////////////////////////////////////////////
	// /Create a LightPoint at a position, with a certain brightness,
	// /that may or may not be used once, and starts enabled or disabled.
	////////////////////////////////////////////////////////////
	LightPoint(const sf::FloatRect& initialPosition, int lightLevel, bool once, bool startEnabled);

	////////////////////////////////////////////////////////////
	// /Ïncrease the player's light-level, if enabled.
	// /Set alive state to false, if only activate once.
	////////////////////////////////////////////////////////////
	void onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection);

	////////////////////////////////////////////////////////////
	// /Update the LightPoint
	////////////////////////////////////////////////////////////
	void update(SubLevel* state);

	////////////////////////////////////////////////////////////
	// /Draw the LightPoint.
	////////////////////////////////////////////////////////////
	void drawSelf();

	////////////////////////////////////////////////////////////
	// /Get the LightPoints hitbox. 
	////////////////////////////////////////////////////////////
	sf::FloatRect& getHitBox();

private:

	void addLightSource(SubLevel* subLevel_p);

	Animation* mCurrentAnimation_p;

	std::map<std::string, Animation> mAnimationMap;

	sf::FloatRect mHitBox;

	int mLightLevel;

	bool mOnce;
};

#endif