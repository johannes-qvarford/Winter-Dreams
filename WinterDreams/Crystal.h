#ifndef INCLUDED_CRYSTAL
#define INCLUDED_CRYSTAL

#include "Wall.h"
#include "Animation.h"
#include <SFML\Graphics\Rect.hpp>
namespace sf{
	class Texture;
};
//////////////////////////////////////////////////////
// /Represents ice blocks which can be destryed by hitting it
// /with a pickaxe.
//////////////////////////////////////////////////////
class Crystal : public Wall {
public:
	//////////////////////////////////////////////////////
	// /The FloatRect's top will represent the Crystal's Y.
	// /The FloatRect's left will represent the Crystal's X.
	// /The FloatRect's width and height sets bounds for it's hitbox
	//////////////////////////////////////////////////////
	Crystal(const sf::FloatRect& position);
	//////////////////////////////////////////////////////
	// /No dynamicly allocated member variables
	//////////////////////////////////////////////////////
	~Crystal();
	//////////////////////////////////////////////////////
	// /Defines what IceBlock should do each update
	//////////////////////////////////////////////////////
	void update(GameState* gameState_p, int milliseconds);
	//////////////////////////////////////////////////////
	// /Defines how the IceBlock should draw itself
	//////////////////////////////////////////////////////
	void drawSelf();
	//////////////////////////////////////////////////////
	// /Returns the hitbox of the IceBlock
	//////////////////////////////////////////////////////
	sf::FloatRect& getHitBox();
	//////////////////////////////////////////////////////
	// /Defines what the IceBlock should do with the entity it collides with
	//////////////////////////////////////////////////////
	void onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection);
	//////////////////////////////////////////////////////
	// /Used for increasing och decreasing an ice block's health.
	// /Send a negative argument to decrease the health.
	// /If the health reaches zero, or lower, the ice block is tagged
	// /as inactive.
	//////////////////////////////////////////////////////
	void adjustHealth(int adjustment);
private:
	sf::FloatRect mHitBox;							//The ice blocks hitbox
	std::map<std::string, Animation> mAnimationMap; //The ice blocks collection of Animations
	Animation* mCurrentAnimation;					//The ice blocks animation
	int mHP;										//The ice blocks HP
	//No copies
	Crystal(const Crystal& crystal);
	//No copies
	Crystal& operator=(const Crystal& crystal);
};

#endif