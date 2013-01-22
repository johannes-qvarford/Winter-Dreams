#ifndef INCLUDED_WALL
#define INCLUDED_WALL

#include "SFML/Graphics/Rect.hpp"
#include "PhysicalEntity.h"

/*
//	A Wall is an Entity that blocks any player from entering it.
*/
class Wall : public PhysicalEntity {
public:

	/*
	//	Create a Wall at (x,y) with width and height
	*/
	Wall(float x, float y, float width, float height);

	/*
	//	Handle collision with a PlayerRelated.
	//	Move the entity away from the Wall if its a Player. 
	*/
	void onCollision(PhysicalEntity* pe);

	/*
	//	Get the Wall's hitbox.
	*/
	const sf::Rect<float>& getHitBox() const;

	/*
	//	Update the Wall(by default, do nothing).
	*/
	void update(GameState* gameState, int milliseconds);

	/*
	//	Draw self(by default, do nothing, if debugging, draw square)
	*/
	void drawSelf() const;

private:

	sf::Rect<float> mHitBox;
};

#endif