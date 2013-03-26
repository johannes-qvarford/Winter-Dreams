#ifndef INCLUDED_DAMAGEHITBOX
#define INCLUDED_DAMAGEHITBOX

#include "Entity.h"
#include "Drawable.h"
#include "Collidable.h"
#include "BaseHitBoxHaveable.h"

#include <memory>

#include "Animation.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "GameToScreen.h"




namespace sf{
	class Sound;
};

class DamageHitBox : public BaseHitBoxHaveable, public Entity, public Collidable, public Drawable {
public:
    enum DamageType{PICKAXE, LANTERN};
    ///////////////////////////////////////////////
    // /Needs a numeric value for the damage, a size and a type of damage.
    // /Damage types are defined in DamageHitBox.h
	// /The member variable mLifeTime (in the definition) describes it's life time
	// /----SHOULD ADD ITSELF TO THE ENTITY VECTOR----
    ///////////////////////////////////////////////
	DamageHitBox(const sf::FloatRect& hitBox, unsigned int damage, const std::string& damageType);
	~DamageHitBox();

    ///////////////////////////////////////////////
    // /The death of a hitbox creeps closer
    ///////////////////////////////////////////////
	void update(SubLevel* subLevel_p);
    ///////////////////////////////////////////////
    // /Draws the damage animation
    ///////////////////////////////////////////////
	void draw();
	///////////////////////////////////////////////
	// /Defines what the DamageHitBox should do on collision.
	///////////////////////////////////////////////
	void onCollision(Collidable* col_p, const sf::FloatRect& intersection);
	///////////////////////////////////////////////
	// /Get the damage hitbox's damage type.
	///////////////////////////////////////////////
	const std::string& getDamageType() const;
	///////////////////////////////////////////////
	// /Get the damage hitbox's damage amount.
	///////////////////////////////////////////////
	int getDamageAmount() const;
	///////////////////////////////////////////////
	// /Disables the hitbox next frame. This is
	// /used if the hitbox should be able to effect
	// /several entitys the same frame.
	///////////////////////////////////////////////
	void disableNextFrame();

private:
	Animation*						mCurrentAnimation_p; //The current animation
	unsigned int					mDamage;
	sf::Rect<float>					mHitBox;
	int								mLifeTime;
	std::string						mDamageType;
	std::shared_ptr<sf::Sound>		mSound;

	bool							mQueDisable;
};
#endif
