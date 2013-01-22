#ifndef INCLUDED_PLAYERRELATED
#define INCLUDED_PLAYERRELATED

#include "PhysicalEntity.h"
#include <SFML\Graphics.hpp>
/*
//	Abstract base class for the player class and and objects
//	directly related to or created by the player.
*/
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
	/*
	//	Each concrete subclass of PlayerRelated has to define a
	//	function for returning the objects hitbox. 
	*/
	virtual const sf::FloatRect& getHitBox() const = 0;
	/*
	//	Each concrete subclass of PlayerRelated has to define what
	//	actions it is supposed to performed when it collides with 
	//	another physical entity.
	*/
	virtual void onCollision(PhysicalEntity* entityCollidedWith_p) = 0;
};

#endif
