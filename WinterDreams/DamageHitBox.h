#ifndef INCLUDED_DAMAGEHITBOX
#define INCLUDED_DAMAGEHITBOX

#include <SFML\Graphics\Rect.hpp>
#include "PlayerRelated.h"

class DamageHitBox:public PlayerRelated{
public:
    enum DamageTypes{PICKAXE, LANTERN};
    ///////////////////////////////////////////////
    // /Needs a numeric value for the damage, a size and a type of damage.
    // /Damage types are defined in DamageHitBox.h
	// /The member variable mLifeTime (in the definition) describes it's life time
	// /----SHOULD ADD ITSELF TO THE ENTITY VECTOR----
    ///////////////////////////////////////////////
	DamageHitBox(unsigned int damage, const sf::Rect<float>& hitBox, DamageTypes type);
	~DamageHitBox();
    ///////////////////////////////////////////////
    // /Returns the rectal hitbox
    ///////////////////////////////////////////////
	sf::Rect<float>& getHitBox() const;
    ///////////////////////////////////////////////
    // /The death of a hitbox creeps closer
    ///////////////////////////////////////////////
	void update(GameState* gameState_p, int milliseconds);
    ///////////////////////////////////////////////
    // /Draws the hitbox (for debugging reasons)
    ///////////////////////////////////////////////
	void drawSelf();
	///////////////////////////////////////////////
	// /Defines what the DamageHitBox should do on collision.
	///////////////////////////////////////////////
	void onCollision(PhysicalEntity* entityCollidedWith_p, const sf::FloatRect& intersection);

private:
	unsigned int mDamage;
	sf::Rect<float> mHitBox;
	int mLifeTime;
	DamageTypes mDamageType;
};
#endif