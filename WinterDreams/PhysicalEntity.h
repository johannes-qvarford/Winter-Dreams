#ifndef INCLUDED_PHYSICALENTITY
#define INCLUDED_PHYSICALENTITY

#include "Entity.h"
#include <SFML\Graphics.hpp>
/*
//	Abstract base class for physical objects in the system.
*/
class PhysicalEntity : public Entity {
public:
	/*
	//	Concrete subclasses of PhysicalEntity has to define an update function.
	//	Update is supposed to represent the action performed by 
	//	concrete subclasses each update-loop.
	*/
	virtual void update(GameState* game) = 0;
	
	/*
	//	Each concrete subclass of PhysicalEntity has to define a
	//	function for drawing itself.
	*/
	virtual void drawSelf(sf::RenderWindow* window) = 0;
	/*
	//	Each concrete subclass of PhysicalEntity has to define a
	//	function for returning the objects hitbox. 
	*/
	virtual const sf::FloatRect& getHitBox() const = 0;
	/*
	//	Each concrete subclass of PhysicalEntity has to define what
	//	actions it is supposed to performed when it collides with 
	//	another physical entity.
	*/
	virtual void onCollision(PhysicalEntity* entityCollidedWith_p) = 0;

};

#endif