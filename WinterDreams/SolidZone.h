#ifndef INCLUDED_SOLIDZONE
#define INCLUDED_SOLIDZONE

#include "Entity.h"
#include "Collidable.h"
#include "BaseHitBoxHaveable.h"

class SolidZone : public Entity, public Collidable, public BaseHitBoxHaveable {
public:

	SolidZone(const sf::FloatRect& hitBox, bool startsEnabled);
	
	void update(SubLevel* subLevel_p);

	void onCollision(Collidable* col_p, const sf::Rect<float>& intersection);

#ifdef DEBUG_SOLIDZONE
	void draw();
#endif
private:
	bool mOnce;
#ifdef DEBUG_SOLIDZONE
	sf::Sprite mSprite;
#endif
};

#endif
