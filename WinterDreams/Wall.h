#ifndef INCLUDED_WALL
#define INCLUDED_WALL

#include <memory>
#include "SFML/Graphics/Rect.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include "PhysicalEntity.h"


namespace sf {
	class Texture;
}

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
	void onCollision(PhysicalEntity* pe, const sf::Rect<float>& intersection);

	/*
	//	Get the Wall's hitbox.
	*/
	sf::Rect<float>& getHitBox();

	/*
	//	Update the Wall(by default, do nothing).
	*/
	void update(GameState* gameState, int milliseconds);

	/*
	//	Draw self(by default, do nothing, if debugging, draw square)
	*/
	void drawSelf();

	/*
	//	Wall needs thisfd 
	*/
	sf::Vector2i getDirection();
private:
#ifdef DEBUG_WALL
	std::shared_ptr<sf::Texture> mTexture;
	sf::Sprite mSprite;
#endif
	sf::Rect<float> mHitBox;
};

#endif