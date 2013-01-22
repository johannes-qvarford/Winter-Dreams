#ifndef INCLUDED_DAMAGEHITBOX
#define INCLUDED_DAMAGEHITBOX

#include <SFML\Graphics\Rect.hpp>
#include "PlayerRelated.h"

class DamageHitBox:public PlayerRelated{
public:
    enum DamageTypes{NORMAL, ANNAT};
    /*
    //  Needs a numeric value for the damage, a size and a type of damage.
    //  Damage types are defined in DamageHitBox.h
    */
	DamageHitBox(unsigned int damage, const sf::Rect<float>& hitBox, DamageTypes type);
	~DamageHitBox();
    /*
    //  Returns the rectal hitbox
    */
	const sf::Rect<float>& getHitBox() const;
    /*
    //  The death of a hitbox creeps closer
    */
	void update(int milliseconds);
    /*
    //  Draws the hitbox (for debugging reasons)
    */
	void drawSelf();
private:
	unsigned int mDamage;
	sf::Rect<float> mHitBox;
	int mMilliseconds;
};
#endif