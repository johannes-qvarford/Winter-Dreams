#ifndef INCLUDED_LEVELENDER
#define INCLUDED_LEVELENDER

#include "Entity.h"
#include "Collidable.h"
#include "BaseHitBoxHaveable.h"

////////////////////////////////////////////////////////////
// /LevelEnder ends the current level, and starts loading the
// /next level, when colliding with the player.
// /WARNING: Should be updated to use onChangeEnabled.
////////////////////////////////////////////////////////////
class LevelEnder : public Entity, public Collidable, public BaseHitBoxHaveable {
public:

	LevelEnder(bool startsEnabled, const sf::FloatRect& hitBox);

	void update(SubLevel* subLevel_p);

	void onCollision(Collidable* physical_p, const sf::FloatRect& intersection);

private:
	SubLevel* mSubLevel_p;

};

#endif