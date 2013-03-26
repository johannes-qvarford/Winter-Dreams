#ifndef INCLUDED_CRYSTAL
#define INCLUDED_CRYSTAL

#include "Entity.h"
#include "Drawable.h"
#include "Collidable.h"
#include "BaseHitBoxHaveable.h"

class SolidZone;

#include "Animation.h"
//////////////////////////////////////////////////////
// /Represents ice blocks which can be destryed by hitting it
// /with a pickaxe.
//////////////////////////////////////////////////////
class Crystal : public Entity, public Drawable, public Collidable, public BaseHitBoxHaveable {
public:
	//////////////////////////////////////////////////////
	// /The FloatRect's top will represent the Crystal's Y.
	// /The FloatRect's left will represent the Crystal's X.
	// /The FloatRect's width and height sets bounds for it's hitbox
	//////////////////////////////////////////////////////
	Crystal(const sf::FloatRect& position, bool startEnabled = true);

	//////////////////////////////////////////////////////
	// /Defines what Crystal should do each update
	//////////////////////////////////////////////////////
	void update(SubLevel* subLevel_p);
	//////////////////////////////////////////////////////
	// /Defines how the Crystal should draw itself
	//////////////////////////////////////////////////////
	void draw();
	//////////////////////////////////////////////////////
	// /Defines what the Crystal should do with the entity it collides with
	//////////////////////////////////////////////////////
	void onCollision(Collidable* col_p, const sf::FloatRect& intersection);
	//////////////////////////////////////////////////////
	// /Used for increasing och decreasing an Crystal's health.
	// /Send a negative argument to decrease the health.
	// /If the health reaches zero, or lower, the Crystal is tagged
	// /as inactive.
	//////////////////////////////////////////////////////
	void adjustHealth(int adjustment);

	//////////////////////////////////////////////////////
	// /Get the Crystal's health.
	//////////////////////////////////////////////////////
	int getHealth() const;

private:
	void updateAnimation();

	std::map<std::string, Animation> mAnimationMap; //The crystal's collection of Animations
	Animation*					mCurrentAnimation;	//The crystal's animation
	int							mHP;				//The crystal's HP
	std::shared_ptr<SolidZone>	mSolidZone;			//The crystal's collision zone(NOTE: its hitbox will not be used for collision detection!)
	std::shared_ptr<sf::SoundBuffer> mSoundBuffer;  //The crystal's "smashed" sound
	std::shared_ptr<sf::SoundBuffer> mDestroyedSoundBuffer;//The crystal's "destroyed completely" sound.
	int							mVersion;
	//No copies
	Crystal(const Crystal& crystal);
	//No copies
	Crystal& operator=(const Crystal& crystal);
};

#endif
