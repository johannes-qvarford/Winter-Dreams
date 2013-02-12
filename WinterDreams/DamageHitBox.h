#ifndef INCLUDED_DAMAGEHITBOX
#define INCLUDED_DAMAGEHITBOX

#include <SFML\Graphics\Rect.hpp>
#include <memory>
#include "GraphicalEntity.h"

	#ifndef SHIPPING
#include "Animation.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "GameToScreen.h"
	#endif


namespace sf{
	class Sound;
};

class DamageHitBox : public GraphicalEntity{
public:
    enum DamageType{PICKAXE, LANTERN};
    ///////////////////////////////////////////////
    // /Needs a numeric value for the damage, a size and a type of damage.
    // /Damage types are defined in DamageHitBox.h
	// /The member variable mLifeTime (in the definition) describes it's life time
	// /----SHOULD ADD ITSELF TO THE ENTITY VECTOR----
    ///////////////////////////////////////////////
	DamageHitBox(const sf::Rect<float>& hitBox, unsigned int damage, DamageType type);
	~DamageHitBox();
    ///////////////////////////////////////////////
    // /Returns the rectal hitbox
    ///////////////////////////////////////////////
	sf::Rect<float>& getHitBox();
    ///////////////////////////////////////////////
    // /The death of a hitbox creeps closer
    ///////////////////////////////////////////////
	void update(SubLevel* subLevel_p);
    ///////////////////////////////////////////////
    // /Draws the hitbox (for debugging reasons)
    ///////////////////////////////////////////////
	void drawSelf();
	///////////////////////////////////////////////
	// /Defines what the DamageHitBox should do on collision.
	///////////////////////////////////////////////
	void onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection);
	///////////////////////////////////////////////
	// /Get the damage hitbox's damage type.
	///////////////////////////////////////////////
	DamageType getDamageType() const;
	///////////////////////////////////////////////
	// /Get the damage hitbox's damage amount.
	///////////////////////////////////////////////
	int getDamageAmount() const;

	sf::Vector2i getDirection() { return sf::Vector2i(0,0); }

private:
	unsigned int				mDamage;
	sf::Rect<float>				mHitBox;
	int							mLifeTime;
	DamageType					mDamageType;
	std::shared_ptr<sf::Sound>	mSound;
#ifndef SHIPPING
	Animation mAnimation;
#endif

};
#endif