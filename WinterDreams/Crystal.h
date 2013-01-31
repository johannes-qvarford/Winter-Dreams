#ifndef INCLUDED_CRYSTAL
#define INCLUDED_CRYSTAL

#include "GraphicalEntity.h"
//#include "SolidZone.h"
#include "Animation.h"
#include <SFML\Graphics\Rect.hpp>

class SolidZone;
//////////////////////////////////////////////////////
// /Represents ice blocks which can be destryed by hitting it
// /with a pickaxe.
//////////////////////////////////////////////////////
class Crystal : public GraphicalEntity {
public:
	//////////////////////////////////////////////////////
	// /The FloatRect's top will represent the Crystal's Y.
	// /The FloatRect's left will represent the Crystal's X.
	// /The FloatRect's width and height sets bounds for it's hitbox
	//////////////////////////////////////////////////////
	Crystal(const sf::FloatRect& position, bool startEnabled);
	//////////////////////////////////////////////////////
	// /No dynamicly allocated member variables
	//////////////////////////////////////////////////////
	~Crystal();
	//////////////////////////////////////////////////////
	// /Defines what IceBlock should do each update
	//////////////////////////////////////////////////////
	void update(GameState* gameState_p);
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
	std::map<std::string, Animation> mAnimationMap; //The crystal's collection of Animations
	Animation* mCurrentAnimation;					//The crystal's animation
	int mHP;										//The crystal's HP
	std::shared_ptr<SolidZone> mSolidZone;			//The crystal's collision zone

	//No copies
	Crystal(const Crystal& crystal);
	//No copies
	Crystal& operator=(const Crystal& crystal);
};

#endif