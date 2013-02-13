#ifndef INCLUDED_COLLISIONZONE
#define INCLUDED_COLLISIONZONE

#include "PhysicalEntity.h"
#include "SFML/Graphics/Rect.hpp"

class CollisionZone : public PhysicalEntity {
public: 

	CollisionZone(bool startsEnabled, sf::Rect<float> HitBox, bool onceCollisionCheck);

	void onCollision(PhysicalEntity* pe_p, const sf::Rect<float>& intersection);

	void update(SubLevel* subLevel_p);

	sf::Rect<float>& getHitBox();
protected:

	sf::Rect<float> mHitBox;
	bool mOnce;
};
#endif