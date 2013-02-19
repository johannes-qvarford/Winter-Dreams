#ifndef INCLUDED_CRYSTAL
#define INCLUDED_CRYSTAL

#include "GraphicalEntity.h"
#include "SolidZone.h"
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
	Crystal(const sf::FloatRect& position, bool startEnabled = true, int imgVersion = 1);
	//////////////////////////////////////////////////////
	// /No dynamicly allocated member variables
	//////////////////////////////////////////////////////
	~Crystal();
	//////////////////////////////////////////////////////
	// /Defines what Crystal should do each update
	//////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);
	//////////////////////////////////////////////////////
	// /Defines how the Crystal should draw itself
	//////////////////////////////////////////////////////
	void drawSelf();
	//////////////////////////////////////////////////////
	// /Returns the hitbox of the Crystal
	//////////////////////////////////////////////////////
	sf::FloatRect& getHitBox();
	//////////////////////////////////////////////////////
	// /Defines what the Crystal should do with the entity it collides with
	//////////////////////////////////////////////////////
	void onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection);
	//////////////////////////////////////////////////////
	// /Used for increasing och decreasing an Crystal's health.
	// /Send a negative argument to decrease the health.
	// /If the health reaches zero, or lower, the Crystal is tagged
	// /as inactive.
	//////////////////////////////////////////////////////
	void adjustHealth(int adjustment);
	//////////////////////////////////////////////////////
	// /This definition is requested by Physical Entity.
	// /Unless an object is dependent on it's direction,
	// /the return value is of no importence.
	//////////////////////////////////////////////////////
	sf::Vector2i getDirection() { return sf::Vector2i( 0,0 ); }
private:
	void updateAnimation();

	std::map<std::string, Animation> mAnimationMap; //The crystal's collection of Animations
	Animation*					mCurrentAnimation;	//The crystal's animation
	int							mHP;				//The crystal's HP
	std::shared_ptr<SolidZone>	mSolidZone;			//The crystal's collision zone
	int							mVersion;
	//No copies
	Crystal(const Crystal& crystal);
	//No copies
	Crystal& operator=(const Crystal& crystal);
};

#endif