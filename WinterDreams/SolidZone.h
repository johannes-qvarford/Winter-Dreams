#ifndef INCLUDED_SOLIDZONE
#define INCLUDED_SOLIDZONE

#include "CollisionZone.h"
#include "SFML/Graphics/Rect.hpp"

class SolidZone : public CollisionZone {
public:

	SolidZone(sf::Rect<float> HitBox, bool startsEnabled);
	
	void onCollision(PhysicalEntity* pe_p, const sf::Rect<float>& intersection);

	void drawSelf();

private:
#ifdef DEBUG_SOLIDZONE
	sf::Sprite mSprite;
#endif
};

#endif
