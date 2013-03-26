#ifndef INCLUDED_COLLIDABLE
#define INCLUDED_COLLIDABLE

#include "HitBoxHaveable.h"

/////////////////////////////////////////////////////////
// /Collidables are things that can collidable with other things.
/////////////////////////////////////////////////////////
class Collidable : virtual public HitBoxHaveable {
public:

	virtual void onCollision(Collidable* col_p, const sf::FloatRect& intersection) = 0;

};


#endif