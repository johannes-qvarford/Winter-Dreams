#ifndef INCLUDED_COLLISIONZONE
#define INCLUDED_COLLISIONZONE

#include "PhysicalEntity.h"
#include "SFML/Graphics/Rect.hpp"

class CollisionZone : public PhysicalEntity {
public: 

	CollisionZone(bool startsEnabled, sf::Rect<float> HitBox, bool onceCollisionCheck);

	void onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection);

	void update(GameState* gameState);

	sf::Rect<float>& getHitBox();

	sf::Vector2i getDirection();
protected:

	sf::Rect<float> mHitBox;
	bool mOnce;
};
#endif