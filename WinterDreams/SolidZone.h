#ifndef INCLUDED_SOLIDZONE
#define INCLUDED_SOLIDZONE

#include "CollisionZone.h"
#include "SFML\Graphics\Rect.hpp"

class SolidZone : public CollisionZone {
public:

	SolidZone(sf::Rect<float> HitBox, bool startsEnabled);

	
	void onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection);

	void drawSelf();


private:
#ifdef DEBUG_SOLIDZONE
	std::shared_ptr<sf::Texture> mTexture;
	sf::Sprite mSprite;
#endif

};




#endif