#ifndef INCLUDED_PLAYERRELATED
#define INCLUDED_PLAYERRELATED

#include "PhysicalEntity.h"

class PlayerRelated : public PhysicalEntity {
public:
	/*
	//	Concrete subclasses of PlayerRelated has to define an update function.
	//	Update is supposed to represent the action performed by 
	//	concrete subclasses each update-loop.
	*/
	virtual void update(GameState* game) = 0;
	/*
	//	Concrete subclasses of PlayerRelated does not have to define a 
	//	function for drawing itself. 
	//	If a definition is not provided the drawSelf-function will 
	//	not provide a graphical representation of the object.
	*/
	virtual void drawSelf(sf::RenderWindow* window) {};

	// getHitBox() & onCollision();
};

#endif
