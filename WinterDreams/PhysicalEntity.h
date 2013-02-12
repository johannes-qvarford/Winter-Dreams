#ifndef INCLUDED_PHYSICALENTITY
#define INCLUDED_PHYSICALENTITY

#include "Entity.h"
#include <SFML\Graphics\Rect.hpp>
////////////////////////////////////////////////////////////
// /Abstract base class for physical objects
// /and points in the system.
////////////////////////////////////////////////////////////
class PhysicalEntity : public Entity {
public:
	////////////////////////////////////////////////////////////
	// /Each concrete subclass of PhysicalEntity has to define a
	// /function for returning the objects hitbox.
	////////////////////////////////////////////////////////////
	virtual sf::FloatRect& getHitBox() = 0;
	////////////////////////////////////////////////////////////
	// /Each concrete subclass of PhysicalEntity has to define what
	// /actions it is supposed to performed when it collides with 
	// /another physical entity.
	////////////////////////////////////////////////////////////
	virtual void onCollision(PhysicalEntity* entityCollidedWith_p, const sf::Rect<float>& intersection) = 0;

	virtual void drawSelf() = 0;
protected:
	PhysicalEntity( bool startEnabled = true ) : Entity (startEnabled) { }
};

#endif