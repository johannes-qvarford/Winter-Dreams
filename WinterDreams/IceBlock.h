#ifndef INCLUDED_ICEBLOCK
#define INCLUDED_ICEBLOCK

#include "PhysicalEntity.h"
#include "Animation.h"
#include <SFML\Graphics\Rect.hpp>
namespace sf{
	class Texture;
};
//////////////////////////////////////////////////////
// /Represents ice blocks which can be destryed by hitting it
// /with a pickaxe.
//////////////////////////////////////////////////////
class IceBlock : public PhysicalEntity {
public:
	IceBlock(const sf::FloatRect& position);
	IceBlock(const IceBlock& iceBlock);
	//////////////////////////////////////////////////////
	// /No dynamicly allocated member variables
	//////////////////////////////////////////////////////
	~IceBlock();
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
	const sf::FloatRect& getHitBox() const;
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
	sf::FloatRect mHitBox;	//The ice blocks hitbox
	Animation mAnimation;	//The ice blocks animation
	int mHP;				//The ice blocks HP
};

#endif