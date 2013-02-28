#ifndef INCLUDED_LEVELENDER
#define INCLUDED_LEVELENDER

#include "CollisionZone.h"

////////////////////////////////////////////////////////////
// /LevelEnder ends the current level, and starts loading the
// /next level, when colliding with the player.
////////////////////////////////////////////////////////////
class LevelEnder : public CollisionZone {
public:

	LevelEnder(bool startsEnabled, const sf::FloatRect& hitBox, bool once);

	void update(SubLevel* subLevel_p);

	void onCollision(PhysicalEntity* physical_p, const sf::Rect<float>& intersection);

	void drawSelf();
private:

	SubLevel* mSubLevel_p;

};

#endif