#ifndef INCLUDED_PHYSICALENTITY
#define INCLUDED_PHYSICALENTITY

#include "Entity.h"
#include <SFML\Graphics\Rect.hpp>
/*
//	Abstract base class for physical objects in the system.
*/
class PhysicalEntity : public Entity {
public:
	/*
	//	Each concrete subclass of PhysicalEntity has to define a
	//	function for drawing itself.
	*/
	virtual void drawSelf() = 0;
	/*
	//	Each concrete subclass of PhysicalEntity has to define a
	//	function for returning the objects hitbox.
	//	Jag var tvungen att ta bort const f�r att Wall m�ste f� modifiera pe /Gustav
	*/
	virtual sf::FloatRect& getHitBox() const = 0;
	/*
	//	Each concrete subclass of PhysicalEntity has to define what
	//	actions it is supposed to performed when it collides with 
	//	another physical entity.
	*/
	virtual void onCollision(PhysicalEntity* entityCollidedWith_p) = 0;

};

#endif