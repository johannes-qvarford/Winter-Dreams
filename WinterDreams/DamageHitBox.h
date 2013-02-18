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
	DamageHitBox(const sf::Rect<float>& hitBox, unsigned int damage, std::string damageType);
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
	std::string getDamageType() const;
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

	sf::Vector2i getDirection() { return sf::Vector2i(0,0); }

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