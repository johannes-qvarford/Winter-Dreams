#ifndef INCLUDED_PHYSICALENTITY
#define INCLUDED_PHYSICALENTITY

#include "Entity.h"

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

};

#endif